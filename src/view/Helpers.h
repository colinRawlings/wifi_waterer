#pragma once

#include "../Types.h"

#include <string>

int RowLength();
int MaxEndLblLength();

std::string FormatRow(std::string left_label, std::string center_label, std::string right_label);

std::string FormatHumidityV(float humidity_V);

std::string PumpIcon(bool pump_running);
