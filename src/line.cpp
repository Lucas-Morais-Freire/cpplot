#include "../headers/line.hpp"
#include <iostream>

Line::Line() {
    _xa = 0; _ya = 0; _xb = 0; _yb = 0; _sw = 0;
    _color = {255, 255, 255};
}

Line::Line(double xa, double ya, double xb, double yb, double sw, cv::Vec3b color) {
    _xa = xa;
    _ya = ya;
    _xb = xb;
    _yb = yb;
    _sw = sw;
    _color = color;
}

void Line::draw(Graph* G) {
	cv::Mat* posession = new cv::Mat;
	this->draw(G, posession);
	delete posession;
}

void Line::draw(Graph* G, cv::Mat* posession) {
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

	if (_xa == _xb && _ya == _yb) {
		std::cout << "Endpoints are equal. Unable to draw.\n";
		return;
	}

	double xaCvs = G->iIdx(_ya);
	double yaCvs = G->jIdx(_xa);
	double xbCvs = G->iIdx(_yb);
	double ybCvs = G->jIdx(_xb);

	if (xaCvs == xbCvs && yaCvs == ybCvs) {
		std::cout << "Endpoints are too close due to resolution of graph. Unable to draw.\n";
		return;
	}

	// determinar a ordem dos pontos no plano
	// P1 = (xa, ya) e P2 = (xb, yb)
	double xmin = xaCvs < xbCvs ? xaCvs : xbCvs;
	double xmax = xaCvs >= xbCvs ? xaCvs : xbCvs;
	double ymin = yaCvs < ybCvs ? yaCvs : ybCvs;
	double ymax = yaCvs >= ybCvs ? yaCvs : ybCvs;
	
	// determinar os limites da regiao retangular de teste
	int x1 = xmin - _sw < 0 ? 0 : (xmin - _sw >= G->yres() ? G->yres() - 1 : (int)(xmin - _sw));
	int y1 = ymin - _sw < 0 ? 0 : (ymin - _sw >= G->xres() ? G->xres() - 1 : (int)(ymin - _sw));
	int x2 = xmax + _sw >= G->yres() ? G->yres() - 1 : (xmax + _sw < 0 ? 0 : (int)(xmax + _sw) + 1);
	int y2 = ymax + _sw >= G->xres() ? G->xres() - 1 : (ymax + _sw < 0 ? 0 : (int)(ymax + _sw) + 1);

	double d;
	double pp1;
	double pp2;
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			// primeiro, testar se o ponto pode ser projetado perpendicularmente ao segmento de reta.
			// checar se o produto escalar (P2 - P1)*(P - P1) >= 0 e (P1 - P2)*(P - P2) >= 0.
			pp1 = (xbCvs - xaCvs)*(i - xaCvs) + (ybCvs - yaCvs)*(j - yaCvs);
			pp2 = (xaCvs - xbCvs)*(i - xbCvs) + (yaCvs - ybCvs)*(j - ybCvs);
			if (pp1 >= 0 && pp2 >= 0) {
				// se sim, calcular a distancia deste ponto ate a reta. baseado nessa distancia, colorir o pixel com esta cor.
				// d = ||(P2 - P1) x (P - P1)||/||(P2 - P1)||.
				d = abs((xbCvs - xaCvs)*(j - yaCvs) - (i - xaCvs)*(ybCvs - yaCvs))/sqrt((xbCvs - xaCvs)*(xbCvs - xaCvs) + (ybCvs - yaCvs)*(ybCvs - yaCvs));
				if (d < _sw) {
					// se o ponto estiver dentro da distancia especificada, pintar da cor escolhida.
					*(G->at(i, j)) = _color;
					posession->at<uchar>(i, j) = 2;
				} else if (d < _sw + 1) {
					// se estiver a ate 1 pixel de distancia, interpolar com a cor de fundo.
					if (posession->at<uchar>(i, j) == 0) {
						G->at(i,j)->operator[](0) = (uchar)((G->at(i,j)->operator[](0) - _color[0])*(d - _sw) + _color[0]);
						G->at(i,j)->operator[](1) = (uchar)((G->at(i,j)->operator[](1) - _color[1])*(d - _sw) + _color[1]);
						G->at(i,j)->operator[](2) = (uchar)((G->at(i,j)->operator[](2) - _color[2])*(d - _sw) + _color[2]);
						posession->at<uchar>(i, j) = 1;
					} else if (posession->at<uchar>(i, j) == 1) {
						G->at(i,j)->operator[](0) = (uchar)(((G->at(i,j)->operator[](0) - _color[0])*(d - _sw) + _color[0] + G->at(i,j)->operator[](0))/2);
						G->at(i,j)->operator[](1) = (uchar)(((G->at(i,j)->operator[](1) - _color[1])*(d - _sw) + _color[0] + G->at(i,j)->operator[](1))/2);
						G->at(i,j)->operator[](2) = (uchar)(((G->at(i,j)->operator[](2) - _color[2])*(d - _sw) + _color[0] + G->at(i,j)->operator[](2))/2);
					}
				}
			} else {
				// se o ponto nao puder ser projetado, testar se fará parte da ponta arredondada da linha.
				if (pp1 < 0) { // se o produto escalar (P2 - P1)*(P - P1) < 0, entao o ponto esta mais proximo de P1.
					d = sqrt((xaCvs - i)*(xaCvs - i) + (yaCvs - j)*(yaCvs - j));
				} else { // se nao, esta mais proximo de P2.
					d = sqrt((xbCvs - i)*(xbCvs - i) + (ybCvs - j)*(ybCvs - j));
				}
				if (d < _sw) { // mesma ideia do caso anterior.
					*(G->at(i, j)) = _color;
					posession->at<uchar>(i, j) = 2;
				} else if (d < _sw + 1) {
					if (posession->at<uchar>(i, j) == 0) {
						G->at(i,j)->operator[](0) = (uchar)((G->at(i,j)->operator[](0) - _color[0])*(d - _sw) + _color[0]);
						G->at(i,j)->operator[](1) = (uchar)((G->at(i,j)->operator[](1) - _color[1])*(d - _sw) + _color[1]);
						G->at(i,j)->operator[](2) = (uchar)((G->at(i,j)->operator[](2) - _color[2])*(d - _sw) + _color[2]);
						posession->at<uchar>(i, j) = 1;
					} else if (posession->at<uchar>(i, j) == 1) {
						G->at(i,j)->operator[](0) = (uchar)(((G->at(i,j)->operator[](0) - _color[0])*(d - _sw) + _color[0] + G->at(i,j)->operator[](0))/2);
						G->at(i,j)->operator[](1) = (uchar)(((G->at(i,j)->operator[](1) - _color[1])*(d - _sw) + _color[0] + G->at(i,j)->operator[](1))/2);
						G->at(i,j)->operator[](2) = (uchar)(((G->at(i,j)->operator[](2) - _color[2])*(d - _sw) + _color[0] + G->at(i,j)->operator[](2))/2);
					}
				}
			}
		}
	}
}

void Line::setXa(double xa) {
	_xa = xa;
}

double Line::xa() {
	return _xa;
}

void Line::setYa(double ya) {
	_ya = ya;
}

double Line::ya() {
	return _ya;
}

void Line::setXb(double xb) {
	_xb = xb;
}

double Line::xb() {
	return _xb;
}

void Line::setYb(double yb) {
	_yb = yb;
}

double Line::yb() {
	return _yb;
}

void Line::setSw(double sw) {
	_sw = sw;
}

double Line::sw() {
	return _sw;
}

void Line::setColor(cv::Vec3b color) {
	_color = color;
}

cv::Vec3b Line::color() {
	return _color;
}