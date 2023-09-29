#ifndef CPPLOT_FUNC_H
#define CPPLOT_FUNC_H

#include "drawing.hpp"
class Line;

double cpl_default(double x);

class Func : public Drawing {
    protected:
        // obligatory attributes:
            // dynamic:
                std::list<Line*>* _lines;
            // standard:
                double (*_func)(double);

        // optional attributes:
            // standard
                double _stroke_weight;
                double _xmin, _xmax, _ymin, _ymax;

        void assign(std::string key, std::string arg);
    public:
        Func() : Func(cpl_default, ""){};
        Func(double (*func)(double), cv::Vec3b color = {0,0,0}) : Func(func, "", color){};
        Func(double (*func)(double), std::string params, cv::Vec3b color = {0,0,0});
        ~Func();
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* posession);
};

#endif