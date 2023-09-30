#ifndef CPPLOT_ARROW_H
#define CPPLOT_ARROW_H

#include "drawing.hpp"
class Line;

class Arrow : public Drawing {
    protected:        
        //obligatory attributes:
			double _xa, _ya, _xb, _yb;

        // dynamic attributes:
			// keyword-independant:
				Line* _rBranch;
				Line* _lBranch;
			// keyword-dependant
				Line* _stem;
        
        // keyword attributes:
			double _head_size, _angle;
        
        void assign(std::string key, std::string arg);
    public:
        Arrow() : Arrow(0, 0, 0, 0) {};
        Arrow(double xa, double ya, double xb, double yb, cv::Vec3b color = {0, 0, 0}) : Arrow(xa, ya, xb, yb, "", color) {};
        Arrow(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0, 0, 0});
        ~Arrow();
        void draw(Graph* G);
        void draw(Graph* G, cv::Mat* posession);
};

#endif