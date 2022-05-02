#include "base.hpp"
#include <map>

int wrap(const int x, const int lim)
{
    auto x_copy = x;
    while(x_copy > lim) { x_copy -= lim; }

    return (x_copy > 0) ? x_copy : lim;
}

struct Player
{
    Player(const int starting_pos)
    {
        pos = starting_pos;
        score = 0;
    }

    Player(const Player& other)
    {
        pos = other.pos;
        score = other.score;
    }

    void move(const int steps)
    {
        pos = wrap(pos+steps, 10);
        score += pos;
    }

    int pos, score;
};

int det_game(Player& p1, Player& p2, const int score_limit)
{
    int num_rolls = 0, die_value = 0;
    bool p1_turn = true;

    while(std::max(p1.score, p2.score) < score_limit)
    {
        int die_sum = 0;
        for(int i=0; i<3; i++)
        {
            die_value = wrap(die_value+1, 100);
            die_sum += die_value;
            ++num_rolls;
        }

        if(p1_turn) { p1.move(die_sum); }
        else { p2.move(die_sum); }

        p1_turn = !p1_turn;
    }

    return num_rolls;
}

struct WinCount
{
    WinCount() { p1 = p2 = 0; }

    void operator +=(const WinCount& other)
    {
        p1 += other.p1;
        p2 += other.p2;
    }

    uint64_t p1, p2;
};

using P1NextTurnWinCountMap = std::map<bool, WinCount>;
using P2ScoreWinCountMap = std::map<int, P1NextTurnWinCountMap>;
using P1ScoreWinCountMap = std::map<int, P2ScoreWinCountMap>;
using P2PosWinCountMap = std::map<int, P1ScoreWinCountMap>;
using P1PosWinCountMap = std::map<int, P2PosWinCountMap>;

WinCount quantum(Player& p1, Player& p2, const int score_limit,
    const bool p1_turn, P1PosWinCountMap& cache)
{
    auto& wc = cache.at(p1.pos).at(p2.pos).at(p1.score).at(p2.score).at(p1_turn);

    if((wc.p1 == 0) && (wc.p2 == 0))
    {
        for(int roll1=1; roll1<=3; roll1++)
        {
            for(int roll2=1; roll2<=3; roll2++)
            {
                for(int roll3=1; roll3<=3; roll3++)
                {
                    Player p1_copy(p1), p2_copy(p2);

                    int roll_sum = roll1 + roll2 + roll3;
                    bool game_over = false;

                    if(p1_turn)
                    {
                        p1_copy.move(roll_sum);
                        if(p1_copy.score >= score_limit) {
                            wc.p1++;
                            game_over = true;
                        }
                    }
                    else
                    {
                        p2_copy.move(roll_sum);
                        if(p2_copy.score >= score_limit)
                        {
                            wc.p2++;
                            game_over = true;
                        }
                    }

                    if(!game_over)
                    {
                        wc += quantum(p1_copy, p2_copy, score_limit, !p1_turn, cache);
                    }
                }
            }
        }
    }

    return wc;
}

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            int p1_sp = std::atoi(split_string((*input_strings_)[0], ':')[1].c_str());
            int p2_sp = std::atoi(split_string((*input_strings_)[1], ':')[1].c_str());
            p1_ = new Player(p1_sp); p2_ = new Player(p2_sp);
        }

    private:
        std::string PartOne()
        {
            auto num_rolls = det_game(*p1_, *p2_, 1000);
            auto out = num_rolls * std::min(p1_->score, p2_->score);
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            int score_limit = 21;

            P1PosWinCountMap p1p_cache_;
            for(int p1p=1; p1p<=10; p1p++)
            {
                P2PosWinCountMap p2p_cache;                
                for(int p2p=1; p2p<=10; p2p++)
                {
                    P1ScoreWinCountMap p1s_cache;
                    for(int p1s=0; p1s<score_limit; p1s++)
                    {
                        P2ScoreWinCountMap p2s_cache;
                        for(int p2s=0; p2s<score_limit; p2s++)
                        {
                            P1NextTurnWinCountMap p1n_cache;
                            for(const bool p1n : std::vector<bool> { false, true })
                            {
                                WinCount wc_;
                                p1n_cache[p1n] = wc_;
                            }
                            p2s_cache[p2s] = p1n_cache;
                        }
                        p1s_cache[p1s] = p2s_cache;
                    }
                    p2p_cache[p2p] = p1s_cache;
                }
                p1p_cache_[p1p] = p2p_cache;
            }

            auto wc = quantum(*p1_, *p2_, score_limit, true, p1p_cache_);
            auto out = std::max(wc.p1, wc.p2);
            return std::to_string(out);
        }

        Player *p1_, *p2_;        
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}