#include "Helpers.h"

#include <vector>

int RowLength() { return 16; }
int EndLblLength() { return 2; }

std::string FormatRow(std::string left_label, std::string center_label, std::string right_label)
{
    auto trim_func = [](std::string & label, int max_length) {
        if (static_cast<int>(label.size()) > max_length)
        {
            label = label.substr(0, max_length);
        }
    };

    auto center_label_length = RowLength() - 2 - 2 * EndLblLength();

    trim_func(left_label, EndLblLength());
    trim_func(right_label, EndLblLength());
    trim_func(center_label, center_label_length);

    //

    if (static_cast<int>(left_label.size()) < EndLblLength())
    {
        std::string padding(EndLblLength() - left_label.size(), ' ');
        left_label.insert(left_label.end(), padding.begin(), padding.end());
    }

    if (static_cast<int>(right_label.size()) < EndLblLength())
    {
        std::string padding(EndLblLength() - right_label.size(), ' ');
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

    return left_label + "|" + center_label + "|" + right_label;
}
