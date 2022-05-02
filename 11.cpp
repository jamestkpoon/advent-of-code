#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            mat_ = new Matrix2D<int>(mat_from_09_int_strings(*input_strings_));
        }

    private:
        int propagate_one_day()
        {
            for(int& i : *mat_->get_buf()) { ++i; }

            int nf_sum = 0;
            while(true)
            {
                int nf = 0;
                for(int r=0; r<mat_->height(); r++)
                {
                    for(int c=0; c<mat_->width(); c++)
                    {
                        Coord2D pos_(r,c);
                        if(mat_->get(pos_) > 9)
                        {
                            mat_->set(pos_, 0);
                            ++nf;

                            for(const auto& n_pos : pos_.get_8_neighbors())
                            {
                                if(mat_->within_bounds(n_pos))
                                {
                                    int nv = mat_->get(n_pos);
                                    if(nv != 0) { mat_->set(n_pos, nv + 1); }
                                }
                            }
                        }
                    }
                }

                nf_sum += nf;
                if(nf == 0) { return nf_sum; }
            }
        }

        std::string PartOne()
        {
            int out = 0;
            for(int i=0; i<100; i++) { out += propagate_one_day(); }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            int out = 1, sz = mat_->size();
            while(propagate_one_day() != sz) { ++out; }

            return std::to_string(out);
        }

        Matrix2D<int> *mat_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}