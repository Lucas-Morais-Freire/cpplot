#ifndef CPPLOT_AXIS
#define CPPLOT_AXIS

#include "drawing.hpp"
class Line;
class Arrow;

class Axis : public Drawing {
    protected:
    // obligatory attributes:
        double _xa, _ya, _xb, _yb;

    // dynamic attributes:
        // keyword-dependant:
            Arrow* _arrow;
        // keyword-independant:
            std::list<Line*>* _ticks;

    // keyword attributes:
        // all:
            double _tick_size, _step, _start, _stroke_weight, _xmin, _ymin, _xmax, _ymax, _head_size, _head_angle;
            bool _full_ticks, _draw_head, _ticks_to_left, _half_ticks;
        // pattern 2:
            unsigned char _horizontal_placement, _vertical_placement;

    void assign(std::string key, std::string arg);
    public:
        Axis() : Axis(true) {};
        // pattern 1:
        Axis(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0}) : Axis(xa, ya, xb, yb, "", color) {};
        Axis(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0,0,0});

        // pattern 2:
        Axis(bool x_axis, cv::Vec3b color = {0,0,0}) : Axis(x_axis, "", color) {};
        Axis(bool x_axis, std::string params, cv::Vec3b color = {0,0,0});

        // pattern 3:
        Axis(bool x_axis, double position, cv::Vec3b color = {0,0,0}) : Axis(x_axis, position, "", color) {};
        Axis(bool x_axis, double position, std::string params, cv::Vec3b color = {0,0,0});

        // pattern 4:
        Axis(double x, double y, double angle, cv::Vec3b color = {0,0,0}) : Axis(x, y, angle, "", color) {};
        Axis(double x, double y, double angle, std::string params, cv::Vec3b color = {0,0,0});

        ~Axis();

        void draw(Graph* G, cv::Mat* posession);
        void draw(Graph* G);
};

#endif