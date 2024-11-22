#include <iostream>
#include <cassert>

template <typename T>
class Grid final
{
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T *data;
    size_type y_size, x_size;

    Grid(T *data, size_type y_size, size_type x_size): data(data), y_size(y_size), x_size(x_size) {}

    //Grid(Grid<T>&&) = delete;
    //Grid<T>& operator=(Grid<T>&&) = delete;

public:

    Grid<T>(Grid<T> const &scr) // задание 2 копирующий конструктор
    {
        y_size = scr.y_size;
        x_size = scr.x_size;

        data = reinterpret_cast<T*>(operator new(sizeof(T)*scr.y_size*scr.x_size));
        for(size_type y = 0; y < y_size; y++)
            for(size_type x = 0; x < x_size; x++)
                new (data + y*x_size + x) T(scr[y][x]);
    }


    Grid<T>& operator= (Grid<T> const &scr) //задание 2 оператор копирующего присваивания
    {
        T *new_data = reinterpret_cast<T*>(operator new(sizeof(T)*scr.y_size*scr.x_size));
        for(size_type y = 0; y < scr.y_size; y++)
            for(size_type x = 0; x < scr.x_size; x++)
                new (new_data + y*x_size + x) T(scr[y][x]);
        delete [] data;
        data = new_data;
        y_size = scr.y_size;
        x_size = scr.x_size;
        return *this;

    }


    Grid<T>(T const &t): y_size(1), x_size(1), data(reinterpret_cast<T*>(operator new(sizeof(T)))) // задание 1.1
    {
        new (data) T(t);
    }


    /*
    Grid<T>(size_type y_size, size_type x_size): y_size(y_size), x_size(x_size), data(new T[y_size*x_size)) // задание 1.2 (другой вариант)
    {
        for(size_type i=0; i < y_size*x_size; ++i)
        {
            *(data+i) = T();
        }
    }
    */

    Grid<T>(size_type y_size, size_type x_size): y_size(y_size), x_size(x_size), data(reinterpret_cast<T*>(operator new(sizeof(T)*y_size*x_size))) // задание 1.2
    {
        for(size_type i=0; i < y_size*x_size; ++i)
        {
            new (data+i) T();
        }
    }

    Grid<T>(size_type y_size, size_type x_size, T const &t): y_size(y_size), x_size(x_size), data(reinterpret_cast<T*>(operator new(sizeof(T)*y_size*x_size))) // задание 1.3
    {
        for(size_type i=0; i < y_size*x_size; ++i)
        {
            new (data+i) T(t);
        }
    }

    T operator()(size_type y_idx, size_type x_idx) const
    {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx)
    {
        return data[y_idx * x_size + x_idx];
    }

    Grid<T>& operator=(T const &t)
    {
        for(auto it = data, end = data + x_size * y_size; it != end; ++it)
        {
            *it = t;
        }
        return *this;
    }

    T * operator[](size_type y_idx) const // задание 2
    {
        if(y_idx>=0 && y_idx<y_size)
            return data + x_size*y_idx;
        else
            std::cout<<"Error: Out of range"<<std::endl;
            return NULL;
    }

    size_type get_y_size() const
    {
        return y_size;
    }

    size_type get_x_size() const
    {
         return x_size;
    }

    ~Grid<T>()
    {
        for(size_type i = 0; i < y_size*x_size; ++i)
        {
            (*(data+i)).~T();
        }
        delete [] data;
    }
};

int main()
{
    Grid<float> g(3, 2, 0.0f);

    Grid<float> m(g);

    Grid<float> k(2, 2, 0.12324f);

    m=k;

    std::cout<<m[1][1]<<std::endl;

    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<float>::size_type;

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    std::cout<<g[1][1]<<std::endl;

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    std::cout<<g[1][1]<<std::endl;

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));

    std::cout<<g[1][1]<<std::endl;


    return 0;
}
