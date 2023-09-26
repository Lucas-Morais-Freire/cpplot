#ifndef CPPLOT_FUNC_H
#define CPPLOT_FUNC_H

#include "drawing.hpp"
class Line;

class Func : public Drawing {
    protected:
        double (*_func)(double);
        double _xmin, _xmax, _ymin, _ymax;
        double _sw;
        std::list<Line*>* _lines;
    public:
        Func();
        Func(double (*func)(double), double xmin, double xmax, double ymin, double ymax, double sw, cv::Vec3b color = {0,0,0});
        Func(double (*func)(double), double xmin, double xmax, double sw, cv::Vec3b color = {0,0,0});
        Func(double (*func)(double), double sw, cv::Vec3b color = {0,0,0});
        ~Func();
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* posession);
};

#endif