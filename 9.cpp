#include "base.hpp"
#include <functional>
#include <algorithm>

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            mat_ = new Matrix2D<int>(mat_from_09_int_strings(*input_strings_));

            local_minima_ = new std::vector<Coord2D>();
            for(int r=0; r<mat_->height(); r++)
            {
                for(int c=0; c<mat_->width(); c++)
                {
                    int v = mat_->get(r, c);
                    Coord2D pt_(r, c);

                    bool is_minima = true;
                    for(const auto& pt_n : pt_.get_cardinal_neighbors())
                    {
                        if(mat_->within_bounds(pt_n) && (v >= mat_->get(pt_n)))
                        {
                            is_minima = false;
                            break;
                        }
                    }

                    if(is_minima) { local_minima_->push_back(pt_); }
                }
            }
        }

    private:
        std::vector<Coord2D> watershed(const Coord2D& origin,
            std::function<bool(const Coord2D&, const Coord2D&)> prop_fn)
        {
            std::vector<Coord2D> frontier { origin }, inliers;

            while(true)
            {
                for(const auto& pt_from : frontier)
                {
                    inliers.push_back(pt_from);
                }

                std::vector<Coord2D> frontier_new;
                for(const auto& pt_from : frontier)
                {
                    for(const auto& pt_to : pt_from.get_cardinal_neighbors())
                    {
                        if(prop_fn(pt_from,pt_to) &&
                            !pt_to.in(inliers) && !pt_to.in(frontier_new))
                        {
                            frontier_new.push_back(pt_to);
                        }
                    }
                }

                if(!frontier_new.empty()) { frontier = frontier_new; }
                else { return inliers; }
            }
        }

        std::string PartOne()
        {
            int out = 0;
            for(const auto& pt : *local_minima_)
            {
                out += mat_->get(pt) + 1;
            }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            auto prop_fn = [this](const Coord2D& pt_from, const Coord2D& pt_to)
            {
                if(!mat_->within_bounds(pt_to)) { return false; }

                int v_to = mat_->get(pt_to);
                return ((v_to != 9) && (v_to > mat_->get(pt_from)));
            };

            std::vector<int> basin_sizes_top3;
            for(const auto& pt : *local_minima_)
            {
                basin_sizes_top3.push_back(watershed(pt, prop_fn).size());

                if(basin_sizes_top3.size() == 4)
                {
                    std::sort(basin_sizes_top3.begin(), basin_sizes_top3.end());
                    basin_sizes_top3.erase(basin_sizes_top3.begin());
                }
            }

            int out = basin_sizes_top3[0] * basin_sizes_top3[1] * basin_sizes_top3[2];
            return std::to_string(out);
        }

        Matrix2D<int>* mat_;
        std::vector<Coord2D>* local_minima_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}