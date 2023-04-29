#!python3

###############################################################
# Imports
###############################################################

import logging
from datetime import datetime, time
from typing import TypeVar, List, Optional
import numpy as np

###############################################################
# Definitions
###############################################################

logger = logging.getLogger(__name__)

HumidityT = TypeVar("HumidityT", None, float, List[Optional[float]])

###############################################################
# Functions
###############################################################


def humidity_v_from_pcnt(dry_V: float, wet_V: float, humidity_pcnt: float) -> float:
    return dry_V - humidity_pcnt / 100 * (dry_V - wet_V)


###############################################################


def humidity_pcnt_from_v(
    dry_V: float, wet_V: float, rel_humidity_V: HumidityT
) -> HumidityT:

    if rel_humidity_V is None:
        return rel_humidity_V

    if isinstance(rel_humidity_V, list):

        arr = np.asarray(rel_humidity_V)
        none_vals = arr == None
        arr[none_vals] = 0

        scaled_arr = (dry_V - arr) / (dry_V - wet_V) * 100

        scaled_optional_arr = scaled_arr
        scaled_optional_arr[none_vals] = None
        return scaled_optional_arr.tolist()

    else:
        return (dry_V - rel_humidity_V) / (dry_V - wet_V) * 100
