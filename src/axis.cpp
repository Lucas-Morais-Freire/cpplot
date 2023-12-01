#include "../headers/axis.hpp"
#include "../headers/arrow.hpp"
#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Axis::Axis(bool x_axis, double position, bool relative, cv::Vec3b color) {
    // initialize obligatory values:
        _x_axis = x_axis;
        _relative = relative;
        if (_relative && (position < 0 || position > 1)) {
            throw std::runtime_error("relative positions should be between 0 and 1.\n");
            exit(-1);
        }
        _position = position;
        _color = color;
    
    // initialize dynamic attributes:
        _ticks = new std::list<Line*>;
        _arrow = NULL;

    // initialize optional attributes:
        _tick_size = -HUGE_VAL; // drawn based on head_size.
        _step = -HUGE_VAL;
        _start = -HUGE_VAL;
        _stroke_weight = 1.0;
        _min_re = -HUGE_VAL;
        _max_re = HUGE_VAL;
        _min_img = 0.0;
        _max_img = 1.0;
        _draw_ticks = true;
        _full_ticks = false;
        _draw_head = false;
        _ticks_to_left = true;
        _half_ticks = true;
        _head_size = 20.0;
        _head_angle = 20.0;
        _draw_numbers = true;
        _number_size = 0;
        _number_offset = -HUGE_VAL;
}

Axis::~Axis() {
    delete _arrow;
    for (std::list<Line*>::iterator iter = _ticks->begin(); iter != _ticks->end(); iter++) {
        delete (*iter);
    }
    _ticks->clear();
    delete _ticks;
}

Axis& Axis::tickSize(double tick_size) {
    _tick_size = tick_size;
    return *this;
}

Axis& Axis::step(double step) {
    _step = step;
    return *this;
}

Axis& Axis::start(double start) {
    _start = start;
    return *this;
}

Axis& Axis::strokeWeight(double stroke_weight) {
    _stroke_weight = stroke_weight;
    return *this;
}

Axis& Axis::minRe(double min_re) {
    _min_re = min_re;
    return *this;
}

Axis& Axis::maxRe(double max_re) {
    _max_re = max_re;
    return *this;
}

Axis& Axis::minImg(double min_img) {
    _min_img = min_img;
    return *this;
}

Axis& Axis::maxImg(double max_img) {
    _max_img = max_img;
    return *this;
}

Axis& Axis::headSize(double head_size) {
    _head_size = head_size;
    return *this;
}

Axis& Axis::headAngle(double head_angle) {
    _head_angle = head_angle;
    return *this;
}

Axis& Axis::numberSize(int number_size) {
    _number_size = number_size;
    return *this;
}

Axis& Axis::numberOffset(double number_offset) {
    _number_offset = number_offset;
    return *this;
}

Axis& Axis::drawTicks(bool draw_ticks) {
    _draw_ticks = draw_ticks;
    return *this;
}

Axis& Axis::fullTicks(bool full_ticks) {
    _full_ticks = full_ticks;
    return *this;
}

Axis& Axis::ticksToLeft(bool ticks_to_left) {
    _ticks_to_left = ticks_to_left;
    return *this;
}

Axis& Axis::halfTicks(bool half_ticks) {
    _half_ticks = half_ticks;
    return *this;
}

Axis& Axis::drawHead(bool draw_head) {
    _draw_head = draw_head;
    return *this;
}

Axis& Axis::drawNumbers(bool draw_numbers) {
    _draw_numbers = draw_numbers;
    return *this;
}

void Axis::draw(Graph* G, cv::Mat* original) {
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

    double tick_size, step, start, number_size, number_offset;

    if (_tick_size == -HUGE_VAL) {
        tick_size = 0.02*(canvas->rows < canvas->cols ? canvas->rows : canvas->cols);
    } else {
        tick_size = _tick_size;
    }

    if (_step == -HUGE_VAL) {
        step = std::pow(2, (_x_axis ? std::floor(std::log2((G->xmax() - G->xmin())/6)) : std::floor(std::log2((G->ymax() - G->ymin())/6))));
    } else {
        step = _step;
    }

    if (_draw_ticks || _draw_numbers) {
        Line* newTick;
        if (_x_axis) {
            if (_start == -HUGE_VAL) {
                if (xi > 0) {
                    start = ((int)(xi/step + 1))*step;
                } else {
                    start = ((int)(xi/step))*step;
                }
            } else {
                start = _start;
            }
            if (start < xi || start > xf) {
                std::cout << "starting point for ticks or numbers can't be outside drawing bounds.\n";
                return;
            }

            if (_draw_numbers) {
                if (_number_size <= 0) {
                    number_size = 1;
                } else {
                    number_size = _number_size;
                }
                if (_number_offset == -HUGE_VAL) {
                    number_offset = tick_size/2 + 1;
                } else {
                    number_offset = _number_offset;
                }

                cv::Point pt;
                pt.y = (int)(G->iIdx(yi) < 0 ? 0 : (G->iIdx(yi) >= canvas->rows ? canvas->rows - 1 : G->iIdx(yi)));
                for (double x = start; x <= xf; x += step) {
                    pt.x = (int)(G->jIdx(x));
                    cv::putText(*canvas, cv::String(cv::format("%.0f", x)), pt, cv::FONT_HERSHEY_SIMPLEX, number_size, this->_color);
                }
                for (double x = start - step; x >= xi; x -= step) {
                    pt.x = (int)(G->jIdx(x));
                    cv::putText(*canvas, cv::String(cv::format("%.0f", x)), pt, cv::FONT_HERSHEY_SIMPLEX, number_size, this->_color);
                }
            }

            if (_draw_ticks) {
                if (_full_ticks) {
                    for (double x = start; x <= xf; x += step) {
                        newTick = new Line(x, yi + G->ypos(G->iIdx(0) - tick_size), x, yi - G->ypos(G->iIdx(0) - tick_size), _color);
                        newTick->strokeWeight(_stroke_weight);
                        _ticks->push_back(newTick);
                    }
                    for (double x = start - step; x >= xi; x -= step) {
                        newTick = new Line(x, yi + G->ypos(G->iIdx(0) - tick_size), x, yi - G->ypos(G->iIdx(0) - tick_size), _color);
                        newTick->strokeWeight(_stroke_weight);
                        _ticks->push_back(newTick);
                    }

                    if (_half_ticks) {
                        for (double x = start + 0.5*step; x <= xf; x += step) {
                            newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - tick_size), x, yi - 0.5*G->ypos(G->iIdx(0) - tick_size), _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double x = start - 0.5*step; x >= xi; x -= step) {
                            newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - tick_size), x, yi - 0.5*G->ypos(G->iIdx(0) - tick_size), _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                    }
                } else {
                    if (_ticks_to_left) {
                        for (double x = start; x <= xf; x += step) {
                            newTick = new Line(x, yi + G->ypos(G->iIdx(0) - tick_size), x, yi, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double x = start - step; x >= xi; x -= step) {
                            newTick = new Line(x, yi + G->ypos(G->iIdx(0) - tick_size), x, yi, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }

                        if (_half_ticks) {
                            for (double x = start + 0.5*step; x <= xf; x += step) {
                                newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - tick_size), x, yi, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                            for (double x = start - 0.5*step; x >= xi; x -= step) {
                                newTick = new Line(x, yi + 0.5*G->ypos(G->iIdx(0) - tick_size), x, yi, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                        }
                    } else {
                        for (double x = start; x <= xf; x += step) {
                            newTick = new Line(x, yi, x, yi - G->ypos(G->iIdx(0) - tick_size), _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double x = start - step; x >= xi; x -= step) {
                            newTick = new Line(x, yi, x, yi - G->ypos(G->iIdx(0) - tick_size), _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }

                        if (_half_ticks) {
                            for (double x = start + 0.5*step; x <= xf; x += step) {
                                newTick = new Line(x, yi, x, yi - 0.5*G->ypos(G->iIdx(0) - tick_size), _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                            for (double x = start - 0.5*step; x >= xi; x -= step) {
                                newTick = new Line(x, yi, x, yi - 0.5*G->ypos(G->iIdx(0) - tick_size), _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                        }
                    }
                }
            }
        } else {
            if (_start == -HUGE_VAL) {
                if (yi > 0) {
                    start = ((int)(yi/step + 1))*step;
                } else {
                    start = ((int)(yi/step))*step;
                }
            } else {
                start = _start;
            }
            if (start < yi || start > yf) {
                std::cout << "starting point for ticks can't be outside drawing bounds.\n";
            }

            if (_draw_numbers) {
                if (_number_size <= 0) {
                    number_size = 1;
                } else {
                    number_size = _number_size;
                }
                if (_number_offset == -HUGE_VAL) {
                    number_offset = tick_size/2 + 1;
                } else {
                    number_offset = _number_offset;
                }

                cv::Point pt;
                pt.x = (int)(G->jIdx(xi) < 0 ? 0 : (G->jIdx(xi) >= canvas->cols ? canvas->cols - 1 : G->jIdx(xi)));
                for (double y = start; y <= yf; y += step) {
                    pt.y = (int)(G->iIdx(y));
                    cv::putText(*canvas, cv::String(cv::format("%.0f", y)), pt, cv::FONT_HERSHEY_SIMPLEX, number_size, this->_color);
                }
                for (double y = start - step; y >= yi; y -= step) {
                    pt.y = (int)(G->iIdx(y));
                    cv::putText(*canvas, cv::String(cv::format("%.0f", y)), pt, cv::FONT_HERSHEY_SIMPLEX, number_size, this->_color);
                }
            }

            if (_draw_ticks) {
                if (_full_ticks) {
                    for (double y = start; y <= yf; y += step) {
                        newTick = new Line(xi - G->xpos(G->jIdx(0) + tick_size), y, xi + G->xpos(G->jIdx(0) + tick_size), y, _color);
                        newTick->strokeWeight(_stroke_weight);
                        _ticks->push_back(newTick);
                    }
                    for (double y = start - step; y >= yi; y -= step) {
                        newTick = new Line(xi - G->xpos(G->jIdx(0) + tick_size), y, xi + G->xpos(G->jIdx(0) + tick_size), y, _color);
                        newTick->strokeWeight(_stroke_weight);
                        _ticks->push_back(newTick);
                    }

                    if (_half_ticks) {
                        for (double y = start + 0.5*step; y <= yf; y += step) {
                            newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi + 0.5*G->xpos(G->jIdx(0) + tick_size), y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double y = start - 0.5*step; y >= yi; y -= step) {
                            newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi + 0.5*G->xpos(G->jIdx(0) + tick_size), y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                    }
                } else {
                    if (_ticks_to_left) {
                        for (double y = start; y <= yf; y += step) {
                            newTick = new Line(xi - G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double y = start - step; y >= yi; y -= step) {
                            newTick = new Line(xi - G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }

                        if (_half_ticks) {
                            for (double y = start + 0.5*step; y <= yf; y += step) {
                                newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                            for (double y = start - 0.5*step; y >= yi; y -= step) {
                                newTick = new Line(xi - 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                        }
                    } else {
                        for (double y = start; y <= yf; y += step) {
                            newTick = new Line(xi + G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }
                        for (double y = start - step; y >= yi; y -= step) {
                            newTick = new Line(xi + G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                            newTick->strokeWeight(_stroke_weight);
                            _ticks->push_back(newTick);
                        }

                        if (_half_ticks) {
                            for (double y = start + 0.5*step; y <= yf; y += step) {
                                newTick = new Line(xi + 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                            for (double y = start - 0.5*step; y >= yi; y -= step) {
                                newTick = new Line(xi + 0.5*G->xpos(G->jIdx(0) + tick_size), y, xi, y, _color);
                                newTick->strokeWeight(_stroke_weight);
                                _ticks->push_back(newTick);
                            }
                        }
                    }
                }
            }
        }
    }

    delete _arrow;
    _arrow = new Arrow(xi, yi, xf, yf, _color);
    _arrow->strokeWeight(_stroke_weight).headSize(_draw_head ? _head_size : 0).angle(_head_angle);
    _arrow->draw(G, original);

    for (std::list<Line*>::iterator iter = _ticks->begin(); iter != _ticks->end(); iter++) {
        (*iter)->draw(G, original);
    }
}

void Axis::draw(Graph* G) {
    cv::Mat* original = new cv::Mat;
	this->draw(G, original);
    delete original;
}