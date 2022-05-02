#include "base.hpp"

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            mat_ = new Matrix2D<int>(mat_from_09_int_strings(*input_strings_));
        }

    private:
        std::vector<Coord2D> plan(const Coord2D& start, const Coord2D& end)
        {
            Matrix2D<int> costs_cumulative(mat_->height(), mat_->width(), INT32_MAX);
            costs_cumulative.set(start, mat_->get(start));
            std::vector<Coord2D> unvisited;
            for(int r=0; r<mat_->height(); r++)
            {
                for(int c=0; c<mat_->width(); c++)
                {
                    unvisited.push_back(Coord2D(r,c));
                }
            }

            while(costs_cumulative.get(end) == INT32_MAX)
            {
                int ui = -1, ui_cc = INT32_MAX;
                for(int i=0; i<unvisited.size(); i++)
                {
                    if(costs_cumulative.get(unvisited[i]) < ui_cc)
                    {
                        ui = i; ui_cc = costs_cumulative.get(unvisited[i]);
                    }
                }

                for(const auto& n : unvisited[ui].get_cardinal_neighbors())
                {
                    if(!costs_cumulative.within_bounds(n)) { continue; }

                    costs_cumulative.set(n,
                        std::min(costs_cumulative.get(n), ui_cc+mat_->get(n)));
                }

                unvisited.erase(unvisited.begin() + ui);
            }

            std::vector<Coord2D> out { end };
            while(!start.equals(out[0]))
            {
                Coord2D neighbor(-1,-1); int best_n_c = costs_cumulative.get(out[0]);
                for(const auto& n : out[0].get_cardinal_neighbors())
                {
                    if(costs_cumulative.within_bounds(n) &&
                        (costs_cumulative.get(n) < best_n_c))
                    {
                        neighbor = n; best_n_c = costs_cumulative.get(n);
                    }
                }

                out.insert(out.begin(), neighbor);
            }

            return out;
        }

        int get_path_cost(const std::vector<Coord2D>& path, const bool discard_start_cost)
        {
            int out = discard_start_cost ? -mat_->get(path[0]) : 0;
            for(const auto& p : path) { out += mat_->get(p); }

            return out;
        }

        std::string PartOne()
        {
            Coord2D start(0,0), end(mat_->height()-1, mat_->width()-1);
            int out = get_path_cost(plan(start, end), true);
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            std::vector<Matrix2D<int>> submats;
            auto buf = *mat_->get_buf();            
            for(int i=0; i<9; i++)
            {
                submats.push_back(Matrix2D<int>(mat_->height(), mat_->width(), buf));

                for(int& v : buf) { if(++v == 10) v = 1; }
            }

            Matrix2D<int> mat_full(mat_->height()*5, mat_->width()*5, 0);
            for(int r_major=0; r_major<5; r_major++)
            {
                for(int c_major=0; c_major<5; c_major++)
                {
                    for(int rr=0, wr=r_major*mat_->height(); rr<mat_->height(); rr++, wr++)
                    {
                        for(int rc=0, wc=c_major*mat_->width(); rc<mat_->width(); rc++, wc++)
                        {
                            mat_full.set(wr,wc, submats[r_major+c_major].get(rr,rc));
                        }
                    }
                }
            }
            *mat_ = mat_full;

            Coord2D start(0,0), end(mat_->height()-1, mat_->width()-1);
            int out = get_path_cost(plan(start, end), true);
            return std::to_string(out);
        }

        Matrix2D<int> *mat_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}