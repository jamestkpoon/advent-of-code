#include "base.hpp"

struct Move
{
    char dir;
    int magnitude;

    Move(const std::string& s)
    {
        auto split = split_string(s, ' ');
        dir = split[0].c_str()[0];
        magnitude = std::atoi(split[1].c_str());
    }

    int displacement() { return (dir == 'd') ? magnitude : -magnitude; }
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            moves_ = new std::vector<Move>();
            for(const auto& s : *input_strings_)
            {
                moves_->push_back(Move(s));
            }
        }

    private:
        std::string PartOne()
        {
            int x = 0, y = 0;
            for(auto& m : *moves_)
            {
                if(m.dir == 'f')
                {
                    x += m.magnitude;
                }
                else
                {
                    y += m.displacement();
                }
            }

            int out = x * y;
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            int x = 0, y = 0, aim = 0;
            for(auto& m : *moves_)
            {
                if(m.dir == 'f')
                {
                    x += m.magnitude;
                    y += aim * m.magnitude;
                }
                else
                {
                    aim += m.displacement();
                }
            }

            int out = x * y;
            return std::to_string(out);
        }

        std::vector<Move>* moves_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}