#ifndef ARG_PARSE_PARSER_H
#define ARG_PARSE_PARSER_H

#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace argparse
{
    class ArgParseException : public std::logic_error
    {
    public:
        ArgParseException(std::string_view arg_name, const char* message)
            : std::logic_error(message)
            , arg_name_(arg_name)
        {
        }

        [[nodiscard]] auto arg_name() const { return arg_name_; }

    private:
        std::string_view arg_name_;
    };

    class ArgumentNotFound : public ArgParseException
    {
    public:
        explicit ArgumentNotFound(std::string_view arg_name)
            : ArgParseException(arg_name, "Argument could not be found")
        {
        }
    };

    class NoArgumentValue : public ArgParseException
    {
    public:
        explicit NoArgumentValue(std::string_view arg_name)
            : ArgParseException(arg_name, "No value for argument was given")
        {
        }
    };

    constexpr const char* const* find_option(std::string_view name, int argc,
                                             const char* const argv[])
    {
        auto begin = argv;
        auto end = argv + argc;
        auto current = begin;
        while (current != end) {
            if (name == *current)
                return current;
            ++current;
        }
        return nullptr;
    }

    using ArgIterator = const char* const*;

    template<typename T>
    struct Parser {
        static T parse(ArgIterator, ArgIterator) {}
    };

    template<>
    struct Parser<int> {
        static int parse(ArgIterator begin, ArgIterator end)
        {
            if (begin + 1 < end) {
                auto val = std::stol(begin[1]);
                if (val > std::numeric_limits<int>::max() ||
                    val < std::numeric_limits<int>::min())
                    throw std::range_error("Argument is larger or less "
                                           "than int::max or int::min");
                return static_cast<int>(val);
            }
            throw NoArgumentValue(begin[0]);
        }
    };

    template<>
    struct Parser<float> {
        static float parse(ArgIterator begin, ArgIterator end)
        {
            if (begin + 1 < end) {
                auto val = std::stof(begin[1]);
                if (val > std::numeric_limits<float>::max() ||
                    val < std::numeric_limits<float>::min())
                    throw std::range_error("Argument is larger or less "
                                           "than float::max or float::min");
                return static_cast<float>(val);
            }
            throw NoArgumentValue(begin[0]);
        }
    };

    template<>
    struct Parser<bool> {
        static bool parse(ArgIterator begin, ArgIterator end)
        {
            if (begin + 1 < end) {
                std::string_view val(begin[1]);
                if (val == "true" || val == "True")
                    return true;
                else if (val == "false" || val == "False")
                    return false;
            }
            return true;
        }
    };

    template<>
    struct Parser<std::string_view> {
        static std::string_view parse(ArgIterator begin, ArgIterator end)
        {
            if (begin + 1 < end)
                return begin[1];
            throw NoArgumentValue(begin[0]);
        }
    };

    template<>
    struct Parser<std::string> {
        static std::string parse(ArgIterator begin, ArgIterator end)
        {
            return std::string{Parser<std::string_view>::parse(begin, end)};
        }
    };
} // namespace argparse

#endif // ARG_PARSE_PARSER_H
