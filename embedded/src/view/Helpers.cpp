#include "Helpers.h"

#include <vector>

static const char kSpinner[5] = "-/|";

int RowLength() { return 16; }
int MaxEndLblLength() { return 4; }

std::string FormatRow(std::string left_label, std::string center_label, std::string right_label)
{
    auto trim_func = [](std::string & label, int max_length) {
        if (static_cast<int>(label.size()) > max_length)
        {
            label = label.substr(0, max_length);
        }
    };

    int end_label_length = std::min(std::min(static_cast<int>(left_label.size()),
                                             static_cast<int>(right_label.size())),
                                    MaxEndLblLength());

    auto center_label_length = RowLength() - 2 - 2 * end_label_length;

    trim_func(left_label, end_label_length);
    trim_func(right_label, end_label_length);
    trim_func(center_label, center_label_length);

    //

    if (static_cast<int>(left_label.size()) < end_label_length)
    {
        std::string padding(end_label_length - left_label.size(), ' ');
        left_label.insert(left_label.end(), padding.begin(), padding.end());
    }

    if (static_cast<int>(right_label.size()) < end_label_length)
    {
        std::string padding(end_label_length - right_label.size(), ' ');
        right_label.insert(right_label.begin(), padding.begin(), padding.end());
    }

    if (static_cast<int>(center_label.size()) < center_label_length)
    {
        auto padding = center_label_length - center_label.size();
        std::string left_padding(padding / 2, ' ');
        std::string right_padding(padding / 2 + padding % 2, ' ');
        center_label = left_padding + center_label + right_padding;
    }

    //

    std::string separator;
    if (end_label_length == 1)
    {
        separator = "|";
    }
    else if (end_label_length > 1)
    {
        separator = " ";
    }

    return left_label + separator + center_label + separator + right_label;
}

std::string FormatHumidityV(float humidity_V, bool add_unit)
{
    char buffer[20];
    std::snprintf(buffer, 20, "%.3f", humidity_V);

    std::string msg(buffer);

    if (add_unit)
        msg += std::string("V");

    return msg;
}

std::string PumpIcon(bool pump_running)
{
    if (pump_running)
    {
        char icon = kSpinner[(millis() / 1000) % 3];
        return "[" + std::string(1, icon) + "]";
    }
    else
    {
        return "[-]";
    }
}
