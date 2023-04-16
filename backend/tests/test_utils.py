#!python3


###############################################################
# Imports
###############################################################

import numpy as np

import waterer_backend.utils as ut


###############################################################
# Tests
###############################################################


def test_roundtrip_conversion():

    DRY_V = 2
    WET_V = 1

    REL_V = 1.2  # wettish

    rel_pcnt = ut.humidity_pcnt_from_v(DRY_V, WET_V, REL_V)
    assert rel_pcnt > 50 and rel_pcnt < 100

    assert np.isclose(REL_V, ut.humidity_v_from_pcnt(DRY_V, WET_V, rel_pcnt))
