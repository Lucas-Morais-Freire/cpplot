#ifndef CPPLOT_FUNC_H
#define CPPLOT_FUNC_H

#include "drawing.hpp"
class Line;

class Func : public Drawing {
    protected:
        // obligatory attributes:
            std::function<double(double)> _func;

        // working attributes:
            std::list<Line*>* _lines;

        // optional attributes:
            double _stroke_weight;
            double _xmin, _xmax, _ymin, _ymax;
    public:
        Func(double (*func)(double), cv::Vec3b color = {0,0,0});
        Func(std::vector<double>& x, std::vector<double>& y, cv::Vec3b color = {0,0,0});
        ~Func();
        Func& setStrokeWeight(double stroke_weight);
        Func& setXmin(double xmin);
        Func& setXmax(double xmax);
        Func& setYmin(double ymin);
        Func& setYmax(double ymax);
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* original);
};

#endif