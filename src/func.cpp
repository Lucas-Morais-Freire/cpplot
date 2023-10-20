#include "../headers/func.hpp"
#include "../headers/cpplot.hpp"
#include <regex>
#include <algorithm>

void Func::assign(std::string key, std::string arg) {
    std::regex double_pattern("(0|\\-?(0|[1-9][0-9]*)(\\.[0-9]*)?|\\-?\\.[0-9]+)");
    if (std::regex_match(arg, double_pattern)) {
        if (key == "stroke_weight") {
            if (std::stod(arg) < 0) {
                std::cout << "stroke_weight can't be a negative value.\n";
                return;
            }
            _stroke_weight = std::stod(arg);
            return;
        } else if (key == "xmin") {
            _xmin = std::stod(arg);
            return;
        } else if (key == "xmax") {
            _xmax = std::stod(arg);
            return;
        } else if (key == "ymin") {
            _ymin = std::stod(arg);
            return;
        } else if (key == "ymax") {
            _ymax = std::stod(arg);
            return;
        }
    } else {
		std::cout << "argument \"" << arg << "\" is not a valid double literal.\n";
        return;
	}
}

Func::Func(double (*func)(double), std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        _func = func;
        _color = color;

    // initialize dynamic values:
        _lines = new std::list<Line*>;
    
    // Func has these optional parameters:
    _keys = {
        "stroke_weight",
        "xmin",
        "xmax",
        "ymin",
        "ymax"
    };
    // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
	for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "stroke_weight") {
            _stroke_weight = 2;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        }
	}
	delete keys;
}

Func::Func(std::vector<double>& x, std::vector<double>& y, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        std::vector<double>::iterator iterx = x.begin();
        std::vector<double>::iterator iterx_prev = iterx;
        if ((*iterx) < (*iterx_prev)) {
            std::cout << "input x vector should be ordered.\n";
            exit(1);
        }
        if (x.size() != y.size()) {
            std::cout << "input vectors x and y must have the same size.\n";
            exit(1);
        }
        if (x.size() < 2) {
            std::cout << "input vector x should have at least size 2.\n";
            exit(1);
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

                xiter++;
                xiter_prev++;
                yiter++;
                yiter_prev++;
            }
            return t;
        };
        _color = color;

    // initialize dynamic values:
        _lines = new std::list<Line*>;
    
    // Func has these optional parameters:
    _keys = {
        "stroke_weight",
        "xmin",
        "xmax",
        "ymin",
        "ymax"
    };
    // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
	for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "stroke_weight") {
            _stroke_weight = 2;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        }
	}
	delete keys;

    _xmin = _xmin < (*(x.begin())) ? (*(x.begin())) : _xmin;
    _xmax = _xmax > (*(--(x.end()))) ? (*(--(x.end()))) : _xmax;
}

Func::~Func() {
    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        delete (*iter);
    }
    _lines->clear();
    delete _lines;
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
            newLine = new Line(x1, f1, x2, f2, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (ymin > f1 && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, x2, f2, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (f1 > ymax && ymin <= f2 && f2 <= ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, x2, f2, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && ymin > f2) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (ymin <= f1 && f1 <= ymax && f2 > ymax) {
            newLine = new Line(x1, f1, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (ymin > f1 && f2 > ymax) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, (x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        } else if (f1 > ymax && ymin > f2) {
            newLine = new Line((x2 - x1)/(f2 - f1)*(ymax - f1) + x1, ymax, (x2 - x1)/(f2 - f1)*(ymin - f1) + x1, ymin, "stroke_weight=" + std::to_string(_stroke_weight), _color);
            _lines->push_back(newLine);
        }
    }

    for (std::list<Line*>::iterator iter = _lines->begin(); iter != _lines->end(); iter++) {
        (*iter)->draw(G, original);
    }
}