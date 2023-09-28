#include "../headers/axis.hpp"
#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"

Axis::Axis() {
    _arrow = NULL;
    _step = 1;
    _style = CPL_FULL_TICK;
    _tick_direction = CPL_LEFT;
    _ticks = new std::list<Line*>;
    _color = {0,0,0};
}

Axis::Axis(double xa, double ya, double xb, double yb, double sw, cv::Vec3b color = {0,0,0}) {
    _arrow = new Arrow(xa, xa, xb, yb, sw, -1, color);
}