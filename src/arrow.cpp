#include "../headers/arrow.hpp"
#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"
#include <iostream>
#include <regex>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Arrow::assign(std::string key, std::string arg) {
    std::regex double_pattern("(0|\\-?(0|[1-9][0-9]*)(\\.[0-9]*)?|\\-?\\.[0-9]+)");
	if (std::regex_match(arg, double_pattern)) {
		if (key == "stroke_weight") {
			if (std::stod(arg) < 0) {
				std::cout << key << "can't be negative.\n";
				return;
			}
			_stem = new Line(_xa, _ya, _xb, _yb, key + " = " + arg, _color);
			return;
		} else if (key == "angle") {
            _angle = M_PI/180*std::stod(arg);
            return;
        } else if (key == "head_size") {
            if (std::stod(arg) < 0) {
				std::cout << key << "can't be negative.\n";
				return;
			}
            _head_size = std::stod(arg);
            return;
        }
	} else {
		std::cout << "argument \"" << arg << "\" is not a valid double literal.\n";
	}
}

Arrow::Arrow(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color) {
    // initialize obligatory values:
        // dynamic:
            _lBranch = NULL;
            _rBranch = NULL;
        // standard:
            _xa = xa; _ya = ya; _xb = xb; _yb = yb;
            _color = color;
    
    // Arrow has these optional parameters:
    _keys = {
        "angle",
        "stroke_weight",
        "head_size"
    };
    // initialize optional parameters with given string:
    std::list<std::string>* keys = init(params);
    // if some values were not assigned, assign their default values:
	for(std::list<std::string>::iterator iter = keys->begin(); iter != keys->end(); iter++) {
		if ((*iter) == "angle") {
            _angle = M_PI/9;
		} else if ((*iter) == "stroke_weight") {
			_stem = new Line(_xa, _ya, _xb, _yb, _color);
        } else if ((*iter) == "head_size") {
            _head_size = 20;
        }
	}
	delete keys;
}

Arrow::~Arrow() {
    delete _stem;
    if (_rBranch != NULL) {
        delete _rBranch;
    }
    if (_lBranch != NULL) {
        delete _lBranch;
    }
}

void Arrow::draw(Graph* G) {
    cv::Mat* posession = new cv::Mat;
	this->draw(G, posession);
    delete posession;
}

void Arrow::draw(Graph* G, cv::Mat* posession) {
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

    if (_head_size < 0) {
        std::cout << "arrow's head size can't be negative. Unable to draw.\n";
        return;
    }

    _stem->draw(G, posession);

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
    
    delete _lBranch;
    _lBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xa - xb)*S)), G->ypos(xb + hsize*((xa - xb)*C + (yb - ya)*S)), "stroke_weight=" + std::to_string(_stem->strokeWeight()), _stem->color());
    _lBranch->draw(G, posession);

    delete _rBranch;
    _rBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xb - xa)*S)), G->ypos(xb + hsize*((xa - xb)*C + (ya - yb)*S)),"stroke_weight=" + std::to_string(_stem->strokeWeight()), _stem->color());
    _rBranch->draw(G, posession);
}