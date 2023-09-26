#include "../headers/func.hpp"
#include "../headers/cpplot.hpp"

double cpplot_default(double x) {
    return 0;
}

Func::Func() {
    _func = cpplot_default;
    _xmin = _ymin = -HUGE_VAL;
    _xmax = _ymax = HUGE_VAL;
    _sw = 0;
    _color = {0,0,0};
    _lines = new std::list<Line*>;
}

Func::Func(double (*func)(double), double xmin, double xmax, double ymin, double ymax, double sw, cv::Vec3b color) {
    _func = func;
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;
    _sw = sw;
    _color = color;
    _lines = new std::list<Line*>;
}

Func::Func(double (*func)(double), double xmin, double xmax, double sw, cv::Vec3b color) {
    _func = func;
    _xmin = xmin; _xmax = xmax;
    _ymin = -HUGE_VAL; _ymax = HUGE_VAL;
    _sw = sw;
    _color = color;
    _lines = new std::list<Line*>;
}

Func::Func(double (*func)(double), double sw, cv::Vec3b color) {
    _func = func;
    _xmin = _ymin = -HUGE_VAL;
    _xmax = _ymax = HUGE_VAL;
    _sw = sw;
    _color = color;
    _lines = new std::list<Line*>;
}

Func::~Func() {
    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        delete (*iter);
    }
    _lines->clear();
    delete _lines;
}

void Func::draw(Graph* G) {
    cv::Mat* posession = new cv::Mat;
	this->draw(G, posession);
    delete posession;
}

void Func::draw(Graph* G, cv::Mat* posession) {
    if (posession->data == NULL) {
        posession = new cv::Mat(G->yres(), G->xres(), CV_8U);
        posession->setTo(0);
    } else if (posession->rows != G->yres() || posession->cols != G->xres()) {
        std::cout << "Graph resolution and matrix dimensions don't match!\n";
        return;
    } else if (posession->type() != CV_8U) {
        std::cout << "matrix type not compatible. it should be CV_8U\n";
        return;
    }

    double xmin = G->xmin() >= _xmin ? G->xmin() : _xmin;
    double ymin = G->ymin() >= _ymin ? G->ymin() : _ymin;
    double xmax = G->xmax() <= _xmax ? G->xmax() : _xmax;
    double ymax = G->ymax() <= _ymax ? G->ymax() : _ymax;

    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        delete (*iter);
    }
    _lines->clear();


    double step = G->xpos(1);
    double f1, f2;
    Line* newLine;
    for (double i = xmin; i <= xmax - step; i += step) {
        f1 = (*_func)(i);
        f2 = (*_func)(i + step);
        if ((ymin <= f1 <= ymax && ymin <= f2 <= ymax) || ((ymax < f1 || f1 < ymin) && ymin <= f2 <= ymax) || (ymin <= f1 <= ymax && (ymax < f2 || f2 < ymin))) {
            newLine = new Line(i, f1, i + step, f2, _sw, _color);
            _lines->push_back(newLine);
        }
    }

    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        (*iter)->draw(G, posession);
    }
}