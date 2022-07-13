#include "arg-parse/arg-parse.h"

#include <gtest/gtest.h>

TEST(Parser, FindOption)
{
    const std::array argv{"argv_first", "argv_second"};
    const auto position =
        argparse::find_option("argv_second", argv.size(), argv.data());
    EXPECT_EQ(argv.data() + 1, position);
}

TEST(Parser, ParseInt)
{
    const std::array argv{"width", "128"};
    const auto parsed =
        argparse::Parser<int>::parse(argv.data(), argv.data() + argv.size());
    EXPECT_EQ(128, parsed);
}

TEST(Parser, ParseFloat)
{
    const std::array argv{"width", "128.5"};
    const auto parsed =
        argparse::Parser<float>::parse(argv.data(), argv.data() + argv.size());
    EXPECT_EQ(128.5, parsed);
}

TEST(Parser, ParseBoolNoValue)
{
    const std::array argv{"arg", "vsync", "128.5"};
    const auto parsed = argparse::Parser<bool>::parse(
        argv.data() + 1, argv.data() + argv.size());
    EXPECT_EQ(true, parsed);
}

TEST(Parser, ParseBoolWithValue)
{
    const std::array argv{"arg", "vsync", "false"};
    const auto parsed = argparse::Parser<bool>::parse(
        argv.data() + 1, argv.data() + argv.size());
    EXPECT_EQ(false, parsed);
}
