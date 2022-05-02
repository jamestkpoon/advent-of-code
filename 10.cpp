#include "base.hpp"
#include <algorithm>

struct Char2WithScore
{
    Char2WithScore(const char _a, const char _b, const int _score)
    {
        a = _a; b = _b;
        score = _score;
    }

    char a, b;
    int score;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {

        }

    private:
        void sort(const std::vector<Char2WithScore>& c2wss)
        {
            corrupted_scores_ = new std::vector<int>();
            incomplete_substrs_ = new std::vector<std::string>();
            for(const auto& s : *input_strings_)
            {
                auto sf = filter(s, c2wss);
                int score = 0;

                for(const char c : sf)
                {
                    for(const auto& c2ws : c2wss)
                    {
                        if(c == c2ws.b)
                        {
                            score = c2ws.score;
                            break;
                        }
                    }

                    if(score != 0) { break; }
                }

                if(score != 0)
                {
                    corrupted_scores_->push_back(score);
                }
                else
                {
                    incomplete_substrs_->push_back(sf);
                }
            }
        }

        std::string filter(const std::string& _s,
            const std::vector<Char2WithScore>& c2wss)
        {
            std::string s = _s;
            while(true)
            {
                auto prev_sz_ = s.size();
                for(int a=0, b=1; b<s.size(); a++, b++)
                {
                    for(const auto& c2ws : c2wss)
                    {
                        if((s[a] == c2ws.a) && (s[b] == c2ws.b))
                        {
                            s.erase(a, 2);
                        }
                    }
                }

                if((s.size() == prev_sz_) || s.empty()) { return s; }
            }
        }

        std::string PartOne()
        {
            sort(std::vector<Char2WithScore> {
                Char2WithScore('(', ')', 3),
                Char2WithScore('[', ']', 57),
                Char2WithScore('{', '}', 1197),
                Char2WithScore('<', '>', 25137)
            });

            uint64_t out = 0;
            for(const int i: *corrupted_scores_) { out += i; }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto c2ws_ = std::vector<Char2WithScore> {
                Char2WithScore('(', ')', 1),
                Char2WithScore('[', ']', 2),
                Char2WithScore('{', '}', 3),
                Char2WithScore('<', '>', 4)
            };

            sort(c2ws_);

            std::vector<uint64_t> scores;
            for(const auto& s : *incomplete_substrs_)
            {
                uint64_t score = 0;
                for(int i=s.size()-1; i>=0; i--)
                {
                    score *= 5;
                    for(const auto& c2ws : c2ws_)
                    {
                        if(s[i] == c2ws.a)
                        {
                            score += c2ws.score;
                            break;
                        }
                    }
                }

                scores.push_back(score);
            }

            std::sort(scores.begin(), scores.end());
            return std::to_string(scores[scores.size() / 2]);
        }

        std::vector<int> *corrupted_scores_;
        std::vector<std::string> *incomplete_substrs_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}