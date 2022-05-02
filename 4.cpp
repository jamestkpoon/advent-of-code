#include "base.hpp"

class BingoBoard
{
    public:
        BingoBoard(const int _h, const int _w, const std::vector<int>& _board_values)
        {
            board_ = new Matrix2D<int>(_h, _w, _board_values);
            hit_ = new Matrix2D<bool>(_h, _w, false);
            won_ = false;
        }

        bool play(int x)
        {
            for(int r=0; r<board_->height(); r++)
            {
                for(int c=0; c<board_->width(); c++)
                {
                    if(board_->get(r,c) == x)
                    {
                        return update_hit_and_perform_win_check(r, c);
                    }
                }
            }

            return false;
        }

        int unmarked_sum()
        {
            int out = 0;

            for(int r=0; r<board_->height(); r++)
            {
                for(int c=0; c<board_->width(); c++)
                {
                    if(!hit_->get(r,c))
                    {
                        out += board_->get(r,c);
                    }
                }
            }

            return out;
        }

        const bool is_won() { return won_; }

    private:
        bool update_hit_and_perform_win_check(const int check_row, const int check_col)
        {
            hit_->set(check_row,check_col, true);

            bool all_hit = true;
            for(int r=0; r<hit_->height(); r++)
            {
                if(!hit_->get(r, check_col))
                {
                    all_hit = false;
                    break;
                }
            }
        
            if(all_hit)
            {
                won_ = true;
                return true;
            }

            all_hit = true;
            for(int c=0; c<hit_->width(); c++)
            {
                if(!hit_->get(check_row, c))
                {
                    all_hit = false;
                    break;
                }
            }

            if(all_hit)
            {
                won_ = true;
                return true;
            }

            return false;            
        }

        Matrix2D<int>* board_;
        Matrix2D<bool>* hit_;
        bool won_;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            calls_ = new std::vector<int>(split_string_to_ints((*input_strings_)[0], ','));

            boards_ = new std::vector<BingoBoard>();
            std::vector<std::vector<int>> bufs;
            input_strings_->push_back("");
            for(size_t i=2; i<input_strings_->size(); i++)
            {
                auto s = (*input_strings_)[i];
                if(s.size() != 0)
                {
                    bufs.push_back(split_string_to_ints(s, ' '));
                }
                else
                {
                    std::vector<int> b;
                    for(const auto& buf : bufs)
                    {
                        b.insert(b.end(), buf.begin(), buf.end());
                    }

                    boards_->push_back(BingoBoard(bufs.size(), bufs[0].size(), b));
                    bufs.clear();
                }
            }            
        }

    private:
        std::string PartOne()
        {
            for(const auto c : *calls_)
            {
                for(auto& b: *boards_)
                {
                    if(b.play(c))
                    {
                        return std::to_string(c * b.unmarked_sum());
                    }
                }
            }

            return "";
        }

        std::string PartTwo()
        {
            int num_boards_won = 0;

            for(const auto c : *calls_)
            {
                for(auto& b: *boards_)
                {
                    if((!b.is_won() && b.play(c)) && (++num_boards_won == boards_->size()))
                    {
                        return std::to_string(c * b.unmarked_sum());
                    }
                }
            }

            return "";
        }

        std::vector<int>* calls_;
        std::vector<BingoBoard>* boards_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}