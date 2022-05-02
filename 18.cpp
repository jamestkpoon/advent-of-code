#include "base.hpp"

struct StrWithIdx
{
    StrWithIdx() { s = ""; }

    StrWithIdx(const std::string& _s, const size_t _pos)
    {
        s = _s; pos = _pos;
    }

    void replace(std::string& _s, const std::string& new_content)
    {
        _s.erase(pos, s.size());
        _s.insert(pos, new_content);
    }

    void split_pair(long& a, long& b)
    {
        auto spl = split_string(s, ',');
        a = std::atol(spl[0].substr(1).c_str());
        b = std::atol(spl[1].substr(0,spl[1].size()-1).c_str());
    }

    std::string s;
    size_t pos;
};

std::vector<StrWithIdx> find_regulars(const std::string& _s)
{
    std::vector<StrWithIdx> out;   
    int rI = -1; std::string s = "";
    while(++rI < _s.size())
    {
        bool is_digit = ((_s[rI] != ',') &&
            (_s[rI] != '[') && (_s[rI] != ']'));

        if(is_digit) { s += _s[rI]; }
        else if(!s.empty())
        {
            out.push_back(StrWithIdx(s, rI-s.size()));
            s = "";
        }
    }

    return out;
}

std::vector<StrWithIdx> find_pairs(const std::string& s)
{
    std::vector<StrWithIdx> out;
    auto regular_indices = find_regulars(s);
    int rI = -1;
    while(++rI < s.size())
    {
        if(s[rI] == '[')
        {
            for(int i=0; i<regular_indices.size()-1; i++)
            {
                StrWithIdx& a = regular_indices[i],
                    b = regular_indices[i+1];
                size_t comma_pos_ = a.pos + a.s.size(),
                    b_pos_ = comma_pos_ + 1,
                    cb_pos_ = b_pos_ + b.s.size();
                if(((rI+1) == a.pos) && (s[comma_pos_] == ',') &&
                    (b.pos == b_pos_) && (s[cb_pos_] == ']'))
                {
                    auto ss = s.substr(rI, cb_pos_+1 - rI);
                    out.push_back(StrWithIdx(ss, rI));
                }
            }
        }
    }

    return out;
}

bool find_first_explode_candidate(StrWithIdx& swi, const std::string& s)
{
    for(const auto& pair : find_pairs(s))
    {
        int br_sum = 0;
        for(const char c : s.substr(0, pair.pos+1))
        {
            if((c == '[') && (++br_sum > 4))
            {
                swi.s = pair.s; swi.pos = pair.pos;
                return true;
            }
            else if(c == ']') { --br_sum; }
        }
    }

    return false;
}

bool explode(std::string& s)
{
    StrWithIdx swi;
    if(!find_first_explode_candidate(swi, s)) { return false; }
 
    long a_incr, b_incr; swi.split_pair(a_incr, b_incr);
    
    s.erase(swi.pos, swi.s.size());
    auto regular_indices = find_regulars(s);
    
    int b_idx = 0;
    while((b_idx < regular_indices.size()) &&
        (regular_indices[b_idx].pos < swi.pos))
    {
        ++b_idx;
    }
    if(b_idx < regular_indices.size())
    {
        auto& b = regular_indices[b_idx];
        int new_value = std::atol(b.s.c_str()) + b_incr;
        b.replace(s, std::to_string(new_value));
    }

    int a_idx = regular_indices.size() - 1;
    while((a_idx >= 0) && (regular_indices[a_idx].pos > swi.pos))
    {
        --a_idx;
    }
    if(a_idx >= 0)
    {
        auto& a = regular_indices[a_idx];
        auto old_str_size = a.s.size();
        int new_value = std::atol(a.s.c_str()) + a_incr;
        std::string new_str = std::to_string(new_value);
        a.replace(s, new_str);

        swi.pos += new_str.size() - old_str_size;
    }

    s.insert(swi.pos, "0");

    return true;
}

bool split(std::string& s)
{
    for(auto& swi : find_regulars(s))
    {
        auto x = std::atol(swi.s.c_str());
        if(x > 9)
        {
            auto x_half = x / 2;
            std::string new_str = '[' +
                std::to_string(x_half) + ',' +
                std::to_string(x-x_half) + ']';
            
            swi.replace(s, new_str);

            return true;
        }
    }

    return false;
}

void reduce(std::string& s)
{
    while(explode(s) || split(s)) { continue; }
}

std::string add(const std::string& a, const std::string& b)
{
    std::string sum = '[' + a + ',' + b + ']';
    return sum;
}

void to_magnitude(std::string& s)
{
    while(true)
    {
        auto pairs = find_pairs(s);
        if(pairs.empty()) { break; }

        for(int i=pairs.size()-1; i>=0; i--)
        {
            long a, b; pairs[i].split_pair(a, b);
            auto new_value = 3*a + 2*b;
            pairs[i].replace(s, std::to_string(new_value));
        }
    }
}

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            
        }

    private:
        std::string PartOne()
        {
            std::string s = input_strings_->front();

            for(int i=1; i<input_strings_->size(); i++)
            {
                s = add(s, (*input_strings_)[i]);
                reduce(s);
            }

            to_magnitude(s);

            return s;
        }

        std::string PartTwo()
        {
            long out = 0;
            for(int aI=0; aI<input_strings_->size(); aI++)
            {
                const auto& a = (*input_strings_)[aI];

                for(int bI=0; bI<input_strings_->size(); bI++)
                {
                    if(aI == bI) { continue; }

                    auto s = add(a, (*input_strings_)[bI]);
                    reduce(s); to_magnitude(s);
                    out = std::max(out, std::atol(s.c_str()));
                }
            }

            return std::to_string(out);
        }
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}