#include "../headers/arrow.hpp"
#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"
#include <iostream>
#include <regex>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Arrow::Arrow(double xa, double ya, double xb, double yb, cv::Vec3b color) {
    // initialize obligatory values:
        _xa = xa; _ya = ya; _xb = xb; _yb = yb;
        _color = color;
    // initialize dynamic values:
        _lBranch = NULL;
        _rBranch = NULL;
        _stem = new Line(_xa, _ya, _xb, _yb, _color);
    //initialize optional values:
        _angle = M_PI/9;
        _head_size = 20;
}

Arrow::~Arrow() {
    delete _stem;
    delete _rBranch;
    delete _lBranch;
}

Arrow& Arrow::setHeadSize(double head_size) {
    if (head_size < 0.5) {
        _head_size = 0.0;
    } else {
        _head_size = head_size;
    }

    return *this;
}

Arrow& Arrow::setAngle(double angle) {
    _angle = M_PI/180.0*angle;
    
    return *this;
}

void Arrow::draw(Graph* G) {
    cv::Mat* original = new cv::Mat;
	this->draw(G, original);
    delete original;
}

Arrow& Arrow::setStrokeWeight(double stroke_weight) {
    _stem->setStrokeWeight(stroke_weight);

    return *this;
}

void Arrow::draw(Graph* G, cv::Mat* original) {
    cv::Mat* canvas = G->getCanvas();

    if (original->data == NULL) {
        cv::cvtColor(*canvas, *original, cv::COLOR_BGR2BGRA);
    } else if (original->rows != canvas->rows || original->cols != canvas->cols) {
        std::cout << "Graph resolution and original matrix dimensions don't match!\n";
        return;
    } else if (original->type() != CV_8UC4) {
        std::cout << "matrix type not compatible. it should be CV_8UC4\n";
        return;
    }

    _stem->draw(G, original);

    // head size is measured in pixels and the angle must be absolute. therefore the scale of the graph should not matter to how the arrow
    // is drawn. We must find where the endpoints of the head lines will sit in matrix space before converting back to R2 space.
    
    double S = sin(_angle); double C = cos(_angle);

    // x coordinates in matrix space depend on the y coordinate in R2.
    double xa = G->iIdx(_stem->ya());
    double xb = G->iIdx(_stem->yb());
    // y coordinates in matrix space depend on the x coordinate in R2.
    double ya = G->jIdx(_stem->xa());
    double yb = G->jIdx(_stem->xb());
    // now we find where the head lines' endpoints will sit in matrix space:
    // normalize the size with respect to arrow size.
    double hsize = _head_size/sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
    
    if (_head_size > 0.5) {
        delete _lBranch;
        _lBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xa - xb)*S)), G->ypos(xb + hsize*((xa - xb)*C + (yb - ya)*S)), _stem->color());
        _lBranch->setStrokeWeight(_stem->strokeWeight());
        _lBranch->draw(G, original);
        
        delete _rBranch;
        _rBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xb - xa)*S)), G->ypos(xb + hsize*((xa - xb)*C + (ya - yb)*S)), _stem->color());
        _rBranch->setStrokeWeight(_stem->strokeWeight());
        _rBranch->draw(G, original);
    }
}