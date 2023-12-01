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
        void xa(double xa);
        double getxa();
        void ya(double ya);
        double getya();
        void xb(double xb);
        double getxb();
        void yb(double yb);
        double getyb();
        Line& strokeWeight(double stroke_weight);
        double getStrokeWeight();
        Line& color(cv::Vec3b color);
        cv::Vec3b getColor();
};

#endif