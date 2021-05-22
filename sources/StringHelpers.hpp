#ifndef RESCOM_STRINGHELPERS_HPP
#define RESCOM_STRINGHELPERS_HPP
#include <string>
#include <string_view>
#include <vector>
#include <numeric>

void replaceAll(std::string& str, char toReplace, char replacement);
std::string toUpper(std::string str);
std::string_view trim(std::string_view view);
std::string_view removeComment(std::string_view view, char const* oneLineCommentStart);

namespace details
{
    template <typename T>
    using CleanedType = std::remove_cv_t<std::remove_reference_t<T>>;

    template <typename T>
    struct ToString
    {
        static std::string toString(T const& value)
        {
            using ValueType = std::remove_all_extents_t<CleanedType<T>>;
            static constexpr bool const IsValueTypeArray = std::is_array_v<CleanedType<T>>;
            static constexpr bool const IsChar = std::is_same_v<std::string::value_type, ValueType>;
            static constexpr bool const IsString = std::is_same_v<std::string, ValueType> || std::is_same_v<std::string_view, ValueType>;

            if constexpr ((IsValueTypeArray && IsChar) || IsString)
                return std::string(value);
            else
                return std::to_string(value);
        }
    };
}

template <typename ... A>
std::string format(std::string_view const& fmt, A&& ... args)
{
    std::initializer_list<std::string> argumentStrings = {details::ToString<A>::toString(std::forward<A>(args))...};
    std::string result;

    auto argumentIt = argumentStrings.begin();
    auto i = 0u;
    bool braceOpen = false;
    auto const expectedSize = fmt.size() + std::accumulate(argumentStrings.begin(), argumentStrings.end(), 0u,
                                                           [](unsigned int c, auto const& str)
                                                           {
                                                                return c + str.size();
                                                           });
    result.reserve(expectedSize);

    while (i < fmt.size())
    {
        auto current = fmt[i];

        if (current == '{' && !braceOpen)
        {
            braceOpen = true;
        }
        else if (current == '}' && braceOpen)
        {
            if (argumentIt != argumentStrings.end())
                result.append(*argumentIt++);
            braceOpen = false;
        }
        else
        {
            if (braceOpen)
            {
                result.push_back('{');
                braceOpen = false;
            }
            result.push_back(current);
        }
        ++i;
    }

    if (braceOpen)
    {
        result.push_back('{');
    }

    return result;
}

#endif //RESCOM_STRINGHELPERS_HPP
