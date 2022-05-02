#ifndef MAT_HPP
#define MAT_HPP

struct Coord2D
{
    Coord2D(const int _r, const int _c)
    {
        r = _r; c = _c;
    }

    bool equals(const Coord2D& other) const
    {
        return ((other.r == r) && (other.c == c));
    }

    bool in(const std::vector<Coord2D>& v) const
    {
        for(const auto& vv : v)
        {
            if(equals(vv))
            {
                return true;
            }
        }

        return false;
    }

    std::vector<Coord2D> get_cardinal_neighbors() const
    {
        return std::vector<Coord2D> {
            Coord2D(r-1, c),
            Coord2D(r+1, c),
            Coord2D(r, c-1),
            Coord2D(r, c+1) };
    }

    std::vector<Coord2D> get_diagonal_neighbors() const
    {
        return std::vector<Coord2D> {
            Coord2D(r-1, c-1),
            Coord2D(r+1, c+1),
            Coord2D(r+1, c-1),
            Coord2D(r-1, c+1) };
    }

    std::vector<Coord2D> get_8_neighbors() const
    {
        auto cardinal = get_cardinal_neighbors();
        auto diagonal = get_diagonal_neighbors();

        cardinal.insert(cardinal.end(), diagonal.begin(), diagonal.end());

        return cardinal;
    }

    int r, c;
};

template <typename T>
class Matrix2D
{
    public:
        Matrix2D(const int h, const int w, const std::vector<T>& buf)
        {
            h_ = h; w_ = w;
            buf_ = new std::vector<T>(buf);
        }

        Matrix2D(const int h, const int w, const T v)
        {
            h_ = h; w_ = w;
            buf_ = new std::vector<T>(h_*w_, v);
        }

        Matrix2D(const Matrix2D<T>& other)
        {
            h_ = other.h_; w_ = other.w_; 
            buf_ = new std::vector<T>(*other.buf_);
        }

        void set(const int r, const int c, T v)
        {
            (*buf_)[ravel_idx(r,c)] = v;
        }

        T get(const int r, const int c)
        {
            return (*buf_)[ravel_idx(r,c)];
        }

        std::vector<T>* get_buf() { return buf_; }

        bool within_bounds(const int r, const int c) const
        {
            return ((r >= 0) && (r < h_) && (c >= 0) && (c < w_));
        }

        void unravel_idx(const int i, int& r, int& c)
        {
            r = i / w_; c = i - (r * w_);
        }

        void set(const Coord2D& pt, T v) { set(pt.r, pt.c, v); }
        T get(const Coord2D& pt) { return get(pt.r, pt.c); }
        bool within_bounds(const Coord2D& pt) const { return within_bounds(pt.r, pt.c); }
        void unravel_idx(const int i, Coord2D& pt) { unravel_idx(pt.r, pt.c); }

        int height() const { return h_; }
        int width() const { return w_; }
        int size() const { return buf_->size(); }

    private:
        int ravel_idx(const int r, const int c)
        {
            return r*w_ + c;
        }

        int h_, w_;
        std::vector<T>* buf_;
};

Matrix2D<int> mat_from_09_int_strings(const std::vector<std::string>& input_strings)
{
    int h = input_strings.size(),
        w = input_strings[0].size();

    std::vector<int> buf;
    for(const auto&s : input_strings)
    {
        for(const char c : s)
        {
            buf.push_back(std::atoi(&c));
        }
    }

    return Matrix2D<int>(h, w, buf);
}

#endif