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

    double x1, x2, f1, f2;
    Line* newLine;
    int j = (int)(G->jIdx(xmin) + 0.5);
    while (j <= (int)(G->jIdx(xmax) + 0.5) - 1) {
        x1 = G->xpos(j);
        x2 = G->xpos(++j);
        f1 = (*_func)(x1);
        f2 = (*_func)(x2);
        if (ymin <= f1 && f1 <= ymax && ymin <= f2 && f2 <= ymax) {
            newLine = new Line(x1, f1, x2, f2, _sw, _color);
            _lines->push_back(newLine);
        } else if (ymin > f1 && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, x2, f2, _sw, _color);
            _lines->push_back(newLine);
        } else if (f1 > ymax && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, x2, f2, _sw, _color);
            _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && ymin > f2) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, _sw, _color);
            _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && f2 > ymax) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, _sw, _color);
            _lines->push_back(newLine);
        } else if (ymin > f1 && f2 > ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, _sw, _color);
            _lines->push_back(newLine);
        } else if (f1 > ymax && ymin > f2) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, _sw, _color);
            _lines->push_back(newLine);
        }
    }

    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        (*iter)->draw(G, posession);
    }
}