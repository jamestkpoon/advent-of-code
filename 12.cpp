#include "base.hpp"
#include <map>
#include <algorithm>

struct Edge
{
    Edge(const uint8_t& _a, const uint8_t& _b)
    {
        a = _a; b = _b;
    }

    uint8_t a, b;
};

struct Path
{
    Path(const uint8_t& start_vertex,
        const std::vector<uint8_t>& _path,
        const std::map<uint8_t, int>& _visited_vertices_counters)
    {
        path = _path;
        visited_vertices_counters = _visited_vertices_counters;

        path.push_back(start_vertex);
        visited_vertices_counters[start_vertex] += 1;
    }

    std::vector<Path> propagate(const std::vector<Edge>& edges,
        const std::map<uint8_t,int>& visited_vertices_limits)
    {
        std::vector<Path> paths;
        for(const auto& edge : edges)
        {
            bool a_eq = (edge.a == path.back()), b_eq = (edge.b == path.back());
            if(!(a_eq || b_eq)) continue;

            auto vn = a_eq ? edge.b : edge.a;
            int vvl = visited_vertices_limits.at(vn);
            if((vvl < 0) || (visited_vertices_counters.at(vn) < vvl))
            {
                paths.push_back(Path(vn, path, visited_vertices_counters));
            }
        }

        return paths;
    }

    std::vector<uint8_t> path;
    std::map<uint8_t, int> visited_vertices_counters;
};

class Day : public Base
{
    public:
        Day(const char* input_filepath) : Base(input_filepath)
        {
            vertex_names = new std::map<std::string, uint8_t>();
            edges = new std::vector<Edge>();

            for(const auto& s : *input_strings_)
            {
                auto vns = split_string(s, '-');

                for(const auto& vn : vns)
                {
                    if(vertex_names->find(vn) == vertex_names->end())
                    {
                        (*vertex_names)[vn] = vertex_names->size();
                    }
                }

                edges->push_back(Edge(vertex_names->at(vns[0]), vertex_names->at(vns[1])));
            }
        }

    private:
        std::vector<Path> plans(const std::string& sn, const std::string& en,
            const std::map<uint8_t,int>& visited_vertices_limits)
        {
            std::map<uint8_t,int> vvc;
            for(const auto& kv : *vertex_names) { vvc[kv.second] = 0; }
            std::vector<Path> paths_fin_,
                paths_wip { Path(vertex_names->at(sn), std::vector<uint8_t>(), vvc) };
            auto end_idx = vertex_names->at(en);

            while(true)
            {
                std::vector<Path> paths_wip_new;

                for(auto& p : paths_wip)
                {
                    if(p.path.back() == end_idx) { paths_fin_.push_back(p); }
                    else
                    {
                        for(const auto& pp : p.propagate(*edges, visited_vertices_limits))
                        {
                            paths_wip_new.push_back(pp);
                        }
                    }
                }

                if(!paths_wip_new.empty()) { paths_wip = paths_wip_new; }
                else { return paths_fin_; }
            }
        }

        std::string PartOne()
        {
            std::map<uint8_t,int> vvl;
            for(const auto& itr : *vertex_names)
            {
                vvl[itr.second] = std::isupper(itr.first[0]) ? -1 : 1;
            }

            int out_ = plans("start", "end", vvl).size();
            return std::to_string(out_);
        }

        std::string PartTwo()
        {
            std::map<uint8_t,int> vvl;
            std::vector<uint8_t> can_visit_twice;
            for(const auto& itr : *vertex_names)
            {
                vvl[itr.second] = std::isupper(itr.first[0]) ? -1 : 1;

                if(std::islower(itr.first[0]) &&
                    (itr.first != "start") && (itr.first != "end"))
                {
                    can_visit_twice.push_back(itr.second);
                }
            }

            std::vector<std::vector<uint8_t>> paths_all_;
            for(const auto& vk : can_visit_twice)
            {
                auto vvl2 = vvl; vvl2[vk] = 2;
                for(const auto& p : plans("start", "end", vvl2))
                {
                    bool is_new = true;
                    for(const auto& existing_path : paths_all_)
                    {
                        if(p.path == existing_path)
                        {
                            is_new = false;
                            break;
                        }
                    }

                    if(is_new) { paths_all_.push_back(p.path); }
                }
            }

            return std::to_string(paths_all_.size());
        }

        std::map<std::string, uint8_t> *vertex_names;
        std::vector<Edge> *edges;
};

int main(int argc, char *argv[])
{
    (new Day(argv[1]))->run(argv[2]);

    return 0;
}