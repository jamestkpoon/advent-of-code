#include "base.hpp"
#include <algorithm>

struct LineSegment
{
    LineSegment(const std::string& s)
    {
        auto segments = split_string(s, ' ');
        auto pt1 = split_string_to_ints(segments[0], ',');
        auto pt2 = split_string_to_ints(segments[2], ',');

        x1 = pt1[0]; y1 = pt1[1]; x2 = pt2[0]; y2 = pt2[1];

        dx = (x1 == x2) ? 0 : ( (x2 > x1) ? 1 : -1 );
        dy = (y1 == y2) ? 0 : ( (y2 > y1) ? 1 : -1 );
        x2_exclusive = x2 + dx;
        y2_exclusive = y2 + dy;
    }

    int x1, y1, x2, y2;
    int dx, dy, x2_exclusive, y2_exclusive;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            line_segments = new std::vector<LineSegment>();
            for(const auto& s : *input_strings_)
            {
                line_segments->push_back(LineSegment(s));
            }

            int x_max = 0, y_max = 0;
            for(const auto& ls : *line_segments)
            {
                x_max = std::max(x_max, std::max(ls.x1, ls.x2));
                y_max = std::max(y_max, std::max(ls.y1, ls.y2));
            }
            mat_ = new Matrix2D<int>(y_max+1, x_max+1, 0);
        }

    private:
        int count_overlap(bool omit_diagonals)
        {
            for(const auto& ls : *line_segments)
            {
                if(omit_diagonals && (ls.dx != 0) && (ls.dy != 0)) continue;

                int x = ls.x1, y = ls.y1;
                while((x != ls.x2_exclusive) || (y != ls.y2_exclusive))
                {
                    mat_->set(y,x, mat_->get(y,x) + 1);
                    x += ls.dx; y += ls.dy;
                }
            }

            int out = 0;
            for(const int v : *(mat_->get_buf()))
            {
                out += (v > 1) ? 1 : 0;
            }

            return out;
        }

        std::string PartOne()
        {
            return std::to_string(count_overlap(true));
        }

        std::string PartTwo()
        {
            return std::to_string(count_overlap(false));
        }

        std::vector<LineSegment>* line_segments;
        Matrix2D<int>* mat_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}