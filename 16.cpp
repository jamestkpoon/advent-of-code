#include "base.hpp"
#include <map>

struct Packet
{
    Packet(std::string& s)
    {
        ver  = binary_to_decimal(s.substr(0, 3));
        type = binary_to_decimal(s.substr(3, 3));
        s = s.substr(6);

        if(type == 4)
        {
            std::string binary_value_ = "";
            while(!s.empty())
            {
                bool eov_ = (s[0] == '0');
                binary_value_ += s.substr(1,4);
                s = s.substr(5);

                if(eov_) { break; }
            }

            ans = binary_to_decimal(binary_value_);
            ans_valid = true;
        }
        else
        {
            ans_valid = false;
        }

        children.clear();
    }

    int sum_vers()
    {
        int out = ver;
        for(auto& child : children) { out += child.sum_vers(); }

        return out;
    }

    uint64_t get_ans()
    {
        if(!ans_valid)
        {
            std::vector<uint64_t> op_values;
            for(auto& child : children)
            {
                op_values.push_back(child.get_ans());
            }

            ans = op(op_values);
            ans_valid = true;
        }

        return ans;
    }

    uint64_t op(const std::vector<uint64_t>& vs)
    {
        uint64_t out;

        switch(type)
        {
            case 0:
                out = 0;
                for(const auto v : vs) { out += v; }
                return out;
            case 1:
                out = 1;
                for(const auto v : vs) { out *= v; }
                return out;
            case 2:
                out = UINT64_MAX;
                for(const auto v : vs) { out = std::min(out, v); }
                return out;
            case 3:
                out = 0;
                for(const auto v : vs) { out = std::max(out, v); }
                return out;
            case 5:
                return (vs[0] >  vs[1]) ? 1 : 0;
            case 6:
                return (vs[0] <  vs[1]) ? 1 : 0;
            case 7:
                return (vs[0] == vs[1]) ? 1 : 0;
            default: return 0;
        }
    }

    int ver, type;
    uint64_t ans; bool ans_valid;
    std::vector<Packet> children;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            auto s = hex_to_binary(input_strings_->front());
            top_packet_ = new Packet(read(s));
        }

    private:
        Packet read(std::string& s)
        {
            Packet p(s);

            if(p.type != 4)
            {
                if(s[0] == '0')
                {
                    int n_sp_b = binary_to_decimal(s.substr(1, 15));
                    auto ss = s.substr(16, n_sp_b);
                    while(ss.size() > 10) { p.children.push_back(read(ss)); }
                   
                    s = s.substr(16 + n_sp_b);
                }
                else
                {
                    int n_sp = binary_to_decimal(s.substr(1, 11));
                    s = s.substr(12);
                    for(int i=0; i<n_sp; i++) { p.children.push_back(read(s)); }
                }
            }

            return p;
        }

        std::string PartOne()
        {
            auto out = top_packet_->sum_vers();
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto out = top_packet_->get_ans();
            return std::to_string(out);
        }

        Packet *top_packet_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}