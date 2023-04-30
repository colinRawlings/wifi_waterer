# !python3

###############################################################
# Imports
###############################################################

import json
import logging
import pathlib as pt
import typing as ty

import pkg_resources as rc
from waterer_backend.models import SmartPumpSettings

###############################################################
# Logging
###############################################################

_LOGGER = logging.getLogger(__name__)


###############################################################
# Functions
###############################################################


def get_config_dir() -> pt.Path:

    return pt.Path(rc.resource_filename("waterer_backend", "config"))


def get_history_dir() -> pt.Path:

    return pt.Path(rc.resource_filename("waterer_backend", "history"))


def get_pump_history_filepath() -> pt.Path:

    return get_history_dir() / "pump_history.json"


def get_user_config_filepath() -> pt.Path:

    return get_config_dir() / "user_pump_config.json"


def get_default_config_filepath() -> pt.Path:

    return get_config_dir() / "default_pump_config.json"
