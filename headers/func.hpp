#ifndef CPPLOT_FUNC_H
#define CPPLOT_FUNC_H

#include "drawing.hpp"
class Line;

class Func : public Drawing {
    protected:
        // obligatory attributes:
            std::function<double(double)> _func;

        // dynamic attributes:
            std::list<Line*>* _lines;

        // keyword attributes:
            double _stroke_weight;
            double _xmin, _xmax, _ymin, _ymax;

        void assign(std::string key, std::string arg);
    public:
        Func() : Func([](double x){return x;}, ""){};
        Func(double (*func)(double), cv::Vec3b color = {0,0,0}) : Func(func, "", color){};
        Func(double (*func)(double), std::string params, cv::Vec3b color = {0,0,0});

        Func(std::vector<double>& x, std::vector<double>& y, cv::Vec3b color = {0,0,0}) : Func(x, y, "", color){};
        Func(std::vector<double>& x, std::vector<double>& y, std::string params, cv::Vec3b color = {0,0,0});

        ~Func();

        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* original);
};

#endif