#ifndef CPPLOT_LINE_H
#define CPPLOT_LINE_H

#include "drawing.hpp"

class Line : public Drawing {
    protected:
        // obligatory attributes:
            double _xa, _ya, _xb, _yb;
        
        // optional attributes:
            double _stroke_weight;
    public:
        Line(double xa, double ya, double xb, double yb, cv::Vec3b color = {0, 0, 0});
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* original);
        void setXa(double xa);
        double xa();
        void setYa(double ya);
        double ya();
        void setXb(double xb);
        double xb();
        void setYb(double yb);
        double yb();
        Line& setStrokeWeight(double stroke_weight);
        double strokeWeight();
        Line& setColor(cv::Vec3b color);
        cv::Vec3b color();
};

#endif