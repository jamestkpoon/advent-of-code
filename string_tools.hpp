#ifndef STRING_TOOLS_HPP
#define STRING_TOOLS_HPP

#include <string>
#include <vector>
#include <cmath>
#include <map>



std::vector<std::string> split_string(const std::string& s, const char sep)
{
    std::vector<std::string> out;
    std::string substring = "";
    for(const char& c : s + sep)
    {
        if(c != sep) substring += c;
        else if(!substring.empty())
        {
            out.push_back(substring);
            substring.clear();
        }
    }

    return out;
}

std::vector<int> split_string_to_ints(const std::string& s, const char sep)
{
    std::vector<int> out;
    for(const auto& ss : split_string(s, sep))
    {
        out.push_back(std::atoi(ss.c_str()));
    }

    return out;
}



uint64_t binary_to_decimal(const std::string& s, const char high_char='1')
{
    uint64_t out = 0;
    for(int i=0, ri=s.size()-1; i<s.size(); i++, ri--)
    {
        if(s[ri] == high_char) { out += std::pow((uint64_t)2, i); }
    }

    return out;
}

std::string hex_to_binary(const std::string& s)
{
    std::map<char,std::string> hex_to_binary_map;
    hex_to_binary_map['0'] = "0000";
    hex_to_binary_map['1'] = "0001";
    hex_to_binary_map['2'] = "0010";
    hex_to_binary_map['3'] = "0011";
    hex_to_binary_map['4'] = "0100";
    hex_to_binary_map['5'] = "0101";
    hex_to_binary_map['6'] = "0110";
    hex_to_binary_map['7'] = "0111";
    hex_to_binary_map['8'] = "1000";
    hex_to_binary_map['9'] = "1001";
    hex_to_binary_map['A'] = "1010";
    hex_to_binary_map['B'] = "1011";
    hex_to_binary_map['C'] = "1100";
    hex_to_binary_map['D'] = "1101";
    hex_to_binary_map['E'] = "1110";
    hex_to_binary_map['F'] = "1111";

    std::string out = "";
    for(const char& c : s) { out += hex_to_binary_map[c]; }

    return out;
}



bool contains(const std::string& s, const char c)
{
    return (s.find(c) != std::string::npos);
}

std::string set_intersection(const std::vector<std::string>& ss)
{
    std::string out = "";
    for(const char c : ss[0])
    {
        bool ok = true;
        for(int i=1; i<ss.size(); i++)        
        {
            if(!contains(ss[i], c))
            {
                ok = false;
                break;
            }
        }

        if(ok)
        {
            out += c;
        }
    }

    return out;
}

#endif