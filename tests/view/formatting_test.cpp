#include <gtest/gtest.h>

#include "../../src/view/Helpers.h"

TEST(DisplaySuite, Formatting)
{
    ASSERT_EQ("+H|   Time   |+M", FormatRow("+H", "Time", "+M"));
    ASSERT_EQ("+ |   Time   | -", FormatRow("+", "Time", "-"));
    ASSERT_EQ("+ |   Tim    | -", FormatRow("+", "Tim", "-"));
    ASSERT_EQ("+ |0123456789| -", FormatRow("+", "0123456789", "-"));
    ASSERT_EQ("+ |0123456789| -", FormatRow("+", "01234567890", "-"));
    ASSERT_EQ("+H|   Time   |+M", FormatRow("+HH", "Time", "+MM"));
}
