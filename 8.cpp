#include "base.hpp"
#include <map>

class SSDisplay
{
    public:
        static int atoi(const std::string& s)
        {
            switch(s.size())
            {
                case 2: return 1;
                case 3: return 7;
                case 4: return 4;
                case 7: return 8;
                default: return -1;
            }
        }

        static int concat(const std::vector<int>& digits)
        {
            int out = 0;
            for(int i=digits.size()-1, m=1; i>=0; i--, m*=10)
            {
                out += m * digits[i];
            }

            return out;
        }

        SSDisplay(const std::string& s)
        {
            auto ss = split_string(s, '|');
            signals_ = new std::vector<std::string>(split_string(ss[0], ' '));
            outputs_ = new std::vector<std::string>(split_string(ss[1], ' '));
       }

        int count_known_outputs()
        {
            int out = 0;
            for(const auto& s : *outputs_)
            {
                out += (atoi(s) != -1) ? 1 : 0;
            }

            return out;
        }

        int read(const std::vector<std::string>& seqs)
        {
            solve_char_map();

            std::vector<int> digits;
            for(const auto& s : *outputs_)
            {
                digits.push_back(atoi(s, seqs));
            }

            return concat(digits);
        }

    private:
        void solve_char_map()
        {
            auto signal_map = std::map<int,std::string>();
            for(const auto& s : *signals_)
            {
                int i = atoi(s);
                if(i != -1)
                {
                    signal_map[i] = s;
                }
            }

            char_map = new std::map<char,char>();

            // a
            char a_key;
            for(const char c : signal_map[7])
            {
                if(!contains(signal_map[1], c))
                {
                    a_key = c;
                    (*char_map)[a_key] = 'a';
                    break;
                }
            }            
            
            // g
            std::vector<std::string> signals_length_gt_4;
            for(const auto& s : *signals_)
            {
                if(s.size() > 4)
                {
                    signals_length_gt_4.push_back(s);
                }
            }

            std::string gt4_isec = set_intersection(signals_length_gt_4);
            char g_key = (a_key == gt4_isec[0]) ? gt4_isec[1] : gt4_isec[0];
            (*char_map)[g_key] = 'g';

            // e
            for(const char c : signal_map[8])
            {
                if((c != a_key) && (c != g_key) && !contains(signal_map[4], c))
                {
                    (*char_map)[c] = 'e';
                    break;
                }
            }

            // d
            std::vector<std::string> signals_length_eq_5;
            for(const auto& s : *signals_)
            {
                if(s.size() == 5)
                {
                    signals_length_eq_5.push_back(s);
                }
            }

            for(const char c : set_intersection(signals_length_eq_5))
            {
                if((c != a_key) && (c != g_key))
                {
                    (*char_map)[c] = 'd';
                    break;
                }
            }

            // c
            char c_key;
            bool c_found = false;
            for(const auto& s : *signals_)
            {
                if(s.size() == 6)
                {
                    for(const char c : signal_map[1])
                    {
                        if(!contains(s, c))
                        {
                            c_key = c;
                            (*char_map)[c] = 'c';
                            c_found = true;
                            break;
                        }
                    }
                }

                if(c_found) { break; }
            }

            // f
            char f_key = (c_key == signal_map[1][0]) ? signal_map[1][1] : signal_map[1][0];
            (*char_map)[f_key] = 'f';

            // b
            for(const char c : signal_map[4])
            {
                if(char_map->find(c) == char_map->end())
                {
                    (*char_map)[c] = 'b';
                    break;
                }
            }
        }

        int atoi(const std::string& s, const std::vector<std::string>& seqs)
        {
            for(int i=0; i<seqs.size(); i++)
            {
                if(s.size() != seqs[i].size()) { continue; }

                bool match = true;
                for(const char c : s)
                {
                    if(!contains(seqs[i], (*char_map)[c]))
                    {
                        match = false;
                        break;
                    }
                }

                if(match) { return i; }
            }

            return -1;
        }

        std::vector<std::string> *signals_, *outputs_;
        std::map<char, char>* char_map;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {

        }

    private:
        std::string PartOne()
        {
            int out = 0;
            for(const auto& s : *input_strings_)
            {
                out += (new SSDisplay(s))->count_known_outputs();
            }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto seqs = std::vector<std::string> {
                "abcefg",
                "cf",
                "acdeg",
                "acdfg",
                "bcdf",
                "abdfg",
                "abdefg",
                "acf",
                "abcdefg",
                "abcdfg"
            };

            int out = 0;
            for(const auto& s : *input_strings_)
            {
                out += (new SSDisplay(s))->read(seqs);
            }

            return std::to_string(out);
        }
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}
