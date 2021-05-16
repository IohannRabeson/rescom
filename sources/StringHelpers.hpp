#ifndef RESCOM_STRINGHELPERS_HPP
#define RESCOM_STRINGHELPERS_HPP
#include <string>
#include <string_view>

void replaceAll(std::string& str, char toReplace, char replacement);
void toUpper(std::string& str);
std::string toUpper(std::string str);
std::string_view trim(std::string_view view);
std::string_view removeComment(std::string_view view, char const* oneLineCommentStart);

#endif //RESCOM_STRINGHELPERS_HPP
