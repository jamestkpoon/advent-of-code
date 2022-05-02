#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {

        }

    private:
        void log_spawn_increment(int first_spawn_day, int days, int stride, int spawn_delay)
        {
            for(int d=first_spawn_day; d<days; d+=stride)
            {
                ++(*num_spawns_)[d];

                int child_fsd = d + stride + spawn_delay;
                log_spawn_increment(child_fsd, days, stride, spawn_delay);
            }
        }

        uint64_t count_pop(int days, int stride, int spawn_delay)
        {
            num_spawns_ = new std::vector<uint64_t>(days, 0);
            log_spawn_increment(1, days, stride, spawn_delay);

            auto delays_ = split_string_to_ints((*input_strings_)[0], ',');
            uint64_t pop = delays_.size();
            for(const int delay : delays_)
            {
                for(int d=1; d<days-delay; d++)
                {
                    pop += (*num_spawns_)[d];
                }
            }

            return pop;
        }

        std::string PartOne()
        {
            return std::to_string(count_pop(1+80, 7, 2));
        }

        std::string PartTwo()
        {
            return std::to_string(count_pop(1+256, 7, 2));
        }

        std::vector<uint64_t>* num_spawns_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}