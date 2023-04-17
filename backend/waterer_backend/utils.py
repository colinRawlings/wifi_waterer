#!python3

###############################################################
# Imports
###############################################################

import logging
from datetime import datetime, time
from typing import TypeVar, Generic, List, Optional
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


###############################################################


def time_today_from_update_time(the_time: datetime) -> datetime:

    dt_today = datetime.now().date()

    return datetime.combine(dt_today, the_time.time())


###############################################################


def update_spans_activation_time(
    last_update_dt: datetime,
    current_dt: datetime,
    update_time: datetime,
    perform_hourly: bool,
) -> bool:

    if perform_hourly:

        current_hour = current_dt.hour
        last_update_hour = last_update_dt.hour

        result = current_hour > last_update_hour

        logger.debug(
            f"Feedback set to perform hourly: current_hour: {current_hour}, last_update_hour: {last_update_hour}, update_spans: {result}"
        )
        return result

    else:

        assert last_update_dt < current_dt

        update_dt_today = time_today_from_update_time(update_time)

        return last_update_dt < update_dt_today and update_dt_today <= current_dt


###############################################################


def day_of_the_year() -> int:
    return datetime.now().timetuple().tm_yday
