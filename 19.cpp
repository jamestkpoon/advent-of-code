#include "base.hpp"

struct Vector3
{
    Vector3() { x = y = z = 0; }

    Vector3(const std::string& s, const char sep=',')
    {
        auto xyz = split_string_to_ints(s, sep);
        x = xyz.at(0); y = xyz.at(1); z = xyz.at(2);
    }

    Vector3(const int _x, const int _y, const int _z)
    {
        x = _x; y = _y; z = _z;
    }

    Vector3 abs() const
    {
        return Vector3(
          std::abs(x), std::abs(y), std::abs(z)  
        );
    }

    bool operator ==(const Vector3& other) const
    {
        return ((x == other.x) && (y == other.y) && (z == other.z));
    };

    Vector3 operator +(const Vector3& other) const
    {
        return Vector3(x+other.x, y+other.y, z+other.z);
    }

    Vector3 operator -(const Vector3& other) const
    {
        return Vector3(x-other.x, y-other.y, z-other.z);
    }

    Vector3 rotate(const int rot_mode) const
    {
        Vector3 out;

        switch(rot_mode)
        {
            case 0:
                out.x = x; out.y = y; out.z = z;
                break;
            case 1:
                out.x = x; out.y = -y; out.z = -z;
                break;
            case 2:
                out.x = x; out.y = -z; out.z = y;
                break;
            case 3:
                out.x = x; out.y = z; out.z = -y;
                break;
            case 4:
                out.x = -x; out.y = -y; out.z = z;
                break;
            case 5:
                out.x = -x; out.y = y; out.z = -z;
                break;
            case 6:
                out.x = -x; out.y = z; out.z = y;
                break;
            case 7:
                out.x = -x; out.y = -z; out.z = -y;
                break;

            case 8:
                out.x = y; out.y = z; out.z = x;
                break;
            case 9:
                out.x = y; out.y = -z; out.z = -x;
                break;
            case 10:
                out.x = y; out.y = -x; out.z = z;
                break;
            case 11:
                out.x = y; out.y = x; out.z = -z;
                break;
            case 12:
                out.x = -y; out.y = -z; out.z = x;
                break;
            case 13:
                out.x = -y; out.y = z; out.z = -x;
                break;
            case 14:
                out.x = -y; out.y = x; out.z = z;
                break;
            case 15:
                out.x = -y; out.y = -x; out.z = -z;
                break;

            case 16:
                out.x = z; out.y = x; out.z = y;
                break;
            case 17:
                out.x = z; out.y = -x; out.z = -y;
                break;
            case 18:
                out.x = z; out.y = -y; out.z = x;
                break;
            case 19:
                out.x = z; out.y = y; out.z = -x;
                break;
            case 20:
                out.x = -z; out.y = -x; out.z = y;
                break;
            case 21:
                out.x = -z; out.y = x; out.z = -y;
                break;
            case 22:
                out.x = -z; out.y = y; out.z = x;
                break;
            case 23:
                out.x = -z; out.y = -y; out.z = -x;
                break;

            default: break;
        }

        return out;
    }

    int x, y, z;
};

struct Pose
{
    Pose() { }

    Pose(const Vector3& _pos, const int _rot_mode)
    {
        pos = _pos; rot_mode = _rot_mode;
    }

    void transform(Vector3& v) const
    {
        v = v.rotate(rot_mode) + pos;
    }

    void transform(std::vector<Vector3>& vv) const
    {
        for(auto& v : vv) { transform(v); }
    }

    Vector3 pos;
    int rot_mode;
};

struct Transform : Pose
{
    Transform() : Pose() { }

    Transform(const Pose& pose,
            const int _parent_idx, const int _child_idx)
        : Pose(pose.pos, pose.rot_mode)
    {
        parent_idx = _parent_idx; child_idx = _child_idx;
    }

    int child_idx, parent_idx;
};

struct Scanner
{
    Scanner(const std::vector<Vector3>& _local_obs)
    {
        for(int i=0; i<24; i++)
        {
            std::vector<Vector3> rlo;
            for(const auto& lo : _local_obs)
            {
                rlo.push_back(lo.rotate(i));
            }

            rotated_local_observations.push_back(rlo);
        }
    }

    bool align(const Scanner& canonical, Pose& pose) const
    {
        auto canonical_obs = canonical.rotated_local_observations[0];

        pose.rot_mode = -1;
        for(const auto& world_obs_notrans : rotated_local_observations)
        {
            ++pose.rot_mode;

            for(const auto& anchor : canonical_obs)
            {
                for(const auto& wobs_nt_ref : world_obs_notrans)
                {
                    pose.pos = anchor - wobs_nt_ref;
                    std::vector<Vector3> world_obs;
                    for(const auto& wobs_nt : world_obs_notrans)
                    {
                        world_obs.push_back(wobs_nt + pose.pos);
                    }

                    int num_matches = 0;
                    std::vector<bool> wobs_hit(world_obs.size(), false);
                    for(int coI=0; coI<canonical_obs.size(); coI++)
                    {
                        for(int woI=0; woI<world_obs.size(); woI++)
                        {
                            if(wobs_hit[woI]) { continue; }
                            else if(canonical_obs[coI] == world_obs[woI])
                            {
                                if(++num_matches >= 12) { return true; }

                                wobs_hit[woI] = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    std::vector<std::vector<Vector3>> rotated_local_observations;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            scanners_ = new std::vector<Scanner>();
            int rI = 0; std::vector<Vector3> obs;
            while(++rI < input_strings_->size())
            {
                const auto& s = (*input_strings_)[rI];
                if(!s.empty())
                {
                    if(s[1] == '-') { continue; }
                    else { obs.push_back(Vector3(s)); }
                }
                else if(!obs.empty())
                {
                    scanners_->push_back(Scanner(obs));
                    obs.clear();
                }
            }

            solve_scanner_tfs();
        }

    private:
        void solve_scanner_tfs()
        {
            tfs_ = new std::vector<Transform>();
            tfs_->push_back(Transform(Pose(Vector3(), 0), 0, 0));
            
            std::vector<int> unhandled_indices, tfi(scanners_->size(), 0);
            for(int i=1; i<scanners_->size(); i++)
            {
                unhandled_indices.push_back(i);
            }

            while(!unhandled_indices.empty())
            {
                for(int ui=0; ui<unhandled_indices.size(); ui++)
                {
                    Transform tf_;
                    tf_.child_idx = unhandled_indices[ui];
                    const auto& from_ = (*scanners_)[tf_.child_idx];
                    while(tfi[tf_.child_idx] < tfs_->size())
                    {
                        tf_.parent_idx = (*tfs_)[tfi[tf_.child_idx]].child_idx;
                        if(from_.align((*scanners_)[tf_.parent_idx], tf_))
                        {
                            tfs_->push_back(tf_);
                            unhandled_indices.erase(unhandled_indices.begin() + ui);
                            break;
                        }

                        ++tfi[tf_.child_idx];
                    }
                }
            }
        }

        void find_beacons()
        {
            beacons_ = new std::vector<Vector3>();

            for(int cI=0; cI<tfs_->size(); cI++)
            {
                const auto& scanner = (*scanners_)[(*tfs_)[cI].child_idx];
                auto obs = scanner.rotated_local_observations[0];
                for(const int i : find_tf_path(cI)) { (*tfs_)[i].transform(obs); }

                int old_beacons_size = beacons_->size();
                for(const auto& pt : obs)
                {
                    bool is_new = true;
                    for(int i=0; i<old_beacons_size; i++)
                    {
                        if(pt == (*beacons_)[i])
                        {
                            is_new = false;
                            break;
                        }
                    }

                    if(is_new) { beacons_->push_back(pt); }
                }
            }
        }

        Vector3 find_child_pos(const int child_idx)
        {
            auto pos = Vector3();
            for(const int i : find_tf_path(child_idx))
            {
                (*tfs_)[i].transform(pos);
            }

            return pos;
        }

        std::vector<int> find_tf_path(const int child_idx)
        {
            std::vector<int> tf_path { child_idx };
            while((*tfs_)[tf_path.back()].child_idx != 0)
            {
                auto pI = (*tfs_)[tf_path.back()].parent_idx;
                for(int pI=0; pI<child_idx; pI++)
                {
                    if((*tfs_)[pI].child_idx == pI)
                    {
                        tf_path.push_back(pI);
                        break;
                    }
                }
            }

            return tf_path;
        }

        std::string PartOne()
        {
            find_beacons();
            int out = beacons_->size();
            return std::to_string(out);
        }

        std::string PartTwo()
        {
            std::vector<Vector3> positions;
            for(int i=0; i<tfs_->size(); i++)
            {
                positions.push_back(find_child_pos(i));
            }

            int out = 0;
            for(int i=0; i<positions.size()-1; i++)
            {
                for(int j=i+1; j<positions.size(); j++)
                {
                    auto diff = (positions[j] - positions[i]).abs();
                    out = std::max(out, diff.x + diff.y + diff.z);
                }
            }

            return std::to_string(out);
        }

        std::vector<Scanner> *scanners_;
        std::vector<Transform> *tfs_;
        std::vector<Vector3> *beacons_;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}