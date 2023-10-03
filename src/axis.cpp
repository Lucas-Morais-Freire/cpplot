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
    std::regex bool_pattern("true|false");
	if (std::regex_match(arg, double_pattern)) {
        if (key == "tick_size") {
            if (std::stod(arg) <= 0) {
                std::cout << "tick_size can't be negative.\n";
                return;
            }
            _tick_size = std::stod(arg);
        } else if (key == "step") {
            if (std::stod(arg) <= 0) {
                std::cout << "step can't be negative or zero.\n";
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
        } else if (key == "min_re") {
            _min_re = std::stod(arg);
        } else if (key == "max_re") {
            _max_re = std::stod(arg);
        } else if (key == "min_img") {
            if (0 > std::stod(arg) || std::stod(arg) > 1) {
                std::cout << key << " must be between 0 and 1.\n";
                return;
            }
            _min_img = std::stod(arg);
        } else if (key == "max_img") {
            if (0 > std::stod(arg) || std::stod(arg) > 1) {
                std::cout << key << " must be between 0 and 1.\n";
                return;
            }
            _max_img = std::stod(arg);
        } else if (key == "head_size") {
            if (std::stod(arg) < 0) {
                std::cout << arg << "can't be negative.\n";
                return;
            }
            _head_size = std::stoi(arg);
        } else if (key == "head_angle") {
            _head_angle = std::stod(arg);
        }
	} else if (std::regex_match(arg, bool_pattern)) {
        if (key == "draw_ticks") {
            if (arg == "true" || arg == "1") {
                _draw_ticks = true;
            } else {
                _draw_ticks = false;
            }
        } else if (key == "full_ticks") {
            if (arg == "true" || arg == "1") {
                _full_ticks = true;
            } else {
                _full_ticks = false;
            }
        } else if (key == "draw_head") {
            if (arg == "true" || arg == "1") {
                _draw_head = true;
            } else {
                _draw_head = false;
            }
        } else if (key == "ticks_to_left") {
            if (arg == "true" || arg == "1") {
                _ticks_to_left = true;
            } else {
                _ticks_to_left = false;
            }
        } else if (key == "half_ticks") {
            if (arg == "true" || arg == "1") {
                _half_ticks = true;
            } else {
                _half_ticks = false;
            }
        }
	} else {
        std::cout << "argument \"" << arg << "\" is not a valid literal for " << key << "\n";
    }
}

Axis::Axis(bool x_axis, double position, bool relative, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        _x_axis = x_axis;
        _relative = relative;
        if (_relative && (position < 0 || position > 1)) {
            throw std::runtime_error("relative positions should be between 0 and 1.\n");
        }
        _position = position;
        _color = color;
    
    // initialize dynamic attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // this constructor supports these optional parameters:
    _keys = {
        "tick_size",//
        "step",//
        "start",//
        "stroke_weight",//
        "min_re",//
        "max_re",//
        "min_img",//
        "max_img",//
        "head_size",//
        "head_angle",//
        "draw_ticks",//
        "full_ticks",
        "ticks_to_left",
        "half_ticks",
        "draw_head"//
    };
     // initialize optional parameters with given string:
    std::list<std::string>* keys = params != "" ? init(params) : new std::list<std::string>(_keys);
    // if some values were not assigned, assign their default values:
    for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "tick_size") {
            _tick_size = -HUGE_VAL; // drawn based on head_size.
        } else if ((*iter) == "step") {
            _step = -HUGE_VAL;
        } else if ((*iter) == "start") {
            _start = -HUGE_VAL;
        } else if ((*iter) == "stroke_weight") {
            _stroke_weight = 1;
        } else if ((*iter) == "min_re") {
            _min_re = -HUGE_VAL;
        } else if ((*iter) == "max_re") {
            _max_re = HUGE_VAL;
        } else if ((*iter) == "min_img") {
            _min_img = 0;
        } else if ((*iter) == "max_img") {
            _max_img = 1;
        } else if ((*iter) == "draw_ticks") {
            _draw_ticks = true;
        } else if ((*iter) == "full_ticks") {
            _full_ticks = false;
        } else if ((*iter) == "draw_head") {
            _draw_head = false;
        } else if ((*iter) == "ticks_to_left") {
            _ticks_to_left = true;
        } else if ((*iter) == "half_ticks") {
            _half_ticks = true;
        }else if ((*iter) == "head_size") {
            _head_size = 20;
        } else if ((*iter) == "head_angle") {
            _head_angle = 20;
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

    for (std::list<Line*>::iterator iter = _ticks->begin(); iter != _ticks->end(); iter++) {
        delete (*iter);
    }
    _ticks->clear();

    double xi, xf, yi, yf;
    double min_img, max_img;
    if (_x_axis) { // checks if this is an x axis
        min_img = (G->xmax() - G->xmin())*_min_img + G->xmin();
        max_img = (G->xmax() - G->xmin())*_max_img + G->xmin();
        xi = min_img < _min_re ? _min_re : min_img;
        xf = max_img > _max_re ? _max_re : max_img;
        if (_relative) { // if position is relative
            yi = yf = (G->ymax() - G->ymin())*_position + G->ymin();
        } else {
            yi = yf = _position;
        }
    } else {
        min_img = (G->ymax() - G->ymin())*_min_img + G->ymin();
        max_img = (G->ymax() - G->ymin())*_max_img + G->ymin();
        yi = min_img < _min_re ? _min_re : min_img;
        yf = max_img > _max_re ? _max_re : max_img;
        if (_relative) {
            xi = xf = (G->xmax() - G->xmin())*_position + G->xmin();
        } else {
            xi = xf = _position;
        }
    }

    if (_draw_ticks) {
        if (_tick_size == -HUGE_VAL) {
            _tick_size = 0.02*(G->yres() < G->xres() ? G->yres() : G->xres());
        }

        if (_step == -HUGE_VAL) {
            _step = 1 << (_x_axis ? (int)(std::log((G->xmax() - G->xmin())/6)) : (int)(std::log((G->ymax() - G->ymin())/6)));
        }

        Line* newTick;
        if (_x_axis) {
            if (_start == -HUGE_VAL) {
                if (xi > 0) {
                    _start = ((int)(xi/_step + 1))*_step;
                } else {
                    _start = ((int)(xi/_step))*_step;
                }
            }
            if (_start < xi || _start > xf) {
                std::cout << "starting point for ticks can't be outside drawing bounds.\n";
                return;
            }

            if (_full_ticks) {
                for (double x = _start; x <= xf; x += _step) {
                    newTick = new Line(x, yi + G->ypos(G->iIdx(0) - _tick_size), x, yi - G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                    _ticks->push_back(newTick);
                    newTick->draw(G, posession);
                }
                for (double x = _start - _step; x >= xi; x -= _step) {
                    newTick = new Line(x, yi + G->ypos(G->iIdx(0) - _tick_size), x, yi - G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                    _ticks->push_back(newTick);
                    newTick->draw(G, posession);
                }

                if (_half_ticks) {
                    for (double x = _start + 0.5*_step; x <= xf; x += _step) {
                        newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - _tick_size), x, yi - 0.5*G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double x = _start - 0.5*_step; x >= xi; x -= _step) {
                        newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - _tick_size), x, yi - 0.5*G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                }
            } else {
                if (_ticks_to_left) {
                    for (double x = _start; x <= xf; x += _step) {
                        newTick = new Line(x, yi + G->ypos(G->iIdx(0) - _tick_size), x, yi, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double x = _start - _step; x >= xi; x -= _step) {
                        newTick = new Line(x, yi + G->ypos(G->iIdx(0) - _tick_size), x, yi, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }

                    if (_half_ticks) {
                        for (double x = _start + 0.5*_step; x <= xf; x += _step) {
                            newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - _tick_size), x, yi, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                        for (double x = _start - 0.5*_step; x >= xi; x -= _step) {
                            newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - _tick_size), x, yi, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                    }
                } else {
                    for (double x = _start; x <= xf; x += _step) {
                        newTick = new Line(x, yi, x, yi - G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double x = _start - _step; x >= xi; x -= _step) {
                        newTick = new Line(x, yi, x, yi - G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }

                    if (_half_ticks) {
                        for (double x = _start + 0.5*_step; x <= xf; x += _step) {
                            newTick = new Line(x, yi, x, yi - 0.5*G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                        for (double x = _start - 0.5*_step; x >= xi; x -= _step) {
                            newTick = new Line(x, yi, x, yi - 0.5*G->ypos(G->iIdx(0) - _tick_size), "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                    }
                }
            }
        } else {
            if (_start == -HUGE_VAL) {
                if (yi > 0) {
                    _start = ((int)(yi/_step + 1))*_step;
                } else {
                    _start = ((int)(yi/_step))*_step;
                }
            }
            if (_start < yi || _start > yf) {
                std::cout << "starting point for ticks can't be outside drawing bounds.\n";
            }

            if (_full_ticks) {
                for (double y = _start; y <= yf; y += _step) {
                    newTick = new Line(xi - G->xpos(G->jIdx(0) + _tick_size), y, xi + G->xpos(G->jIdx(0) + _tick_size), y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                    _ticks->push_back(newTick);
                    newTick->draw(G, posession);
                }
                for (double y = _start - _step; y >= yi; y -= _step) {
                    newTick = new Line(xi - G->xpos(G->jIdx(0) + _tick_size), y, xi + G->xpos(G->jIdx(0) + _tick_size), y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                    _ticks->push_back(newTick);
                    newTick->draw(G, posession);
                }

                if (_half_ticks) {
                    for (double y = _start + 0.5*_step; y <= yf; y += _step) {
                        newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi + 0.5*G->xpos(G->jIdx(0) + _tick_size), y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double y = _start - 0.5*_step; y >= yi; y -= _step) {
                        newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi + 0.5*G->xpos(G->jIdx(0) + _tick_size), y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                }
            } else {
                if (_ticks_to_left) {
                    for (double y = _start; y <= yf; y += _step) {
                        newTick = new Line(xi - G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double y = _start - _step; y >= yi; y -= _step) {
                        newTick = new Line(xi - G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }

                    if (_half_ticks) {
                        for (double y = _start + 0.5*_step; y <= yf; y += _step) {
                            newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                        for (double y = _start - 0.5*_step; y >= yi; y -= _step) {
                            newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                    }
                } else {
                    for (double y = _start; y <= yf; y += _step) {
                        newTick = new Line(xi + G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }
                    for (double y = _start - _step; y >= yi; y -= _step) {
                        newTick = new Line(xi + G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                        _ticks->push_back(newTick);
                        newTick->draw(G, posession);
                    }

                    if (_half_ticks) {
                        for (double y = _start + 0.5*_step; y <= yf; y += _step) {
                            newTick = new Line(xi + 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                        for (double y = _start - 0.5*_step; y >= yi; y -= _step) {
                            newTick = new Line(xi + 0.5*G->xpos(G->jIdx(0) + _tick_size), y, xi, y, "stroke_weight="+std::to_string(_stroke_weight), _color);
                            _ticks->push_back(newTick);
                            newTick->draw(G, posession);
                        }
                    }
                }
            }
        }
    }

    delete _arrow;
    _arrow = new Arrow(xi, yi, xf, yf, "stroke_weight="+std::to_string(_stroke_weight)+",head_size="+std::to_string(_draw_head ? _head_size : 0)+",angle="+std::to_string(_head_angle), _color);
    _arrow->draw(G, posession);

    for (std::list<Line*>::iterator iter = _ticks->begin(); iter != _ticks->end(); iter++) {
        (*iter)->draw(G, posession);
    }
}

void Axis::draw(Graph* G) {
    cv::Mat* posession = new cv::Mat;
	this->draw(G, posession);
    delete posession;
}