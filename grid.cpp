#include <algorithm>
#include <cassert>

namespace lbm {
    typedef unsigned int uint_t;
    typedef double real_t;

    template <typename T , uint_t Cellsize>
    class Grid {
    public :
        inline Grid();
        inline Grid(uint_t xsize , uint_t ysize);
        inline ~Grid();
        inline T& operator () (uint_t x , uint_t y , uint_t f);
        inline T operator () (uint_t x , uint_t y , uint_t f) const;

        void swap (Grid& grid)
        {   std::swap (xsize_ , grid.xsize_);
            std::swap (ysize_ , grid.ysize_);
            std::swap (data_ , grid.data_);
        }

    private :
        uint_t xsize_;
        uint_t ysize_;
        T* data_;
    };

    template <typename T , uint_t Cellsize>
    Grid<T , Cellsize>::Grid ()
        :xsize_ (0) , ysize_ (0) , data_ (new T[1])
    {}

    template <typename T , uint_t Cellsize>
    Grid<T , Cellsize>::Grid (uint_t xsize , uint_t ysize)
        :xsize_ (xsize) , ysize_ (ysize) , data_ (new T[Cellsize * xsize * ysize])
    {}

    template <typename T , uint_t Cellsize>
    Grid<T , Cellsize>::~Grid ()
    {   delete[] data_;
    }

    template <typename T , uint_t Cellsize>
    inline T& Grid<T , Cellsize>::operator () (uint_t x , uint_t y , uint_t f)
    {   assert (x < xsize_ && y < ysize_ && f < Cellsize);
        return data_[y * xsize_ * Cellsize + x * Cellsize + f];
    }

    template <typename T , uint_t Cellsize>
    inline T Grid<T , Cellsize>::operator () (uint_t x , uint_t y , uint_t f) const
    {   assert (x < xsize_ && y < ysize_ && f < Cellsize);
        return data_[y * xsize_ * Cellsize + x * Cellsize + f];
    }

    template <typename T>
    class Grid <T , 1> {
    public :
        inline Grid();
        inline Grid(uint_t xsize , uint_t ysize);
        inline ~Grid();
        inline T& operator () (uint_t x , uint_t y);
        inline T operator () (uint_t x , uint_t y) const;

    private :
        uint_t xsize_;
        uint_t ysize_;
        T* data_;
    };

    template <typename T>
    Grid<T , 1>::~Grid ()
    {   delete[] data_;
    }

    template <typename T>
    Grid<T , 1>::Grid ()
        :xsize_ (0) , ysize_ (0) , data_ (new T[1])
    {}

    template <typename T>
    Grid<T , 1>::Grid (uint_t xsize , uint_t ysize)
        :xsize_ (xsize) , ysize_ (ysize) , data_ (new T[xsize * ysize])
    {}

    template <typename T>
    inline T& Grid<T , 1>::operator () (uint_t x , uint_t y)
    {   assert (x < xsize_ && y < ysize_);
        return data_[y * xsize_ + x];
    }

    template <typename T>
    inline T Grid<T , 1>::operator () (uint_t x , uint_t y) const
    {   assert (x < xsize_ && y < ysize_);
        return data_[y * xsize_ + x];
    }

    template <typename T>
    class Grid <T , 0>;

    template <typename T , size_t N>
    inline void swap (Grid <T , N>& a , Grid <T , N>& b)
    {   a.swap(b);
    }

    typedef Grid <real_t , 9> PDF_Field;
    typedef Grid <real_t , 3> VelocityField;
    typedef Grid <real_t , 1> DensityField;
    typedef Grid <uint_t , 1> FlagField;
}
