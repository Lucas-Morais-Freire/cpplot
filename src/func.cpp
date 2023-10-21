#include "../headers/func.hpp"
#include "../headers/cpplot.hpp"

Func::Func(double (*func)(double), cv::Vec3b color) {
    // initialize obligatory values:
        _func = func;
        _color = color;

    // initialize working variables:
        _lines = new std::list<Line*>;
    
    // initialize optional values:
        _stroke_weight = 2;
        _xmin = -HUGE_VAL;
        _xmax = HUGE_VAL;
        _ymin = -HUGE_VAL;
        _ymax = HUGE_VAL;
}

Func::Func(std::vector<double>& x, std::vector<double>& y, cv::Vec3b color) {
    // initialize obligatory values:
        if (x.size() < 2) {
            std::cout << "input vector should have at least size 2.\n";
            exit(1);
        }

        if (x.size() != y.size()) {
            std::cout << "input vectors x and y must have the same size.\n";
            exit(-1);
        }

        std::vector<double>::iterator iterx = x.begin();
        std::vector<double>::iterator iterx_prev = iterx;
        while (iterx != x.end()) {
            if ((*iterx) < (*iterx_prev)) {
                std::cout << "input x vector should be ordered.\n";
                exit(-1);
            }
            iterx_prev = iterx;
            iterx++;
        }

        _func = [&x, &y](double t) {
            std::vector<double>::iterator xiter = x.begin();
            std::vector<double>::iterator xiter_prev = xiter;
            xiter++;
            std::vector<double>::iterator yiter = y.begin();
            std::vector<double>::iterator yiter_prev = yiter;
            yiter++;
            while (xiter != x.end()) {
                if (t <= (*xiter)) {
                    return ((*yiter) - (*yiter_prev))/((*xiter) - (*xiter_prev))*(t - (*xiter_prev)) + (*yiter_prev);
                }

                xiter_prev = xiter;
                xiter++;
                yiter_prev = yiter;
                yiter++;
            }

            return 0.0;
        };
        _color = color;

    // initialize working variables:
        _lines = new std::list<Line*>;
    
    // initialize optional values:
        _stroke_weight = 2;
        _xmin = *(x.begin());
        _xmax = *(--(x.end()));
        _ymin = -HUGE_VAL;
        _ymax = HUGE_VAL;
}

Func::~Func() {
    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        delete (*iter);
    }
    _lines->clear();
    delete _lines;
}

Func& Func::setStrokeWeight(double stroke_weight) {
    _stroke_weight = stroke_weight;

    return *this;    
}

Func& Func::setXmin(double xmin) {
    _xmin = xmin;

    return *this;    
}

Func& Func::setXmax(double xmax) {
    _xmax = xmax;

    return *this;    
}

Func& Func::setYmin(double ymin) {
    _ymin = ymin;

    return *this;    
}

Func& Func::setYmax(double ymax) {
    _ymax = ymax;

    return *this;    
}


void Func::draw(Graph* G) {
    cv::Mat* original = new cv::Mat;
	this->draw(G, original);
    delete original;
}

void Func::draw(Graph* G, cv::Mat* original) {
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
        f1 = _func(x1);
        f2 = _func(x2);
        if (ymin <= f1 && f1 <= ymax && ymin <= f2 && f2 <= ymax) {
            newLine = new Line(x1, f1, x2, f2, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (ymin > f1 && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, x2, f2, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (f1 > ymax && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, x2, f2, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && ymin > f2) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && f2 > ymax) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (ymin > f1 && f2 > ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        } else if (f1 > ymax && ymin > f2) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, _color);
            newLine->setStrokeWeight(_stroke_weight);
        _lines->push_back(newLine);
        }
    }

    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        (*iter)->draw(G, original);
    }
}