#ifndef CPPLOT_AXIS
#define CPPLOT_AXIS

#include "drawing.hpp"
class Line;
class Arrow;

#define CPL_X true
#define CPL_Y false

#define CPL_FULL_TICK true
#define CPL_HALF_TICK true

#define CPL_LEFT true
#define CPL_RIGHT false

class Axis : public Drawing {
    protected:
        Arrow* _arrow;
        double _step;
        bool _style;
        bool _tick_direction;
        std::list<Line*>* _ticks;
    public:
        Axis();

        Axis(double xa, double ya, double xb, double yb, double sw, cv::Vec3b color = {0,0,0}) : Axis() {};

        Axis(double xa, double ya, double xb, double yb, double step, double sw, cv::Vec3b color = {0,0,0});

        Axis(double xa, double ya, double xb, double yb, double step, bool style, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double hsize, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double hsize, double angle, double sw, cv::Vec3b color = {0,0,0});
        
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double hsize, double sw, cv::Vec3b color = {0,0,0});
        Axis(double xa, double ya, double xb, double yb, double step, bool style, double ticksize, bool tick_direction, double hsize, double angle, double sw, cv::Vec3b color = {0,0,0});

        Axis(bool axis, double step, double position, bool style, bool tick_direction, double ticksize, double hsize, double angle, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, bool tick_direction, double ticksize, double hsize, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, bool tick_direction, double ticksize, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, bool tick_direction, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, double ticksize, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, double hsize, double angle, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, double hsize, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, bool style, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double step, double position, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double position, double sw, cv::Vec3b color = {0,0,0});
        Axis(bool axis, double sw, cv::Vec3b color = {0,0,0});
        ~Axis();
};

#endif