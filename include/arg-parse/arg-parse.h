#ifndef ARG_PARSE_ARG_PARSE_H
#define ARG_PARSE_ARG_PARSE_H

#include "parser.h"

#include <cstddef>
#include <optional>

namespace argparse
{
    class ArgParse
    {
    public:
        ArgParse(int argc, const char* const argv[])
            : name_(argv[0])
            , argc_(argc)
            , argv_(argv)
        {
        }

        const char* name() const { return name_; }
        std::size_t count() const { return argc_; }

        template<typename T, typename P = Parser<T>>
        T get_required(std::string_view name) const
        {
            if (auto begin = find_option(name, argc_, argv_))
                return P::parse(begin, argv_ + argc_);
            throw ArgumentNotFound(name);
        }

        template<typename T, typename P = Parser<T>>
        std::optional<T> get(std::string_view name) const
        {
            try {
                return get_required<T, P>(name);
            } catch (const ArgParseException& e) {
                return {};
            }
        }

        template<typename T, typename P = Parser<T>>
        T get(std::string_view name, const T& default_value) const
        {
            return get<T, P>(name).value_or(default_value);
        }

    private:
        const char* name_;
        std::size_t argc_ = 0;
        const char* const* argv_;
    };
} // namespace argparse

#endif // ARG_PARSE_ARG_PARSE_H
