#include "base.hpp"

struct Cuboid
{
    Cuboid()
    {
        x_min = y_min = z_min = INT32_MIN;
        x_max = y_max = z_max = INT32_MAX;
    }

    Cuboid(const std::string& s)
    {
        auto ss_ = split_string(s, ',');
        auto xs_ = split_string(ss_[0], '.');
        auto ys_ = split_string(ss_[1], '.');
        auto zs_ = split_string(ss_[2], '.');

        x_min = std::atoi(split_string(xs_[0], '=')[1].c_str());
        x_max = std::atoi(xs_[1].c_str());
        y_min = std::atoi(split_string(ys_[0], '=')[1].c_str());
        y_max = std::atoi(ys_[1].c_str());
        z_min = std::atoi(split_string(zs_[0], '=')[1].c_str());
        z_max = std::atoi(zs_[1].c_str());
    }

    Cuboid(const Cuboid& other)
    {
        x_min = other.x_min; x_max = other.x_max;
        y_min = other.y_min; y_max = other.y_max;
        z_min = other.z_min; z_max = other.z_max;
    }

    bool is_valid() const
    {
        return ((x_max >= x_min) && (y_max >= y_min) && (z_max >= z_min));
    }

    int volume() const
    {
        int dx_ = x_max - x_min + 1,
            dy_ = y_max - y_min + 1,
            dz_ = z_max - z_min + 1;

        return dx_ * dy_ * dz_;
    }

    bool intersects_with(const Cuboid& other) const
    {
        return ((x_min <= other.x_max) || (x_max >= other.x_min) ||
            (y_min <= other.y_max) || (y_max >= other.y_min) ||
            (z_min <= other.z_max) || (z_max >= other.z_min));
    }

    bool intersection_with(const Cuboid& other, Cuboid& out) const
    {
        if(intersects_with(other))
        {
            out.x_min = std::max(x_min, other.x_min);
            out.y_min = std::max(y_min, other.y_min);
            out.z_min = std::max(z_min, other.z_min);

            out.x_max = std::min(x_max, other.x_max);
            out.y_max = std::min(y_max, other.y_max);
            out.z_max = std::min(z_max, other.z_max);

            return true;
        }

        return false;
    }

    bool encompasses(const Cuboid& other) const
    {
        return ((x_min <= other.x_min) && (x_max >= other.x_max) &&
            (y_min <= other.y_min) && (y_max >= other.y_max) &&
            (z_min <= other.z_min) && (z_max >= other.z_max));
    }

    std::vector<Cuboid> cut(const Cuboid& other) const
    {
        if(!intersects_with(other))
        {
            return std::vector<Cuboid> { Cuboid(*this) };
        }
        else
        {
            std::vector<Cuboid> out_;

            for(int dx=-1; dx<=1; dx++)
            {
                for(int dy=-1; dy<=1; dy++)
                {
                    for(int dz=-1; dz<=1; dz++)
                    {
                        if((dx == 0) && (dy == 0) && (dz == 0)) { continue; }

                        Cuboid cuboid(*this);

                        switch(dx)
                        {
                            case -1:
                                cuboid.x_max = other.x_min - 1;
                                break;
                            case 0:
                                cuboid.x_min = other.x_min;
                                cuboid.x_max = other.x_max;
                                break;
                            case 1:
                                cuboid.x_min = other.x_max + 1;
                                break;
                            default: break;                
                        }

                        switch(dy)
                        {
                            case -1:
                                cuboid.y_max = other.y_min - 1;
                                break;
                            case 0:
                                cuboid.y_min = other.y_min;
                                cuboid.y_max = other.y_max;
                                break;
                            case 1:
                                cuboid.y_min = other.y_max + 1;
                                break;
                            default: break;
                        }

                        switch(dz)
                        {
                            case -1:
                                cuboid.z_max = other.z_min - 1;
                                break;
                            case 0:
                                cuboid.z_min = other.z_min;
                                cuboid.z_max = other.z_max;
                                break;
                            case 1:
                                cuboid.z_min = other.z_max + 1;
                                break;
                            default: break;
                        }

                        if(cuboid.is_valid()) { out_.push_back(cuboid); }
                    }
                }
            }

            return out_;
        }
    }

    void print() const
    {
        std::cout << x_min << " " << x_max <<
            " " << y_min << " " << y_max <<
            " " << z_min << " " << z_max << std::endl; 
    }

    int x_min, x_max, y_min, y_max, z_min, z_max;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            step_bounds_ = new std::vector<Cuboid>();
            step_states_ = new std::vector<bool>();
            for(const auto& s : *input_strings_)
            {
                step_bounds_->push_back(Cuboid(s));
                step_states_->push_back(s[1] == 'n');
            }
        }

    private:
        std::string PartOne()
        {
            Cuboid bounds, isec;
            bounds.x_min = bounds.y_min = bounds.z_min = -50;
            bounds.x_max = bounds.y_max = bounds.z_max =  50;

            std::vector<bool> data(bounds.volume(), false);

            for(int i=0; i<step_bounds_->size(); i++)
            {
                if(bounds.intersection_with((*step_bounds_)[i], isec))
                {
                    for(int x=isec.x_min; x<=isec.x_max; x++)
                    {
                        for(int y=isec.y_min; y<=isec.y_max; y++)
                        {
                            for(int z=isec.z_min; z<=isec.z_max; z++)
                            {
                                int i = 101*101*(x+50) + 101*(y+50) + (z+50);
                                data[i] = (*step_states_)[i];
                            }
                        }
                    }
                }
            }

            int out = 0;
            for(const bool v : data) { if(v) { ++out; } }

            return std::to_string(out);
        }

        std::string PartTwo()
        {
            int out = 0;
            return std::to_string(out);
        }

        std::vector<Cuboid> *step_bounds_;
        std::vector<bool> *step_states_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}