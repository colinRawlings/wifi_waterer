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


def get_history_dir() -> pt.Path:

    return pt.Path(rc.resource_filename("waterer_backend", "history"))


def get_pump_history_filepath() -> pt.Path:

    return get_history_dir() / "pump_history.json"
