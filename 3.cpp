#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            byte_array_length_ = (*input_strings_)[0].size();
        }

    private:
        std::string PartOne()
        {
            std::string s = "";
            for(int i=0; i<byte_array_length_; i++)
            {
                int n1 = 0;
                for(const auto& s : *input_strings_)
                {
                    n1 += (s[i] == '1') ? 1 : 0;
                }

                s += (n1 >= (input_strings_->size() / 2)) ? '1' : '0';
            }

            int out = binary_to_decimal(s) * binary_to_decimal(s, '0');
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto rate = [this](bool majority)
            {
                std::vector<int> indices;
                for(int i=0; i<input_strings_->size(); i++)
                {
                    indices.push_back(i);
                }

                int column_idx = 0;
                while(indices.size() != 1)
                {
                    std::vector<int> hi, lo;
                    for(const int i : indices)
                    {
                        if((*input_strings_)[i][column_idx] == '1')
                        {
                            hi.push_back(i);
                        }
                        else
                        {
                            lo.push_back(i);
                        }
                    }

                    column_idx = (column_idx + 1) % byte_array_length_;
                    indices = majority ?
                        ((hi.size() >= lo.size()) ? hi : lo) :
                        ((lo.size() <= hi.size()) ? lo : hi);
                }

                return binary_to_decimal((*input_strings_)[indices[0]]);
            };

            int out = rate(true) * rate(false);
            return std::to_string(out);
        }

        int byte_array_length_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}
