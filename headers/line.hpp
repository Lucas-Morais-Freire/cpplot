#ifndef CPPLOT_LINE_H
#define CPPLOT_LINE_H

#include "drawing.hpp"

class Line : public Drawing {
    protected:
        // obligatory attributes:
            // standard:
                double _xa, _ya, _xb, _yb;
        
        // keyword attributes:
            // standard:
                double _stroke_weight;
        
        void assign(std::string key, std::string arg);
    public:
        Line() : Line(0, 0, 0, 0) {};
        Line(double xa, double ya, double xb, double yb, cv::Vec3b color = {0, 0, 0}) : Line(xa, ya, xb, yb, "", color) {};
        Line(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0, 0, 0});
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
        void setStrokeWeight(double stroke_weight);
        double strokeWeight();
        void setColor(cv::Vec3b color);
        cv::Vec3b color();
};

#endif