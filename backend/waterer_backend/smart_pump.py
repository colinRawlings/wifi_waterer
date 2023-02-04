# !python3

###############################################################
# Imports
###############################################################

import json
import logging
import os
import traceback as tb
import typing as ty
from datetime import datetime
from threading import Event, Lock, Thread
from time import time

import numpy as np
from waterer_backend.config import get_history_filepath
from waterer_backend.embedded_arduino import EmbeddedArduino
from waterer_backend.models import (
    SmartPumpSettings,
    SmartPumpStatus,
    SmartPumpStatusData,
    SmartPumpStatusHistory,
)
from waterer_backend.request import Request
from waterer_backend.response import Response
from waterer_backend.status_log import BinaryStatusLog, FloatStatusLog
from waterer_backend.utils import update_spans_activation_time

###############################################################
# Logging
###############################################################

_LOGGER = logging.getLogger(__name__)


###############################################################
# Classes
###############################################################


class SmartPump(Thread):
    def __init__(
        self,
        channel: int,
        device: ty.Optional[EmbeddedArduino],
        settings: SmartPumpSettings,
        status_update_interval_s: float = 5,
        allow_load_history: bool = False,
        auto_save_interval_s: ty.Optional[int] = 3600,
    ) -> None:

        Thread.__init__(self)

        self._device = device
        if device is None:
            _LOGGER.warning("Pump created without valid device")

        self._channel = channel

        self._settings_lock = Lock()

        with self._settings_lock:
            self._settings = settings

        self._sleep_event = Event()
        self._abort_running = False

        self._status_update_interval_s = status_update_interval_s
        self._auto_save_interval_s = auto_save_interval_s

        if allow_load_history:
            self.load_history()
        else:
            self._init_logs()

        self._last_feedback_update_time: ty.Optional[datetime] = None
        self._last_auto_save_time: float = time()

    def _init_logs(self):
        self._rel_humidity_V_log = FloatStatusLog()
        self._smoothed_rel_humidity_V_log = FloatStatusLog()
        self._pump_status_log = BinaryStatusLog()

    @property
    def channel(self) -> int:
        return self._channel

    @property
    def settings(self) -> SmartPumpSettings:
        with self._settings_lock:
            return self._settings

    @settings.setter
    def settings(self, value: SmartPumpSettings) -> None:
        with self._settings_lock:
            self._settings = value
            _LOGGER.info(f"New setting for channel {self._channel}: {self._settings}")
            self._sleep_event.set()

    def save_history(self) -> None:

        history = SmartPumpStatusData(
            rel_humidity_V_log=self._rel_humidity_V_log.to_data(),
            smoothed_rel_humidity_V_log=self._smoothed_rel_humidity_V_log.to_data(),
            pump_status_log=self._pump_status_log.to_data(),
        )

        filepath = get_history_filepath(self._channel)

        os.makedirs(filepath.parent, exist_ok=True)

        with open(filepath, "w") as fh:
            json.dump(history.dict(), fh)

        _LOGGER.info(f"Saved history for pump {self._channel} to {filepath}")

    def load_history(self) -> None:
        filepath = get_history_filepath(self._channel)

        if not filepath.is_file():
            _LOGGER.info(
                f"Failed to find history file to load for pump {self._channel}: {filepath}"
            )
            self._init_logs()
            return

        with open(filepath, "r") as fh:
            history_dict = json.load(fh)

        try:
            history = SmartPumpStatusData(**history_dict)
        except Exception as e:
            _LOGGER.error(f"Failed to parse history file: {filepath} with exception{e}")
            self._init_logs()
            return

        _LOGGER.info(f"Loaded history for pump {self._channel} from {filepath}")

        self._rel_humidity_V_log = FloatStatusLog.from_data(history.rel_humidity_V_log)
        self._smoothed_rel_humidity_V_log = FloatStatusLog.from_data(
            history.smoothed_rel_humidity_V_log
        )
        self._pump_status_log = BinaryStatusLog.from_data(history.pump_status_log)

    def _check_response(self, desc: str, response: Response) -> None:
        if not response.success:
            raise RuntimeError(f"Failed to {desc}: {response.message}")

    def _pcnt_from_V_humidity(
        self, rel_humidity_V: ty.Union[None, float, ty.List[ty.Optional[float]]]
    ) -> ty.Union[None, float, ty.List[float]]:

        if rel_humidity_V is None:
            return None

        if isinstance(rel_humidity_V, list):

            arr = np.asarray(rel_humidity_V)
            none_vals = arr == None
            arr[none_vals] = 0

            scaled_arr = (
                (self._settings.dry_humidity_V - arr)
                / (self._settings.dry_humidity_V - self._settings.wet_humidity_V)
                * 100
            )

            scaled_optional_arr = scaled_arr
            scaled_optional_arr[none_vals] = None
            return scaled_optional_arr.tolist()

        else:
            return (
                (self._settings.dry_humidity_V - rel_humidity_V)
                / (self._settings.dry_humidity_V - self._settings.wet_humidity_V)
                * 100
            )

    def _smoothed_humidity(self, rel_humidity_V: float) -> ty.Optional[float]:
        alpha = 1.0 / self._settings.num_smoothing_samples
        _, last_value = self._smoothed_rel_humidity_V_log.get_newest_value()
        _, last_status = self._pump_status_log.get_newest_value()

        if last_status:
            # value not reliable while pump is running
            return None

        if last_value is None:
            return rel_humidity_V

        return alpha * rel_humidity_V + (1 - alpha) * last_value

    def turn_on(self, duration_s: int = 0):

        if self._device is None:
            _LOGGER.warning("No device connected")
            return

        response = self._device.make_request(
            Request(
                channel=self.channel,
                instruction="turn_on",
                data=duration_s,
            )
        )
        self._check_response("turn_on", response)

    def turn_off(self):

        if self._device is None:
            _LOGGER.warning("No device connected")
            return

        response = self._device.make_request(
            Request(
                channel=self.channel,
                instruction="turn_off",
                data=0,
            )
        )
        self._check_response("turn_off", response)

    def _update_status(self) -> bool:

        if self._device is None:
            _LOGGER.warning("No device connected")
            return False

        _LOGGER.debug(f"Collecting status of pump: {self._channel}")

        ok, response = self._make_request_safe(
            Request(channel=self.channel, instruction="get_voltage", data=0)
        )
        if not ok:
            return False

        assert response is not None

        rel_humidity_V = response.data
        rel_humidity_pcnt = self._pcnt_from_V_humidity(rel_humidity_V)
        smoothed_rel_humidity_V = self._smoothed_humidity(rel_humidity_V)

        ok, response = self._make_request_safe(
            Request(channel=self.channel, instruction="get_state", data=0)
        )
        if not ok:
            return False

        assert response is not None

        pump_status = bool(response.data)
        status_time = time()

        # log

        self._pump_status_log.add_sample(status_time, pump_status)
        self._rel_humidity_V_log.add_sample(status_time, rel_humidity_V)
        self._smoothed_rel_humidity_V_log.add_sample(
            status_time, smoothed_rel_humidity_V
        )

        # save

        if self._auto_save_interval_s is not None and (
            (time() - self._last_auto_save_time) > self._auto_save_interval_s
        ):
            self.save_history()
            self._last_auto_save_time = time()

        return True

    @property
    def status(self) -> SmartPumpStatus:

        with self._settings_lock:

            self._update_status()

            status_time, pump_status = self._pump_status_log.get_newest_value()
            assert status_time is not None
            assert pump_status is not None

            _, rel_humidity_V = self._rel_humidity_V_log.get_newest_value()
            assert rel_humidity_V is not None

            rel_humidity_pcnt = self._pcnt_from_V_humidity(rel_humidity_V)
            assert isinstance(rel_humidity_pcnt, float)

            (
                _,
                smoothed_rel_humidity_V,
            ) = self._smoothed_rel_humidity_V_log.get_newest_value()

            smoothed_rel_humidity_pcnt = self._pcnt_from_V_humidity(
                smoothed_rel_humidity_V
            )
            assert isinstance(smoothed_rel_humidity_pcnt, float)

            return SmartPumpStatus(
                rel_humidity_V,
                rel_humidity_pcnt,
                smoothed_rel_humidity_pcnt,
                pump_status,
                status_time,
            )

    def clear_status_logs(self):
        self._rel_humidity_V_log.clear()
        self._smoothed_rel_humidity_V_log.clear()
        self._pump_status_log.clear()

    def get_status_since(
        self, earliest_epoch_time_s: ty.Optional[float]
    ) -> SmartPumpStatusHistory:
        """
        if earliest time is None all samples are returned
        """
        rel_humidity_V_epoch_time, rel_humidity_V = self._rel_humidity_V_log.get_values(
            earliest_epoch_time_s
        )

        rel_humidity_pcnt = self._pcnt_from_V_humidity(rel_humidity_V)
        assert isinstance(rel_humidity_pcnt, list)

        (
            smoothed_rel_humidity_V_epoch_time,
            smoothed_rel_humidity_V,
        ) = self._smoothed_rel_humidity_V_log.get_values(earliest_epoch_time_s)

        smoothed_rel_humidity_pcnt = self._pcnt_from_V_humidity(smoothed_rel_humidity_V)
        assert isinstance(smoothed_rel_humidity_pcnt, list)

        pump_running_epoch_time, pump_running = self._pump_status_log.get_values(
            earliest_epoch_time_s
        )

        return SmartPumpStatusHistory(
            rel_humidity_V=rel_humidity_V,
            rel_humidity_V_epoch_time=rel_humidity_V_epoch_time,
            rel_humidity_pcnt=rel_humidity_pcnt,
            rel_humidity_pcnt_epoch_time=rel_humidity_V_epoch_time,
            smoothed_rel_humidity_pcnt=smoothed_rel_humidity_pcnt,
            smoothed_rel_humidity_pcnt_epoch_time=smoothed_rel_humidity_V_epoch_time,
            pump_running_epoch_time=pump_running_epoch_time,
            pump_running=pump_running,
        )

    # Stops the feedback loop (so a join() should execute quickly)
    def interrupt(self):
        _LOGGER.info("Interrupting the pump thread")
        self._abort_running = True
        self._sleep_event.set()

    def _make_request_safe(
        self, request: Request
    ) -> ty.Tuple[bool, ty.Optional[Response]]:
        """Catch exceptions during request

        Args:
            request (Request): [description]

        Returns:
            bool: ok
        """

        task_desc = f"{request.instruction} on channel {self.channel}"

        assert self._device is not None

        try:
            response = self._device.make_request(request)
        except Exception as e:
            _LOGGER.error(
                f"Encountered exception {e} at {tb.format_exc()} \nwhile trying: {task_desc}"
            )
            return False, None

        if not response.success:
            _LOGGER.error(f"Failed to: {task_desc}")
            return False, None

        return True, response

    def _do_activate_closed_loop_pump(self):

        (
            _,
            current_humidity_V,
        ) = self._smoothed_rel_humidity_V_log.get_newest_value()
        current_humidity_pcnt = self._pcnt_from_V_humidity(current_humidity_V)

        _LOGGER.info(
            f"{self.channel}: Humidity: Current: {current_humidity_pcnt} %, Target: {self._settings.feedback_setpoint_pcnt} %"
        )

        if (
            isinstance(current_humidity_pcnt, float)
            and self._settings.feedback_setpoint_pcnt > current_humidity_pcnt
        ):
            _LOGGER.info(f"Activating pump for {self._settings.pump_on_time_s} s")

            turn_on_request = Request(
                channel=self.channel,
                instruction="turn_on",
                data=self._settings.pump_on_time_s,
            )

            ok, _ = self._make_request_safe(turn_on_request)
            if not ok:
                return

    def _do_run_loop(self):
        self._sleep_event.clear()
        self._sleep_event.wait(timeout=self._status_update_interval_s)
        ok = self._update_status()
        if not ok:
            return

        with self._settings_lock:

            next_update_time = datetime.now()

            should_activate = (
                self._last_feedback_update_time is not None
                and update_spans_activation_time(
                    self._last_feedback_update_time,
                    next_update_time,
                    self._settings.pump_activation_time,
                )
            )

            self._last_feedback_update_time = next_update_time

            if should_activate:
                _LOGGER.info(f"{self.channel}: Performing feedback event: ")
                self._do_activate_closed_loop_pump()

    def run(self):

        while not self._abort_running:
            self._do_run_loop()

        _LOGGER.info(f"Smart pump for channel: {self._channel} finished")
