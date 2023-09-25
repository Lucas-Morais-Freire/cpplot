#ifndef LINE_H
#define LINE_H

#include "cpplot.hpp"
#include "drawing.hpp"

class Line : public Drawing {
    protected:
        double _xa, _ya, _xb, _yb, _sw;
    public:
        Line();
        Line(double xa, double ya, double xb, double yb, double sw, cv::Vec3b color = {0, 0, 0});
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* posession);
        void setXa(double xa);
        double xa();
        void setYa(double ya);
        double ya();
        void setXb(double xb);
        double xb();
        void setYb(double yb);
        double yb();
        void setSw(double sw);
        double sw();
        void setColor(cv::Vec3b color);
        cv::Vec3b color();
};

#endif