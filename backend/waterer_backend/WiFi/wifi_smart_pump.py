# !python3

###############################################################
# Imports
###############################################################

import asyncio
import json
import logging
import typing as ty
from datetime import datetime
from time import time
from aiohttp import ClientSession
from pydantic import BaseModel
from aiohttp import client_exceptions

import waterer_backend.config as cfg
import waterer_backend.utils as ut
from waterer_backend.models import (
    ActivationTime,
    SmartPumpSettings,
    SmartPumpStatus,
    SmartPumpStatusData,
    SmartPumpStatusHistory,
)
from waterer_backend.status_log import BinaryStatusLog, FloatStatusLog

###############################################################
# Logging
###############################################################

_LOGGER = logging.getLogger(__name__)

_CLOCK_SYNC_HOUR = 2  # sync at 2am hack to work around shitty rtc in arduino


class Response(BaseModel):
    PumpStatus: bool
    Humidity: float
    FBHumidityV: float
    FBOnTimeHour: int
    FBPumpDurationS: int


###############################################################
# Functions
###############################################################


def get_current_hour() -> int:
    return datetime.now().hour


###############################################################
# Classes
###############################################################


class WiFiSmartPump:
    def __init__(
        self,
        channel: int,
        client: ClientSession,
        ip: ty.Optional[str],
        status_update_interval_s: float = 5,
        allow_load_history: bool = False,
        auto_save_interval_s: ty.Optional[int] = 3600,
    ) -> None:

        self._ip = ip
        if ip is None:
            _LOGGER.warning("Pump created without valid client")

        self._task: ty.Optional[asyncio.Task] = None

        self._channel = channel

        self._connected = True

        self._client = client

        self._settings: SmartPumpSettings = (
            self._load_settings()
        )  # n.b. device will have a chance to overwrite when it starts

        self._abort_running = False

        self._status_update_interval_s = status_update_interval_s
        self._auto_save_interval_s = auto_save_interval_s

        if allow_load_history:
            self.load_history()
        else:
            self._init_logs()

        self._last_update_hour: int = get_current_hour()
        self._last_auto_save_time: float = time()

        _LOGGER.info(
            f"Created smart pump for channel {self._channel}@{self._ip} with update interval: {self._status_update_interval_s}s"
        )

    ###############################################################

    def _init_logs(self):
        self._rel_humidity_V_log = FloatStatusLog()
        self._smoothed_rel_humidity_V_log = FloatStatusLog()
        self._pump_status_log = BinaryStatusLog()

    ###############################################################

    def _load_settings(self) -> SmartPumpSettings:

        assert self.address is not None

        user_config_path = cfg.get_user_config_filepath()
        default_config_path = cfg.get_default_config_filepath()
        if not default_config_path.is_file():
            raise RuntimeError(
                f"{self._channel}: No default config found at: {default_config_path}"
            )

        if user_config_path.is_file():

            _LOGGER.info(
                f"{self._channel}: Attempting to load user config: {user_config_path}"
            )

            with open(user_config_path, "r") as fh:
                user_config_dict = json.load(fh)

            if self.address in user_config_dict:
                _LOGGER.info(
                    f"{self._channel}: Loading entry for device {self.address} in user config, loading ..."
                )
                return SmartPumpSettings(**user_config_dict[self.address])

        _LOGGER.info(
            f"{self._channel}: Falling back to default user settings for device: {self.address}"
        )

        with open(default_config_path, "r") as fh:
            default_config_dict = json.load(fh)

        return SmartPumpSettings(**default_config_dict)

    ###############################################################

    async def save_settings(self) -> str:

        # save to disk (e.g. Name, Humidity etc)

        config_filepath = cfg.get_user_config_filepath()

        if config_filepath.is_file():
            with open(config_filepath, "r") as fh:
                current_configs = json.load(fh)
        else:
            current_configs: ty.Dict[str, ty.Dict] = dict()

        current_configs[self.address] = self._settings.dict()

        with open(config_filepath, "w") as fh:
            json.dump(current_configs, fh)

        # save to device

        if (await self._transact_with_client(f"http://{self._ip}/save")) is None:
            return "save failed"

        return f"Saved to: {self.channel}"

    ###############################################################

    def save_history(self) -> str:

        pump_history_filepath = cfg.get_pump_history_filepath()
        if pump_history_filepath.is_file():

            with open(pump_history_filepath, "r") as fh:
                pump_history = json.load(fh)
        else:
            pump_history = {}

        pump_history[self.address] = self.history.dict()

        pump_history_filepath.parent.mkdir(exist_ok=True, parents=True)

        with open(pump_history_filepath, "w") as fh:
            json.dump(pump_history, fh)

        _LOGGER.debug(f"{self._channel}: Saved history to: {pump_history_filepath}")

        return str(cfg.get_pump_history_filepath())

    ###############################################################

    @property
    def info(self) -> str:
        assert self._ip is not None
        return f"{self._ip}"

    ###############################################################

    @property
    def channel(self) -> int:
        return self._channel

    ###############################################################

    @property
    def address(self) -> str:
        assert self._ip and self._ip is not None
        return self._ip

    ###############################################################

    @property
    def connected(self) -> bool:
        return self._connected

    ###############################################################

    async def settings(self) -> SmartPumpSettings:

        await self._update_device_status()

        return self._settings

    ###############################################################

    async def _transact_with_client(self, url: str) -> ty.Optional[str]:

        success = False
        try:
            async with self._client.get(url) as resp:
                txt_response = await resp.text()
                success = True
        except client_exceptions.ClientConnectorError as e:
            txt_response = None
            success = False

        if not success and self._connected:
            _LOGGER.warning(f"Lost connection to: {self.address}")
        elif success and not self._connected:
            _LOGGER.info(f"Regained connection to: {self.address}")

        self._connected = success

        return txt_response

    ###############################################################

    async def send_settings(self) -> None:

        #

        s = self._settings

        embedded_settings = dict(
            pump_on_time_s=s.pump_on_time_s,
            fb_hour=s.pump_activation_time.hour,
            fb_humidity_mv=1000
            * ut.humidity_v_from_pcnt(
                s.dry_humidity_V, s.wet_humidity_V, s.feedback_setpoint_pcnt
            ),
            current_hour=datetime.now().hour,
            current_minute=datetime.now().minute,
        )

        for key, value in embedded_settings.items():
            _ = await self._transact_with_client(
                f"http://{self._ip}/{int(value)}/{key}"
            )

    ###############################################################

    async def set_settings(self, value: SmartPumpSettings) -> None:
        self._settings = value
        _LOGGER.info(f"{self._channel}: new settings")
        await self.send_settings()

    ###############################################################

    @property
    def history(self) -> SmartPumpStatusData:
        return SmartPumpStatusData(
            rel_humidity_V_log=self._rel_humidity_V_log.to_data(),
            smoothed_rel_humidity_V_log=self._smoothed_rel_humidity_V_log.to_data(),
            pump_status_log=self._pump_status_log.to_data(),
        )

    ###############################################################

    def load_history(self) -> None:
        filepath = cfg.get_pump_history_filepath()

        if not filepath.is_file():
            _LOGGER.info(
                f"{self._channel}: Failed to find history file for: {filepath}"
            )
            self._init_logs()
            return

        with open(filepath, "r") as fh:
            history_dict = json.load(fh)

        if self.address not in history_dict:
            _LOGGER.info(
                f"{self._channel}: Failed to find entry in history for: {self.address}"
            )
            self._init_logs()
            return

        try:
            history = SmartPumpStatusData(**history_dict[self.address])
        except Exception as e:
            _LOGGER.error(
                f"{self._channel}: Failed to parse history file: {filepath} with exception{e}"
            )
            self._init_logs()
            return

        _LOGGER.info(f"{self._channel}: Loaded history from {filepath}")

        self._rel_humidity_V_log = FloatStatusLog.from_data(history.rel_humidity_V_log)
        self._smoothed_rel_humidity_V_log = FloatStatusLog.from_data(
            history.smoothed_rel_humidity_V_log
        )
        self._pump_status_log = BinaryStatusLog.from_data(history.pump_status_log)

    ###############################################################

    def _pcnt_from_V_humidity(self, rel_humidity_V: ut.HumidityT) -> ut.HumidityT:

        return ut.humidity_pcnt_from_v(
            self._settings.dry_humidity_V, self._settings.wet_humidity_V, rel_humidity_V
        )

    ###############################################################

    def _smoothed_humidity(self, rel_humidity_V: float) -> ty.Optional[float]:
        alpha = 1.0 / self._settings.num_smoothing_samples
        _, last_value = self._smoothed_rel_humidity_V_log.get_newest_value()

        if last_value is None:
            return rel_humidity_V

        return alpha * rel_humidity_V + (1 - alpha) * last_value

    ###############################################################

    async def turn_on(self):

        await self._transact_with_client(f"http://{self._ip}/on")

    ###############################################################

    async def turn_off(self):
        await self._transact_with_client(f"http://{self._ip}/off")

    ###############################################################
    # Access Data
    ###############################################################

    async def _update_device_status(self) -> ty.Optional[ty.Tuple[float, bool]]:

        txt = await self._transact_with_client(f"http://{self._ip}/")
        if txt is None:
            return None

        response = Response(**json.loads(txt))

        rel_humidity_V = response.Humidity
        pump_status = response.PumpStatus

        s = self._settings
        s.pump_activation_time = ActivationTime(hour=response.FBOnTimeHour, minute=0)
        val = self._pcnt_from_V_humidity(response.FBHumidityV)
        assert isinstance(val, float)
        s.feedback_setpoint_pcnt = val
        s.pump_on_time_s = response.FBPumpDurationS

        return rel_humidity_V, pump_status

    ###############################################################

    async def _update_status(self) -> bool:

        result = await self._update_device_status()
        if result is None:
            return False

        rel_humidity_V, pump_status = result

        smoothed_rel_humidity_V = self._smoothed_humidity(rel_humidity_V)

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

    ###############################################################

    @property
    async def status(self) -> SmartPumpStatus:

        _ = await self._update_status()

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

        smoothed_rel_humidity_pcnt = self._pcnt_from_V_humidity(smoothed_rel_humidity_V)  # type: ignore
        assert isinstance(smoothed_rel_humidity_pcnt, float)

        return SmartPumpStatus(
            rel_humidity_V=rel_humidity_V,
            rel_humidity_pcnt=rel_humidity_pcnt,
            smoothed_rel_humidity_pcnt=smoothed_rel_humidity_pcnt,
            pump_running=pump_status,
            epoch_time=status_time,
            is_connected=self.connected,
        )

    ###############################################################

    def clear_status_logs(self):
        self._rel_humidity_V_log.clear()
        self._smoothed_rel_humidity_V_log.clear()
        self._pump_status_log.clear()

    ###############################################################

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
            is_connected=self.connected,
        )

    ###############################################################

    async def _update_arduino_clock(self):
        this_update_hour = get_current_hour()

        if (
            self._last_update_hour != this_update_hour
            and this_update_hour == _CLOCK_SYNC_HOUR
        ):
            _LOGGER.info(f"Re-sending the time to keep the arduino in sync")
            await self.send_settings()

        self._last_update_hour = this_update_hour

    ###############################################################

    async def _do_loop_iteration(self):

        await self._update_status()

        await self._update_arduino_clock()

    ###############################################################

    async def run(self):

        await self._update_device_status()
        await self.send_settings()

        while not self._abort_running:
            try:
                await self._do_loop_iteration()
                await asyncio.sleep(self._status_update_interval_s)
            except asyncio.CancelledError:
                _LOGGER.info(f"{self._channel}: run cancelled, stopping ...")
                break
            except Exception as e:
                _LOGGER.error(f"{self.channel}: Encountered exception in run loop: {e}")

        _LOGGER.info(f"{self.channel}: run() finished")

    ###############################################################

    async def start(self):
        self._task = asyncio.get_event_loop().create_task(self.run())

    ###############################################################

    async def interrupt(self):

        if self._task is not None and not self._task.done():
            self._task.cancel()
            await self._task
