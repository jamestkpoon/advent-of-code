#include "base.hpp"
#include <map>
#include <algorithm>

struct Insertion
{
    Insertion(const std::string& _ab, const char c)
    {
        ab = _ab;
        ac = std::string({ ab[0] }) + c;
        cb = c + std::string({ ab[1] });
    }

    std::string ab, ac, cb;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            pairs_ = new std::map<std::string,long>();
            auto& starting_str = (*input_strings_)[0];
            (*pairs_)[std::string({ starting_str[0] })] = 1;
            (*pairs_)[std::string({ starting_str.back() })] = 1;
            for(int i=0; i<starting_str.size()-1; i++)
            {
                incr_map(*pairs_, starting_str.substr(i,2), 1);
            }

            insertions_ = new std::vector<Insertion>();
            for(int i=2; i<input_strings_->size(); i++)
            {
                auto& s = (*input_strings_)[i];
                insertions_->push_back(Insertion(s.substr(0,2), s.back()));
            }
        }

    private:
        void incr_map(std::map<std::string,long>& map,
            const std::string& k, const long dv)
        {
            if(map.find(k) != map.end())
            {
                map[k] += dv;
                if(map[k] == 0) { map.erase(k); }
            }
            else { map[k] = dv; }
        }

        void step()
        {
            std::map<std::string,long> dc;
            for(const auto& i : *insertions_)
            {
                auto itr = pairs_->find(i.ab);
                if(itr != pairs_->end())
                {
                    incr_map(dc, i.ab, -itr->second);
                    incr_map(dc, i.ac,  itr->second);
                    incr_map(dc, i.cb,  itr->second);
                }
            }

            for(const auto& kv : dc) { incr_map(*pairs_, kv.first, kv.second); }
        }

        std::map<std::string,long> count()
        {
            std::map<std::string,long> out;
            for(const auto& kv : *pairs_)
            {
                for(const char c : kv.first)
                {
                    incr_map(out, std::string({ c }), kv.second);
                }
            }

            for(const auto& kv : out) { out[kv.first] = kv.second / 2; }

            return out;
        }

        long count_diff(const long n_steps)
        {
            for(long i=0; i<n_steps; i++) { step(); }
            
            std::vector<long> counts_arr;
            for(const auto& kv : count()) { counts_arr.push_back(kv.second); }
            std::sort(counts_arr.begin(), counts_arr.end());

            return counts_arr.back() - counts_arr[0];
        }

        std::string PartOne()
        {
            long out = count_diff(10);
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            long out = count_diff(40);
            return std::to_string(out);
        }

        std::map<std::string,long> *pairs_;
        std::vector<Insertion> *insertions_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}