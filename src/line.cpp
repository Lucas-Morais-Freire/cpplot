#include "../headers/line.hpp"
#include "../headers/cpplot.hpp"
#include <iostream>
#include <regex>

Line::Line(double xa, double ya, double xb, double yb, cv::Vec3b color) {
	// initialize obligatory values:
		_xa = xa;
		_ya = ya;
		_xb = xb;
		_yb = yb;
		_color = color;

	// initialize optional values:
		_stroke_weight = 0.5;
}

void Line::draw(Graph* G) {
	cv::Mat* original = new cv::Mat;
	this->draw(G, original);
	delete original;
}

void Line::draw(Graph* G, cv::Mat* original) {
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
	int x1 = (int)(xmin - _stroke_weight) < 0 ? 0 : ((int)(xmin - _stroke_weight) >= canvas->rows ? canvas->rows - 1 : (int)(xmin - _stroke_weight));
	int y1 = (int)(ymin - _stroke_weight) < 0 ? 0 : ((int)(ymin - _stroke_weight) >= canvas->cols ? canvas->cols - 1 : (int)(ymin - _stroke_weight));
	int x2 = (int)(xmax + _stroke_weight) + 1 >= canvas->rows ? canvas->rows - 1 : ((int)(xmax + _stroke_weight) + 1 < 0 ? 0 : (int)(xmax + _stroke_weight) + 1);
	int y2 = (int)(ymax + _stroke_weight) + 1 >= canvas->cols ? canvas->cols - 1 : ((int)(ymax + _stroke_weight) + 1 < 0 ? 0 : (int)(ymax + _stroke_weight) + 1);

	double d;
	double pp1;
	double pp2;
	uchar proposed[3];
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

			} else if (pp1 < 0) {// se o ponto nao puder ser projetado, testar se fará parte da ponta arredondada da linha.

				// se o produto escalar (P2 - P1)*(P - P1) < 0, entao o ponto esta mais proximo de P1.
				d = sqrt((xaCvs - i)*(xaCvs - i) + (yaCvs - j)*(yaCvs - j));

			} else { // se nao, esta mais proximo de P2.
				d = sqrt((xbCvs - i)*(xbCvs - i) + (ybCvs - j)*(ybCvs - j));
			}

			if (d < _stroke_weight) {
				// se o ponto estiver dentro da distancia especificada, pintar com a cor escolhida.
				canvas->at<cv::Vec3b>(i, j) = _color;
				// Sinalizar que este ponto já foi utilizado pela imagem para uma cor sólida.		
				original->at<cv::Vec4b>(i, j)[3] = 254;
			} else if (d < _stroke_weight + 1) { // se estiver a ate 1 pixel de distancia...
				//... tentar interpolar com a cor de fundo original...
				proposed[0] = (uchar)(((int)original->at<cv::Vec4b>(i,j)[0] - (int)_color[0])*(d - _stroke_weight) + _color[0]);
				proposed[1] = (uchar)(((int)original->at<cv::Vec4b>(i,j)[1] - (int)_color[1])*(d - _stroke_weight) + _color[1]);
				proposed[2] = (uchar)(((int)original->at<cv::Vec4b>(i,j)[2] - (int)_color[2])*(d - _stroke_weight) + _color[2]);
				if (original->at<cv::Vec4b>(i, j)[3] == 255 || original->at<cv::Vec4b>(i, j)[3] == 253 && ((_color[0] == canvas->at<cv::Vec3b>(i,j)[0] ? false : (_color[0] < canvas->at<cv::Vec3b>(i,j)[0] ? (proposed[0] < canvas->at<cv::Vec3b>(i,j)[0]) : (proposed[0] > canvas->at<cv::Vec3b>(i,j)[0]))) || (_color[1] == canvas->at<cv::Vec3b>(i,j)[1] ? false : (_color[1] < canvas->at<cv::Vec3b>(i,j)[1] ? (proposed[1] < canvas->at<cv::Vec3b>(i,j)[1]) : (proposed[1] > canvas->at<cv::Vec3b>(i,j)[1]))) || (_color[2] == canvas->at<cv::Vec3b>(i,j)[2] ? false : (_color[2] < canvas->at<cv::Vec3b>(i,j)[2] ? (proposed[2] < canvas->at<cv::Vec3b>(i,j)[2]) : (proposed[2] > canvas->at<cv::Vec3b>(i,j)[2]))))) {
					canvas->at<cv::Vec3b>(i,j) = {proposed[0], proposed[1], proposed[2]};
					original->at<cv::Vec4b>(i, j)[3] = 253;
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

Line& Line::setStrokeWeight(double sw) {
	_stroke_weight = sw;

	return *this;
}

double Line::strokeWeight() {
	return _stroke_weight;
}

Line& Line::setColor(cv::Vec3b color) {
	_color = color;

	return *this;
}

cv::Vec3b Line::color() {
	return _color;
}