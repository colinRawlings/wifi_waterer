#!python3

###############################################################
# Import
###############################################################

import asyncio
import logging
import pathlib as pt
from typing import List, Optional, Union
import aiohttp

import numpy as np
import waterer_backend.config as cfg
import waterer_backend.models as mdl
from waterer_backend.WiFi.find_smart_pumps import find_smart_pump_ips
from waterer_backend.WiFi.wifi_smart_pump import WiFiSmartPump

###############################################################
# Definitions
###############################################################

logger = logging.getLogger(__name__)

SCAN_DURATION_S = 5

###############################################################
# Class
###############################################################

pump_manager_settings_type = Union[List[mdl.SmartPumpSettings], mdl.SmartPumpSettings]


class WiFiPumpManager:
    def __init__(
        self,
        client: aiohttp.ClientSession,
        status_update_interval_s: int = 30,
        ips: List[str] = [],
        allow_load_history: bool = True,
    ) -> None:

        self._client = client
        self._status_update_interval_s = status_update_interval_s
        self._ips = ips

        self._allow_load_history = allow_load_history

        #

        self._pumps: List[WiFiSmartPump] = []
        for channel, ip in enumerate(ips):
            self._pumps.append(
                WiFiSmartPump(
                    channel=channel,
                    client=self._client,
                    ip=ip,
                    status_update_interval_s=self._status_update_interval_s,
                    allow_load_history=self._allow_load_history,
                )
            )

    @property
    def device_info(self) -> List[str]:
        info = []
        for idx, pump in enumerate(self._pumps):
            info.append(f"pump {idx}: {pump.info}")
        return info

    @property
    def num_pumps(self) -> int:
        return len(self._pumps)

    def _check_channel(self, channel: int) -> None:
        if channel < 0:
            raise ValueError(f"Channel ({channel}) cannot be negative")

        if channel >= self.num_pumps:
            raise ValueError(
                f"Channel ({channel}) cannot be greater than or equal to {self.num_pumps}"
            )

    async def turn_on(self, channel: int, duration_ms: int = 0) -> None:
        self._check_channel(channel)
        await self._pumps[channel].turn_on(duration_ms=duration_ms)

    async def turn_off(self, channel: int) -> None:
        self._check_channel(channel)
        await self._pumps[channel].turn_off()

    async def set_settings(self, channel: int, settings: mdl.SmartPumpSettings) -> None:
        self._check_channel(channel)
        await self._pumps[channel].set_settings(settings)
        await self.save_settings()

    async def get_settings(self, channel: int) -> mdl.SmartPumpSettings:
        self._check_channel(channel)
        return await self._pumps[channel].settings()

    async def save_settings(self) -> str:

        for pump in self._pumps:
            await pump.save_settings()

        return str(cfg.get_user_config_filepath())

    def save_history(self) -> str:

        for pump in self._pumps:
            pump.save_history()

        return str(cfg.get_pump_history_filepath())

    async def get_status(self, channel: int) -> mdl.SmartPumpStatus:
        self._check_channel(channel)
        return await self._pumps[channel].status

    def clear_status_logs(self, channel: int) -> None:
        self._check_channel(channel)
        return self._pumps[channel].clear_status_logs()

    def get_status_since(
        self, channel: int, earliest_epoch_time_s: Optional[float]
    ) -> mdl.SmartPumpStatusHistory:
        self._check_channel(channel)
        return self._pumps[channel].get_status_since(earliest_epoch_time_s)

    async def start(self):
        for idx, pump in enumerate(self._pumps):
            logger.info(f"starting pump: {idx+1} / {len(self._pumps)}")
            await pump.start()

    async def interrupt(self):
        for pump in self._pumps:
            logger.info(f"interrupting: {pump.channel}")
            await pump.interrupt()

        self.save_history()


class PumpManagerContext:
    def __init__(
        self,
        status_update_interval_s: int = 10,
        allow_load_history: bool = False,
        scan_duration_s: float = SCAN_DURATION_S,
    ) -> None:

        self._status_update_interval_s = status_update_interval_s
        self._allow_load_history = allow_load_history
        self._scan_duration_s = scan_duration_s

        self._client: Optional[aiohttp.ClientSession] = None

        #

        self._pump_manager: Optional[WiFiPumpManager] = None

    async def __aenter__(self) -> WiFiPumpManager:
        char_logger = logging.getLogger("charset_normalizer")
        char_logger.setLevel(logging.WARNING)
        self._client = aiohttp.ClientSession(version=aiohttp.HttpVersion11)

        ips = await find_smart_pump_ips(self._client)

        if len(ips) == 0:
            await self._client.close()
            raise RuntimeError("No smart pumps detected on network")

        self._pump_manager = WiFiPumpManager(
            client=self._client,
            status_update_interval_s=self._status_update_interval_s,
            ips=ips,
            allow_load_history=self._allow_load_history,
        )

        return self._pump_manager

    async def __aexit__(self, exc_type, exc_value, exc_traceback):

        logger.info("Pump manager context shutting down ... ")
        if self._pump_manager:
            await self._pump_manager.interrupt()

        if self._client is not None:
            await self._client.close()

        logger.info("Pump manager completed shut down ... ")
