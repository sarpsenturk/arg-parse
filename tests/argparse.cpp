#include "arg-parse/arg-parse.h"

#include <array>
#include <gtest/gtest.h>

TEST(ArgParse, Constructor)
{
    const std::array argv{"argv_first", "argv_second"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    EXPECT_STREQ(argv[0], arg_parse.name());
    EXPECT_EQ(argv.size(), arg_parse.count());
}

TEST(ArgParse, GetOptional)
{
    const std::array argv{"argv_first", "int_option", "12"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    auto optional_value = arg_parse.get<int>("int_option");
    EXPECT_EQ(12, optional_value.value());
}

TEST(ArgParse, GetOptionalEmpty)
{
    const std::array argv{"argv_first", "int_option"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    auto optional_value = arg_parse.get<int>("int_option");
    EXPECT_FALSE(optional_value.has_value());
}

TEST(ArgParse, GetOptionalEmptyRequired)
{
    const std::array argv{"argv_first", "int_option"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    EXPECT_THROW(arg_parse.get_required<int>("int_option"),
                 argparse::NoArgumentValue);
}

TEST(ArgParse, GetWithDefaultWithValue)
{
    const std::array argv{"argv_first", "bool_option"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    auto optional_value = arg_parse.get<bool>("bool_option", false);
    EXPECT_TRUE(optional_value);
}

TEST(ArgParse, GetWithDefaultWithoutValue)
{
    const std::array argv{"argv_first"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    auto optional_value = arg_parse.get<bool>("bool_option", false);
    EXPECT_FALSE(optional_value);
}

struct Vector2 {
    float x;
    float y;
};

template<>
struct argparse::Parser<Vector2> {
    static Vector2 parse(argparse::ArgIterator begin, argparse::ArgIterator end)
    {
        float x = Parser<float>::parse(begin++, end);
        float y = Parser<float>::parse(begin, end);
        return {x, y};
    }
};

TEST(ArgParse, GetUserDefinedType)
{
    const std::array argv{"vector", "15", "50"};
    const argparse::ArgParse arg_parse(argv.size(), argv.data());
    auto vector = arg_parse.get_required<Vector2>("vector");
    EXPECT_EQ(15, vector.x);
    EXPECT_EQ(50, vector.y);
}
