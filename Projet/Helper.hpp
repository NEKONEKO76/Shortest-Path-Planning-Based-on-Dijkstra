#pragma once
#include <string>
#include <regex>
#include <memory>

//Split string
std::vector<std::string> split_string_by_string(const std::string& s, const std::string& delim) {
	std::vector<std::string> ret;
	std::regex re(delim);
	std::sregex_token_iterator iter(s.begin(), s.end(), re, -1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter) {
		ret.push_back(*iter);
	}
	return ret;
}

//Remove leading and trailing spaces from string
//const std::string trim(const std::string& s)
//{
//	std::string::const_iterator iter1 = std::find_if(s.begin(), s.end(), [](char c) { return !std::isspace(c); });
//	std::string::const_iterator iter2 = std::find_if(s.rbegin(), s.rend(), [](char c) { return !std::isspace(c); }).base();
//
//	return iter1 < iter2 ? std::string(iter1, iter2) : std::string("");
//}


namespace unique {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
