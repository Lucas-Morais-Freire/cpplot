#ifndef CPPLOT_ARROW_H
#define CPPLOT_ARROW_H

#include "drawing.hpp"
class Line;

class Arrow : public Drawing {
    protected:
        double _hsize, _angle;
        Line* _stem;
        Line* _rBranch;
        Line* _lBranch;
    public:
        Arrow();
        Arrow(double xa, double ya, double xb, double yb, double sw, double hsize, double angle = 20, cv::Vec3b color = {0, 0, 0});
        Arrow(double xa, double ya, double xb, double yb, double sw, double hsize, cv::Vec3b color = {0, 0, 0});
        ~Arrow();
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* posession);
};

#endif