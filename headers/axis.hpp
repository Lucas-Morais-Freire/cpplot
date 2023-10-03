#ifndef CPPLOT_AXIS
#define CPPLOT_AXIS

#include "drawing.hpp"
class Line;
class Arrow;

class Axis : public Drawing {
    protected:
    // obligatory attributes:
        bool _x_axis;
        double _position;
        bool _relative;

    // dynamic attributes:
        // keyword-dependant:
            Arrow* _arrow;
        // keyword-independant:
            std::list<Line*>* _ticks;

    // keyword attributes:
            double _tick_size, _step, _start, _stroke_weight, _min_re, _max_re, _min_img, _max_img, _head_size, _head_angle;
            bool _draw_ticks, _full_ticks, _ticks_to_left, _half_ticks, _draw_head;

    void assign(std::string key, std::string arg);
    public:
        Axis() : Axis(true, 0, true) {};

        Axis(bool x_axis, double position, bool relative, cv::Vec3b color = {0,0,0}) : Axis(x_axis, position, relative, "", color) {};
        Axis(bool x_axis, double position, bool relative, std::string params, cv::Vec3b color = {0,0,0});

        ~Axis();

        void draw(Graph* G, cv::Mat* posession);
        void draw(Graph* G);
};

#endif