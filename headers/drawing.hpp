#ifndef DRAWING_H
#define DRAWING_H

#include "cpplot.hpp"

class Graph;

class Drawing {
    protected:
        cv::Vec3b _color;
    public:
        virtual void draw(Graph* G) = 0;
        virtual ~Drawing() = default;
};

#endif