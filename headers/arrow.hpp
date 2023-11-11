#ifndef CPPLOT_ARROW_H
#define CPPLOT_ARROW_H

#include "drawing.hpp"
class Line;

class Arrow : public Drawing {
    protected:        
        //obligatory attributes:
			double _xa, _ya, _xb, _yb;

        // working attributes:
				Line* _rBranch;
				Line* _lBranch;
				Line* _stem;
        
        // optional attributes:
			double _head_size, _angle;
    public:
        Arrow(double xa, double ya, double xb, double yb, cv::Vec3b color = {0, 0, 0});
        ~Arrow();
        Arrow& setHeadSize(double head_size);
        Arrow& setAngle(double angle);
        Arrow& setStrokeWeight(double stroke_weight);
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* original);
};

#endif