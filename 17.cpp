#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            auto split = split_string(input_strings_->front(), '=');
            auto x_substr = split[1].substr(0, split[1].size()-3);
            auto x_vec = split_string_to_ints(x_substr, '.');
            auto y_vec = split_string_to_ints(split[2], '.');
            x_min_incl_ = x_vec[0]; x_max_incl_ = x_vec[1];
            y_min_incl_ = y_vec[0]; y_max_incl_ = y_vec[1];
        }

    private:
        int tri_area(const int l)
        {
            return (l * (l + 1)) / 2;
        }

        std::string PartOne()
        {
            int out = tri_area(-y_min_incl_ - 1);
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto intercepts = [this](const int _ux, const int _uy)
            {
                int ux = _ux, uy = _uy, x = 0, y = 0;
                while(true)
                {
                    x += ux; y += uy; --uy;
                    if(--ux < 0) { ux = 0; }

                    if((x > x_max_incl_) || (y < y_min_incl_)) { return false; }
                    else
                    {
                        bool x_ok = ((x >= x_min_incl_) && (x <= x_max_incl_)),
                             y_ok = ((y >= y_min_incl_) && (y <= y_max_incl_));
                        if(x_ok && y_ok) { return true; }
                    }
                }
            };

            int out = 0, ux = 1;
            while(tri_area(ux) < x_min_incl_) { ++ux; }
            while(ux <= x_max_incl_)
            {
                for(int uy=y_min_incl_; uy<=100; uy++)
                {
                    if(intercepts(ux,uy)) { ++out; }
                }

                ++ux;
            }

            return std::to_string(out);
        }

        int x_min_incl_, x_max_incl_, y_min_incl_, y_max_incl_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}