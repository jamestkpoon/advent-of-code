#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            integers_ = new std::vector<int>();
            for(const auto& s : *input_strings_)
            {
                integers_->push_back(std::atoi(s.c_str()));
            }
        }

    private:
        std::string PartOne()
        {
            int out = 0;
            for(size_t i=1; i<integers_->size(); i++)
            {
                if((*integers_)[i] > (*integers_)[i-1]) ++out;
            }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            int out = 0;
            for(size_t i=3; i<integers_->size(); i++)
            {
                if((*integers_)[i] > (*integers_)[i-3]) ++out;
            }

            return std::to_string(out);
        }

        std::vector<int>* integers_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}