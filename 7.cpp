#include "base.hpp"
#include <cmath>

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {

        }

    private:
        int min(const std::vector<int>& v)
        {
            int out = INT32_MAX;
            for(const int i : v)
            {
                out = (i < out) ? i : out;
            }

            return out;
        }

        int max(const std::vector<int>& v)
        {
            int out = INT32_MIN;
            for(const int i : v)
            {
                out = (i > out) ? i : out;
            }

            return out;
        }

        int sum_fuel(const std::vector<int>& v, int (*dist_func)(int))
        {
            int out = INT32_MAX;
            for(int i=min(v); i<max(v); i++)
            {
                int sf = 0;
                for(const int vv : v)
                {
                    sf += dist_func(abs(vv - i));
                }

                out = (sf < out) ? sf : out;
            }

            return out;
        }

        std::string PartOne()
        {
            auto ints = split_string_to_ints((*input_strings_)[0], ',');
            int out = sum_fuel(ints, [](int i) { return i; });
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto ints = split_string_to_ints((*input_strings_)[0], ',');
            int out = sum_fuel(ints, [](int i) { return (i*(i+1))/2; });
            return std::to_string(out);
        }
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}