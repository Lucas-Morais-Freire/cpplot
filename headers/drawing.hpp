#ifndef CPPLOT_DRAWING_H
#define CPPLOT_DRAWING_H

#include <opencv2/opencv.hpp>

class Graph;

class Drawing {
    protected:
        cv::Vec3b _color;
    public:
        virtual void draw(Graph* G) = 0;
        virtual ~Drawing() = default;
};

#endif