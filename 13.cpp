#include "base.hpp"

struct Fold
{
    Fold(const char _axis, const int _pos)
    {
        axis = _axis; pos = _pos;
    }

    char axis;
    int pos;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            std::vector<Coord2D> trues;
            int r_max = 0, c_max = 0;
            folds_ = new std::vector<Fold>();

            for(const auto& s : *input_strings_)
            {
                if(contains(s,','))
                {
                    auto coord_ = split_string_to_ints(s, ',');
                    trues.push_back(Coord2D(coord_[1], coord_[0]));
                    r_max = std::max(r_max, trues.back().r);
                    c_max = std::max(c_max, trues.back().c);
                }
                else if(contains(s,'='))
                {
                    auto ss_ = split_string(s, '=');
                    folds_->push_back(Fold(ss_[0].back(), std::atoi(ss_[1].c_str())));
                }
            }

            mat_ = new Matrix2D<char>(r_max+1, c_max+1, ' ');
            for(const auto& p : trues) { mat_->set(p, '#'); }
        }

    private:
        Matrix2D<char> fold(Matrix2D<char>* M, const Fold& fold)
        {
            if(fold.axis == 'x')
            {
                Matrix2D<char> out(M->height(), fold.pos, ' ');
                for(int r=0; r<M->height(); r++)
                {
                    for(int c=0; c<fold.pos; c++)
                    {
                        if(M->get(r,c) == '#') { out.set(r,c, '#'); }
                    }

                    for(int cr=fold.pos+1, cw=fold.pos-1; cr<M->width(); cr++, cw--)
                    {
                        if(M->get(r,cr) == '#') { out.set(r,cw, '#'); }
                    }
                }

                return out;
            }
            else if(fold.axis == 'y')
            {
                Matrix2D<char> out(fold.pos, M->width(), ' ');
                for(int c=0; c<M->width(); c++)
                {
                    for(int r=0; r<fold.pos; r++)
                    {
                        if(M->get(r,c) == '#') { out.set(r,c, '#'); }
                    }

                    for(int rr=fold.pos+1, rw=fold.pos-1; rr<M->height(); rr++, rw--)
                    {
                        if(M->get(rr,c) == '#') { out.set(rw,c, '#'); }
                    }
                }

                return out;
            }
            else { return Matrix2D<char>(1,1,' '); }
        }

        std::string PartOne()
        {
            int out_ = 0;
            for(const char c : *fold(mat_, (*folds_)[0]).get_buf())
            {
                if(c == '#') { ++out_; }
            }
            return std::to_string(out_);
        }

        std::string PartTwo()
        {
            for(const auto& f : *folds_) { *mat_ = fold(mat_, f); }
            
            for(int r=0; r<mat_->height(); r++)
            {
                for(int c=0; c<mat_->width(); c++)
                {
                    std::cout << mat_->get(r,c);
                }
                std::cout << std::endl;
            }

            return "";
        }

        Matrix2D<char> *mat_;
        std::vector<Fold> *folds_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}