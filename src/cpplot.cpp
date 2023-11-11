#include "../headers/cpplot.hpp"
#include <iostream>

Graph::Graph() {
    // first, those variables who are keyword arguments:  
    _xres = 854; _yres = 480;
    // then, the proper variables:
    _canvas = new cv::Mat(_yres, _xres, CV_8UC3);
    _xmin = -5; _xmax = 5;
    _ymin = -5; _ymax = 5;
    _bgColor = {255, 255, 255};
    _drawOrder = new std::list<Drawing*>;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
}

Graph::Graph(double xmin, double xmax, double ymin, double ymax, cv::Vec3b bgColor) {
    // first, those variables who are keyword arguments:  
    _xres = 854; _yres = 480;
    // then, the proper variables:
    _canvas = new cv::Mat(480, 854, CV_8UC3);
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

Graph& Graph::setRes(int xres, int yres) {
    if (xres < 0 || yres < 0) {
        std::cout << "negative resolution values are not allowed\n";
        exit(-1);
    }
    _xres = xres; _yres = yres;
    delete _canvas;
    _canvas = new cv::Mat(_yres, _xres, CV_8UC3);

    return *this;
}

int Graph::xres() {
    return _canvas->cols;
}

Graph& Graph::setXres(int xres) {
    if (xres < 0) {
        std::cout << "negative resolution values are not allowed\n";
        exit(-1);
    }
    _xres = xres;
    delete _canvas;
    _canvas = new cv::Mat(_yres, _xres, CV_8UC3);

    return *this;
}

int Graph::yres() {
    return _canvas->rows;
}

Graph& Graph::setYres(int yres) {
    if (yres < 0) {
        std::cout << "negative resolution values are not allowed\n";
        exit(-1);
    }
    _yres = yres;
    delete _canvas;
    _canvas = new cv::Mat(_yres, _yres, CV_8UC3);

    return *this;
}

Graph& Graph::setRange(double xmin, double xmax, double ymin, double ymax) {
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;

    return *this;
}


double Graph::xmin() {
    return _xmin;
}

Graph& Graph::setXmin(int xmin) {
    if (_xmax <= xmin) {
        std::cout << "Graph xmin can't be equal or greater than xmax.\n";
        exit(-1);
    }

    _xmin = xmin;

    return *this;
}

double Graph::xmax() {
    return _xmax;
}

Graph& Graph::setXmax(int xmax) {
    if (_xmin >= xmax) {
        std::cout << "Graph xmax can't be equal or smaller than xmin.\n";
        exit(-1);
    }

    _xmax = xmax;

    return *this;
}

Graph& Graph::setXbounds(int xmin, int xmax) {
    if (xmin >= xmax) {
        std::cout << "Graph xmax can't be equal or smaller than xmin.\n";
        exit(-1);
    }

    _xmin = xmin; _xmax = xmax;

    return *this;
}

double Graph::ymin() {
    return _ymin;
}

Graph& Graph::setYmin(int ymin) {
    if (_ymax <= ymin) {
        std::cout << "Graph ymin can't be equal or greater than ymax.\n";
        exit(-1);
    }

    _ymin = ymin;

    return *this;
}

double Graph::ymax() {
    return _ymax;
}

Graph& Graph::setYmax(int ymax) {
    if (_ymin >= ymax) {
        std::cout << "Graph ymax can't be equal or smaller than ymin.\n";
        exit(-1);
    }

    _ymax = ymax;

    return *this;
}

Graph& Graph::setYbounds(int ymin, int ymax) {
    if (ymin >= ymax) {
        std::cout << "Graph ymax can't be equal or smaller than ymin.\n";
        exit(-1);
    }

    _ymin = ymin; _ymax = ymax;

    return *this;
}

cv::Vec3b Graph::bgColor() {
    return _bgColor;
}

Graph& Graph::setBGColor(cv::Vec3b bgColor) {
    _bgColor = bgColor;

    return *this;
}

cv::Mat* Graph::getCanvas() {
    return _canvas;
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

Line& Graph::drawLine(double xa, double ya, double xb, double yb, cv::Vec3b color) {
    Line* newLine = new Line(xa, ya, xb, yb, color);
    _drawOrder->push_back(newLine);
    return *newLine;
}

Arrow& Graph::drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color) {
    Arrow* newArrow = new Arrow(xa, ya, xb, yb, color);
    _drawOrder->push_back(newArrow);
    return *newArrow;
}

Func& Graph::drawFunc(double (*func)(double), cv::Vec3b color) {
    Func* newFunc = new Func(func, color);
    _drawOrder->push_back(newFunc);
    return *newFunc;
}
Func& Graph::drawFunc(std::vector<double>& x, std::vector<double>& y, cv::Vec3b color) {
    Func* newFunc = new Func(x, y, color);
    _drawOrder->push_back(newFunc);
    return *newFunc;
}

Axis& Graph::drawAxis(bool x_axis, double position, bool relative, cv::Vec3b color) {
    Axis* newAxis = new Axis(x_axis, position, relative, color);
    _drawOrder->push_back(newAxis);
    return *newAxis;
}

void Graph::write(const char* filename) {
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
    for (std::list<Drawing*>::iterator iter = _drawOrder->begin(); iter != _drawOrder->end(); iter++) {
        (*iter)->draw(this);
    }
    cv::imwrite(filename, (*_canvas));
}