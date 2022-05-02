#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <fstream>
#include "string_tools.hpp"
#include "mat.hpp"

class Base
{
    public:
        Base(const char* input_filepath)
        {
            input_strings_ = new std::vector<std::string>();

            std::ifstream file(input_filepath);
            if(file.is_open())
            {
                std::string line;
                while(getline(file, line))
                {
                    input_strings_->push_back(line);
                }
            }
        }

        void run(const char* part_string)
        {
            switch(std::atoi(part_string))
            {
                case 1:
                    std::cout << PartOne() << std::endl;
                    break;
                case 2:
                    std::cout << PartTwo() << std::endl;
                    break;
                default: break;
            }
        }

    protected:
        virtual std::string PartOne() { return ""; }
        virtual std::string PartTwo() { return ""; }

        std::vector<std::string>* input_strings_;
};

#endif

/*

#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {

        }

    private:
        std::string PartOne()
        {
            int out_ = 0;
            return std::to_string(out_);
        }

        std::string PartTwo()
        {
            int out_ = 0;
            return std::to_string(out_);
        }
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}

*/