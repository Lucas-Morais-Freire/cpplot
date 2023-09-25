#include "../headers/arrow.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Arrow::Arrow() {
    _stem = new Line;
    _lBranch = new Line;
    _rBranch = new Line;
    _hsize = _angle = 0;
}

Arrow::Arrow(double xa, double ya, double xb, double yb, double sw, double hsize, double angle, cv::Vec3b color) {
    _stem = new Line(xa, ya, xb, yb, sw, color);
    _lBranch = new Line;
    _rBranch = new Line;
    _angle = M_PI/180*angle;
    _hsize = hsize;
}

Arrow::Arrow(double xa, double ya, double xb, double yb, double sw, double hsize, cv::Vec3b color) {
    _stem = new Line(xa, ya, xb, yb, sw, color);
    _lBranch = new Line;
    _rBranch = new Line;
    _angle = M_PI/9;
    _hsize = hsize;
}

Arrow::~Arrow() {
    delete _stem;
    delete _rBranch;
    delete _lBranch;
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
    double hsize = _hsize/sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));

    delete _lBranch;
    _lBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xa - xb)*S)), G->ypos(xb + hsize*((xa - xb)*C + (yb - ya)*S)), _stem->sw(), _stem->color());
    _lBranch->draw(G, posession);

    delete _rBranch;
    _rBranch = new Line(_stem->xb(), _stem->yb(), G->xpos(yb + hsize*((ya - yb)*C + (xb - xa)*S)), G->ypos(xb + hsize*((xa - xb)*C + (ya - yb)*S)), _stem->sw(), _stem->color());
    _rBranch->draw(G, posession);
}