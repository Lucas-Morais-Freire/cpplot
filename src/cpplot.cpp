#include "../headers/cpplot.hpp"
#include <iostream>

Graph::Graph() {
    _canvas = new cv::Mat(854, 480, CV_8UC3);
    _xmin = -5; _xmax = 5;
    _ymin = -5; _ymax = 5;
    _bgColor = {255, 255, 255};
    _drawOrder = new std::list<Drawing*>;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
}

Graph::Graph(double xmin, double xmax, double ymin, double ymax, uint xres, uint yres, cv::Vec3b bgColor) {
    _canvas = new cv::Mat(yres, xres, CV_8UC3);
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;
    _bgColor = bgColor;
    _drawOrder = new std::list<Drawing*>;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
}

Graph::Graph(double xmin, double xmax, double ymin, double ymax, cv::Vec3b bgColor) {
    _canvas = new cv::Mat(854, 480, CV_8UC3);
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;
    _bgColor = bgColor;
    _drawOrder = new std::list<Drawing*>;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
}

Graph::~Graph() {
    for (std::list<Drawing*>::iterator iter = _drawOrder->begin(); iter != _drawOrder->end(); iter++) {
        delete (*iter);
    }
    _drawOrder->clear();
    delete _drawOrder;
    delete _canvas;
}

void Graph::setRes(uint xres, uint yres) {
    delete _canvas;
    _canvas = new cv::Mat(yres, xres, CV_8UC3);
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
    for (std::list<Drawing*>::iterator iter = _drawOrder->begin(); iter != _drawOrder->end(); iter++) {
        (*iter)->draw(this);
    }
}

int Graph::xres() {
    return _canvas->cols;
}

int Graph::yres() {
    return _canvas->rows;
}

void Graph::setRange(double xmin, double xmax, double ymin, double ymax) {
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
    for (std::list<Drawing*>::iterator iter = _drawOrder->begin(); iter != _drawOrder->end(); iter++) {
        (*iter)->draw(this);
    }
}


double Graph::xmin() {
    return _xmin;
}

double Graph::xmax() {
    return _xmax;
}

double Graph::ymin() {
    return _ymin;
}

double Graph::ymax() {
    return _ymax;
}

void Graph::setBGColor(cv::Vec3b bgColor) {
    _bgColor = bgColor;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
    for (std::list<Drawing*>::iterator iter = _drawOrder->begin(); iter != _drawOrder->end(); iter++) {
        (*iter)->draw(this);
    }
}

cv::Vec3b Graph::bgColor() {
    return _bgColor;
}

cv::Vec3b* Graph::at(int i, int j) {
    return &(_canvas->at<cv::Vec3b>(i, j));
}

double Graph::iIdx(double y) {
    return _canvas->rows - 1 - (_canvas->rows - 1)/(_ymax - _ymin)*(y - _ymin);
}

double Graph::jIdx(double x) {
    return (_canvas->cols - 1)/(_xmax - _xmin)*(x - _xmin);
}

double Graph::xpos(double j) {
    return (_xmax - _xmin)/(_canvas->cols - 1)*j + _xmin;
}

double Graph::ypos(double i) {
    return -(_ymax - _ymin)/(_canvas->rows - 1)*(i - _canvas->rows + 1) + _ymin;
}

void Graph::drawLine(double xa, double ya, double xb, double yb, cv::Vec3b color, double sw) {
    Line* newLine = new Line(xa, ya, xb, yb, sw, color);
    newLine->draw(this);
    _drawOrder->push_back(newLine);
}

void Graph::drawLine(double xa, double ya, double xb, double yb, double sw) {
    this->drawLine(xa, ya, xb, yb, {0,0,0}, sw);
}

void Graph::drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color, double sw, double hsize, double angle) {
    Arrow* newArrow = new Arrow(xa, ya, xb, yb, sw, hsize, color);
    newArrow->draw(this);
    _drawOrder->push_back(newArrow);
}

void Graph::drawArrow(double xa, double ya, double xb, double yb, double sw, double hsize, double angle) {
    this->drawArrow(xa, ya, xb, yb, {0, 0, 0}, sw, hsize, angle);
}

void Graph::drawFunc(double (*func)(double), double xmin, double xmax, double ymin, double ymax, double sw, cv::Vec3b color) {
    Func* newFunc = new Func(func, xmin, xmax,  ymin,  ymax,  sw, color);
    newFunc->draw(this);
    _drawOrder->push_back(newFunc);
}

void Graph::drawFunc(double (*func)(double), double xmin, double xmax, double sw, cv::Vec3b color) {
    Func* newFunc = new Func(func,  xmin,  xmax,  sw, color);
    newFunc->draw(this);
    _drawOrder->push_back(newFunc);
}

void Graph::drawFunc(double (*func)(double), double sw, cv::Vec3b color) {
    Func* newFunc = new Func(func,  sw, color);
    newFunc->draw(this);
    _drawOrder->push_back(newFunc);
}

void Graph::write(const char* filename) {
    cv::imwrite(filename, (*_canvas));
}