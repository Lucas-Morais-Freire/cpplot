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
        Arrow* _arrow;
        std::list<Line*>* _ticks;

    // optional attributes:
        double _tick_size, _step, _start, _stroke_weight, _min_re, _max_re, _min_img, _max_img, _head_size, _head_angle;
        bool _draw_ticks, _full_ticks, _ticks_to_left, _half_ticks, _draw_head;
    public:
        Axis(bool x_axis, double position, bool relative, cv::Vec3b color = {0,0,0});
        Axis& tickSize(double tick_size);
        Axis& step(double step);
        Axis& start(double start);
        Axis& strokeWeight(double stroke_weight);
        Axis& minRe(double min_re);
        Axis& maxRe(double max_re);
        Axis& minImg(double min_img);
        Axis& maxImg(double max_img);
        Axis& headSize(double head_size);
        Axis& headAngle(double head_angle);
        Axis& drawTicks(bool draw_ticks);
        Axis& fullTicks(bool full_ticks);
        Axis& ticksToLeft(bool ticks_to_left);
        Axis& halfTicks(bool half_ticks);
        Axis& drawHead(bool draw_head);

        ~Axis();

        void draw(Graph* G, cv::Mat* original);
        void draw(Graph* G);
};

#endif