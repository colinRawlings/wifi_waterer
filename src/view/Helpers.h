#pragma once

#include "../Types.h"

#include <string>

int RowLength();
int EndLblLength();

std::string FormatRow(std::string left_label, std::string center_label, std::string right_label);
