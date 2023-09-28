#include "../headers/cpplot.hpp"
#include <iostream>
#include <regex>
#include <algorithm>

void Graph::init(const char* params) {
    std::string s = params;
    std::list<std::string> keys = _keys;
    std::list<std::string> done;
    std::regex pattern("( *\\w+ *= *([0-9]|\\.|[a-z]|[A-Z])+ *,)*( *\\w+ *= *([0-9]|\\.|[a-z]|[A-Z])+ *)");
    std::regex key_matches("\\w+ *=");
    std::regex arg_matches("= *([0-9]|\\.|[a-z]|[A-Z])+");
    if (std::regex_match(s, pattern)) { // if the parameter string matches the regex...
        // ...we will check for all keys that were given.
        std::sregex_iterator key_match_iter(s.begin(), s.end(), key_matches);
        std::sregex_iterator key_match_begin = key_match_iter;
        while (key_match_iter != std::sregex_iterator()) {
            // get the position of the character just after the key:
            size_t pos = key_match_iter->str().find(" ") != std::string::npos ? key_match_iter->str().find(" ") : key_match_iter->str().find("=");
            // extract only the key's string from the iterator and check if it is a valid key:
            std::string key = key_match_iter->str().substr(0,pos);
            std::list<std::string>::iterator keys_list_iter = std::find(keys.begin(), keys.end(), key);
            if (keys_list_iter != keys.end()) { // if it is valid...
                // ...try to get the value it is supposed to be assigned to. First, get the positon of the key match we got:
                pos = std::distance(key_match_begin, key_match_iter);
                // after we match for arguments, we will need its iterator to be at the same distance:
                std::sregex_iterator arg_match_iter(s.begin(), s.end(), arg_matches);
                std::sregex_iterator arg_match_begin = arg_match_iter;
                // go through the argument matches until it is on the same position as the key match: 
                while (std::distance(arg_match_begin, arg_match_iter) != pos) {
                    arg_match_iter++;
                }
                // get position right before the beginning of the argument and extract the string after it:
                pos = arg_match_iter->str().rfind(" ") != std::string::npos ? arg_match_iter->str().rfind(" ") : arg_match_iter->str().rfind("=");
                std::string arg = arg_match_iter->str().substr(pos + 1);
                // now, check if it is possible to convert this string to a value and assign it to its variable:
                assign(key, arg);
                // erase that key from the list so that we don't re-assign any values to it.
                keys.erase(keys_list_iter);
            } else {
                std::cout << "\"" << key << "\" is not a valid key for this object or you have repeated it. valid keys are: ";
                std::list<std::string>::iterator last = _keys.end();
                last--;
                for (keys_list_iter = _keys.begin(); keys_list_iter != last; keys_list_iter++) {
                    std::cout << (*keys_list_iter) << ", ";
                }
                std::cout << (*keys_list_iter) << ".\n";
                return; 
            }
            key_match_iter++;
        }
    } else {
        std::cout << "invalid keyword-argument list format. the correct syntax is \"key1 = val1, key2 = val2, ...\"\n";
    }
}

void Graph::assign(std::string key, std::string arg) {
    std::cout << "assigning \"" << arg << "\" to \"" << key << "\"\n";
}

Graph::Graph() {
    _canvas = new cv::Mat(854, 480, CV_8UC3);
    _xmin = -5; _xmax = 5;
    _ymin = -5; _ymax = 5;
    _bgColor = {255, 255, 255};
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

Graph::Graph(double xmin, double xmax, double ymin, double ymax, const char* params, cv::Vec3b bgColor) {
    _canvas = new cv::Mat(854, 480, CV_8UC3);
    _xmin = xmin; _xmax = xmax;
    _ymin = ymin; _ymax = ymax;
    _bgColor = bgColor;
    _drawOrder = new std::list<Drawing*>;
    (*_canvas) = cv::Scalar(_bgColor[0], _bgColor[1], _bgColor[2]);
    init(params);
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