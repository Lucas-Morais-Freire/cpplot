#include "../headers/axis.hpp"
#include "../headers/arrow.hpp"
#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"
#include <regex>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Axis::assign(std::string key, std::string arg) {
    std::regex double_pattern("(0|\\-?(0|[1-9][0-9]*)(\\.[0-9]*)?|\\-?\\.[0-9]+)");
    std::regex bool_pattern("true|false|0|1");
	if (std::regex_match(arg, double_pattern)) {
		if (key == "step") {
            if (std::stod(arg) < 0) {
                std::cout << "step can't be negative.\n";
                return;
            }
            _step = std::stod(arg);
        } else if (key == "start") {
            _start = std::stod(arg);
        } else if (key == "stroke_weight") {
            if (std::stod(arg) < 0) {
                std::cout << "stroke_weight can't be negative.\n";
                return;
            }
            _stroke_weight = std::stod(arg);
        } else if (key == "xmin") {
            _xmin = std::stod(arg);
        } else if (key == "ymin") {
            _ymin = std::stod(arg);
        } else if (key == "xmax") {
            _xmax = std::stod(arg);
        } else if (key == "ymax") {
            _ymax = std::stod(arg);
        } else if (key == "head_size") {
            if (std::stod(arg) < 0) {
                std::cout << arg << "can't be negative.\n";
                return;
            }
            _head_size = std::stoi(arg);
        } else if (key == "head_angle") {
            _head_angle = std::stod(arg)*M_PI/180;
        }
	} else if (std::regex_match(arg, bool_pattern)) {
        if (key == "full_ticks") {
            if (key == "true" || key == "1") {
                _full_ticks = true;
            } else {
                _full_ticks = false;
            }
        } else if (key == "draw_head") {
            if (key == "true" || key == "1") {
                _draw_head = true;
            } else {
                _draw_head = false;
            }
        } else if (key == "ticks_to_left") {
            if (key == "true" || key == "1") {
                _ticks_to_left = true;
            } else {
                _ticks_to_left = false;
            }
        } else if (key == "half_ticks") {
            if (key == "true" || key == "1") {
                _ticks_to_left = true;
            } else {
                _ticks_to_left = false;
            }
        }
	} else {
        if (key == "horizontal_placement") {
            if (arg == "bottom") {
                _horizontal_placement = 0;
            } else if (arg == "middle_bottom") {
                _horizontal_placement = 1;
            } else if (arg == "middle") {
                _horizontal_placement = 2;
            } else if (arg == "middle_top") {
                _horizontal_placement = 3;
            } else if (arg == "top") {
                _horizontal_placement = 4;
            }
        } else if (key == "vertical_placement") {
            if (arg == "left") {
                _vertical_placement = 0;
            } else if (arg == "middle_left") {
                _vertical_placement = 1;
            } else if (arg == "middle") {
                _vertical_placement = 2;
            } else if (arg == "middle_right") {
                _vertical_placement = 3;
            } else if (arg == "right") {
                _vertical_placement = 4;
            }
        } else {
		    std::cout << "argument \"" << arg << "\" is not a valid literal for " << key << "\n";
        }
    }
}

Axis::Axis(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        _xa = xa; _ya = ya; _xb = xb; _yb = yb;
        _color = color;
    
    // initialize keyword attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // this constructor supports these optional parameters:
    _keys = {
        "step",
        "start",
        "stroke_weight",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "head_size",
        "head_angle",
        "full_ticks",
        "draw_head",
        "ticks_to_left",
        "half_ticks"
    };
     // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
    for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "step") {
            _step = -HUGE_VAL; // drawn based on screen resolution.
        } else if ((*iter) == "start") {
            _start = 0;
        } else if ((*iter) == "stroke_weight") {
            _stroke_weight = 1;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        } else if ((*iter) == "full_ticks") {
            _full_ticks = false;
        } else if ((*iter) == "draw_head") {
            _draw_head = false;
        } else if ((*iter) == "head_size") {
            _head_size = 0;
        } else if ((*iter) == "head_angle") {
            _head_angle = M_PI/9;
        }
	}
	delete keys;
}

Axis::Axis(bool x_axis, std::string params, cv::Vec3b color) {
    // initialize obligatory attributes:
        if (x_axis) {
            _xa = -HUGE_VAL; _ya = HUGE_VAL; _xb = HUGE_VAL; _yb = HUGE_VAL;
        } else {
            _xa = -HUGE_VAL; _ya = -HUGE_VAL; _xb = HUGE_VAL; _yb = -HUGE_VAL;
        }
    // initialize keyword attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // this constructor supports these optional parameters:
    _keys = {
        "step",
        "start",
        "stroke_weight",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "head_size",
        "head_angle",
        "full_ticks",
        "draw_head",
        "ticks_to_left",
        "half_ticks",
        "horizontal_placement",
        "vertical_placement"
    };
     // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
    for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "step") {
            _step = -HUGE_VAL; // drawn based on screen resolution.
        } else if ((*iter) == "start") {
            _start = 0;
        } else if ((*iter) == "stroke_weight") {
            _stroke_weight = 1;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        } else if ((*iter) == "full_ticks") {
            _full_ticks = false;
        } else if ((*iter) == "draw_head") {
            _draw_head = false;
        } else if ((*iter) == "head_size") {
            _head_size = 0;
        } else if ((*iter) == "head_angle") {
            _head_angle = M_PI/9;
        } else if ((*iter) == "horizontal_placement") {
            _horizontal_placement = 0;
        } else if ((*iter) == "vertical_placement") {
            _vertical_placement = 0;
        }
	}
	delete keys;
}

Axis::Axis(bool x_axis, double position, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        if (x_axis) {
            _xa = position; _ya = -HUGE_VAL; _xb = position; _yb = HUGE_VAL;
        } else {
            _xa = -HUGE_VAL; _ya = position; _xb = HUGE_VAL; _yb = position;
        }
        _color = color;
    
    // initialize keyword attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // this constructor supports these optional parameters:
    _keys = {
        "step",
        "start",
        "stroke_weight",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "head_size",
        "head_angle",
        "full_ticks",
        "draw_head",
        "ticks_to_left",
        "half_ticks"
    };
     // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
    for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "step") {
            _step = -HUGE_VAL; // drawn based on screen resolution.
        } else if ((*iter) == "start") {
            _start = 0;
        } else if ((*iter) == "stroke_weight") {
            _stroke_weight = 1;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        } else if ((*iter) == "full_ticks") {
            _full_ticks = false;
        } else if ((*iter) == "draw_head") {
            _draw_head = false;
        } else if ((*iter) == "head_size") {
            _head_size = 0;
        } else if ((*iter) == "head_angle") {
            _head_angle = M_PI/9;
        }
	}
	delete keys;
}

Axis::Axis(double x, double y, double angle, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        _xa = x; _ya = y;
        _xb = x + 1;
        _yb = y + tan(angle*M_PI/180);
    
    // initialize keyword attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // this constructor supports these optional parameters:
    _keys = {
        "step",
        "start",
        "stroke_weight",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "head_size",
        "head_angle",
        "full_ticks",
        "draw_head",
        "ticks_to_left",
        "half_ticks"
    };
     // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
    for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "step") {
            _step = -HUGE_VAL; // drawn based on screen resolution.
        } else if ((*iter) == "start") {
            _start = 0;
        } else if ((*iter) == "stroke_weight") {
            _stroke_weight = 1;
        } else if ((*iter) == "xmin") {
            _xmin = -HUGE_VAL;
        } else if ((*iter) == "ymin") {
            _ymin = -HUGE_VAL;
        } else if ((*iter) == "xmax") {
            _xmax = HUGE_VAL;
        } else if ((*iter) == "ymax") {
            _ymax = HUGE_VAL;
        } else if ((*iter) == "full_ticks") {
            _full_ticks = false;
        } else if ((*iter) == "draw_head") {
            _draw_head = false;
        } else if ((*iter) == "head_size") {
            _head_size = 0;
        } else if ((*iter) == "head_angle") {
            _head_angle = M_PI/9;
        }
	}
	delete keys;
}

Axis::~Axis() {
    delete _arrow;
    for (std::list<Line*>::iterator iter = _ticks->begin(); iter != _ticks->end(); iter++) {
        delete (*iter);
    }
    _ticks->clear();
    delete _ticks;
}

void Axis::draw(Graph* G, cv::Mat* posession) {
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

    if (_xa == -HUGE_VAL && _ya == HUGE_VAL && _xb == HUGE_VAL && _yb == HUGE_VAL) { // checks if this is an x_axis
        switch (_vertical_placement) {
        case 0:
            delete _arrow;
            _arrow = new Arrow(G->xmin() >= _xmin ? G->xmin() : _xmin, G->ypos(G->yres() - 1 - _stroke_weight - _head_size*sin(_head_angle)), G->xmax() <= _xmax ? G->xmax() : _xmax, G->ypos(G->yres() - 1 - _stroke_weight - _head_size*sin(_head_angle)), "stroke_weight="+std::to_string(_stroke_weight)+",head_size="+std::to_string(_head_size)+",angle="+std::to_string(_head_angle), _color);
            break;
        default:
        }
    } else if (_xa == -HUGE_VAL && _ya == -HUGE_VAL && _xb == HUGE_VAL && _yb == -HUGE_VAL) {

    }
}

void Axis::draw(Graph* G) {
    cv::Mat* posession = new cv::Mat;
	this->draw(G, posession);
    delete posession;
}