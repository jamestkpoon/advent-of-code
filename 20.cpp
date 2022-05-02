#include "base.hpp"

std::vector<Coord2D> iterate(const std::vector<Coord2D>& lit_px,
    const bool oob_lit, const std::string& map)
{
    std::vector<Coord2D> out;

    auto check_px = [&out, lit_px,oob_lit,map](const Coord2D& px,
        const int r_min, const int r_max, const int c_min, const int c_max)
    {
        std::string s = "";
        for(int r=px.r-1; r<=px.r+1; r++)
        {
            for(int c=px.c-1; c<=px.c+1; c++)
            {
                bool ib = ((r >= r_min) && (r <= r_max) &&
                        (c >= c_min) && (c <= c_max)),
                    lit = ib ? Coord2D(r,c).in(lit_px) : oob_lit;
                
                s += lit ? '1' : '0';
            }
        }

        if(map[binary_to_decimal(s)] == '#') { out.push_back(px); }
    };

    int r_min = INT32_MAX, r_max = INT32_MIN,
        c_min = INT32_MAX, c_max = INT32_MIN;

    for(const auto& px : lit_px)
    {
        r_min = std::min(r_min, px.r); r_max = std::max(r_max, px.r);
        c_min = std::min(c_min, px.c); c_max = std::max(c_max, px.c);
    }

    for(int r=r_min-2; r<=r_max+2; r++)
    {
        for(int c=c_min-2; c<=c_max+2; c++)
        {
            check_px(Coord2D(r,c), r_min, r_max, c_min, c_max);
        }
    }

    return out;
}

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            lit_px_ = new std::vector<Coord2D>();
            for(int r=0, rr=2; rr<input_strings_->size(); r++, rr++)
            {
                const auto& s = (*input_strings_)[rr];
                for(int c=0; c<s.size(); c++)
                {
                    if(s[c] == '#') { lit_px_->push_back(Coord2D(r,c)); }
                }
            }
        }

    private:
        void iterate_n(const int n)
        {
            const auto& map = input_strings_->front();

            auto oob_lit_fn = [map](const int i)
            {
                if(map[0] == map[9]) { return (map[0] == '#'); }
                else if(map[0] == '#') { return ((i%2) != 0); }
                else { return false; }
            };

            for(int i=0; i<n; i++)
            {
                *lit_px_ = iterate(*lit_px_, oob_lit_fn(i), map);
            }
        }

        std::string PartOne()
        {
            iterate_n(2);
            int out = lit_px_->size();
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            iterate_n(50);
            int out = lit_px_->size();
            return std::to_string(out);
        }

        std::vector<Coord2D>* lit_px_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}
