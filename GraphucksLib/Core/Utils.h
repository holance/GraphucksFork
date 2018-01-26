#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Graphucks
{
    template <class T> void ReleaseResource(T* t)
    {
        if (t)
        {
            t->Release();
        }
    }

    template <typename I, typename F> void for_each_map(const I& begin, const I& end, const F& f)
    {
        I scan = begin;
        while (scan != end)
        {
            f(scan->second);
            scan++;
        }
    }

    template <typename T> const std::string ToString(const T& t)
    {
        std::ostringstream stream;
        stream << t;
        return stream.str();
    }

    template <typename T> const T FromString(const std::string& s)
    {
        std::istringstream ss(s);
        T t;
        ss >> t;
        return t;
    }

    template <typename ComType> std::shared_ptr<ComType> SharedComPtr(ComType* p)
    {
        return std::shared_ptr<ComType> (p, ReleaseResource<ComType>);
    }

    template <class ErrorType>
    void Check(ErrorType result)
    {
        if (result != S_OK) 
        {
            std::string what(std::string("Returned error: ") + ToString(int(result)));
            throw std::runtime_error(what);
        }
    }

    template <class Rect, class T>
    void GrowRectangle(Rect& rect, T by)
    {
        rect.left -= by;
        rect.top -= by;
        rect.right += by;
        rect.bottom += by;
    }
}