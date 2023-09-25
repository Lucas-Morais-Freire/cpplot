#ifndef CPPLOT_H
#define CPPLOT_H

#include <opencv2/opencv.hpp>
#include "drawing.hpp"
#include "line.hpp"
#include "arrow.hpp"

/**
 * @brief WIP. Plotter for the C++ language.
 */
class Graph {
    protected:
        /**
         * @brief cv::Mat object that contains image data.
         */
        cv::Mat canvas;
        /**
         * @brief variables that specify the range of R2 values present in this->canvas (private).
         */
        double _xmin, _xmax, _ymin, _ymax;
        /**
         * @brief cv::Vec3b object specifying background color.
         */
        cv::Vec3b _bgColor;
        /**
         * @brief list of objects drawn on this->canvas (private).
         */
        std::list<Drawing*>* _drawOrder;
    public:
        //constructors
        /**
         * @brief Construct a new Graph object. initializing without any parameters will create a default canvas of 854x480 px with a
         * white background and a range -5 <= x <= 5 and -5 <= y <= 5.
         */
        Graph();
        /**
         * @brief Construct a new Graph object.
         * 
         * @param xmin image will only display x values greater than this.
         * @param xmax image will only display x values less than this.
         * @param ymin image will only display y values greater than this.
         * @param ymax image will only display y values less than this.
         * @param xres horizontal length of the image in pixels.
         * @param yres vertical length of the image in pixels
         * @param bgColor background color.
         */
        Graph(double xmin, double xmax, double ymin, double ymax, uint xres, uint yres, cv::Vec3b bgColor = {255,255,255});
        /**
         * @brief Construct a new Graph object. Not specifying a resolution will instantiate a 854x480 px canvas.
         * 
         * @param xmin image will only display x values greater than this.
         * @param xmax image will only display x values less than this.
         * @param ymin image will only display y values greater than this.
         * @param ymax image will only display y values less than this.
         * @param bgColor cv::Vec3b object containing color values for the background in BGR format.
         */
        Graph(double xmin, double xmax, double ymin, double ymax, cv::Vec3b bgColor = {255,255,255});
        /**
         * @brief Destroy the Graph object.
         * 
         */
        ~Graph();

        //functions pertaining to resolution control
        /**
         * @brief Set a new resolution to the image. this will scale the image accordingly.
         * 
         * @param xres horizontal length of the image in pixels.
         * @param yres vertical length of the image in pixels
         */
        void setRes(uint xres, uint yres);
        /**
         * @brief returns the horizontal length of the image.
         * 
         * @return unsigned int
         */
        uint xres();
        /**
         * @brief returns the vertical length of the image.
         * 
         * @return unsigned int
         */
        uint yres();

        //functions pertaining to range of what is being shown on the graph
        /**
         * @brief Sets the range of values in R2 space that will be displayed on the image.
         * 
         * @param xmin image will only display x values greater than this.
         * @param xmax image will only display x values less than this.
         * @param ymin image will only display y values greater than this.
         * @param ymax image will only display y values less than this.
         */
        void setRange(double xmin, double xmax, double ymin, double ymax);
        /**
         * @brief returns the upper horizontal bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double xmax();
        /**
         * @brief returns the lower horizontal bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double xmin();
        /**
         * @brief returns the upper vertical bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double ymax();
        /**
         * @brief returns the lower vertical bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double ymin();

        //functions that convert between index space and R2 space.
        /**
         * @brief given a y coordinate in R2 space, returns the correspondent row value in matrix space. The user must decide
         * how to handle a conversion to integer if needed.
         * 
         * @param y y coordinate in R2 space.
         * @return double 
         */
        double iIdx(double y);
        /**
         * @brief given an x coordinate in R2 space, returns the correspondent column value in matrix space. The user must decide
         * how to handle a conversion to integer if needed.
         * 
         * @param x x coordinate in R2 space.
         * @return double 
         */
        double jIdx(double x);
        /**
         * @brief given a column value in matrix space, returns the correspondent x value in R2 space.
         * 
         * @param j column value in matrix space.
         * @return double 
         */
        double xpos(double j);
        /**
         * @brief given a row value in matrix space, returns the correspondent y value in R2 space.
         * 
         * @param i row value in matrix space.
         * @return double 
         */
        double ypos(double i);

        //functions pertaining to the background color.
        /**
         * @brief sets image background color to desired value.
         * 
         * @param bgColor cv::Vec3b object containing color values in BGR format.
         */
        void setBGColor(cv::Vec3b bgColor);
        /**
         * @brief returns cv::Vec3b object containing background color values in BGR format.
         * 
         * @return cv::Vec3b
         */
        cv::Vec3b bgColor();

        /**
         * @brief Set a particular pixel of the graph to the desired color.
         * 
         * @param i row index of target pixel.
         * @param j column index of target pixel.
         * @return cv::Vec3b* pointer to vector containing color values in BGR format.
         */
        cv::Vec3b* at(int i, int j);

        //drawLine and its overloads.
        /**
         * @brief draws a line between the endpoints (xa, ya) and (xb, yb) in R2 space.
         * 
         * @param xa x coordinate of the first endpoint.
         * @param ya y coordinate of the first endpoint.
         * @param xb x coordinate of the second endpoint.
         * @param yb y coordinate of the second endpoint.
         * @param color color of the line.
         * @param sw half-thickness of the line in pixels.
         */
        void drawLine(double xa, double ya, double xb, double yb, cv::Vec3b color, double sw);
        /**
         * @brief draws a line between the endpoints (xa, ya) and (xb, yb) in R2 space. An unspecified color will draw a black line.
         * 
         * @param xa x coordinate of the first endpoint.
         * @param ya y coordinate of the first endpoint.
         * @param xb x coordinate of the second endpoint.
         * @param yb y coordinate of the second endpoint.
         * @param sw half-thickness of the line in pixels.
         */
        void drawLine(double xa, double ya, double xb, double yb, double sw);

        //drawArrow and its overloads.
        /**
         * @brief draws an arrow between the endpoints (xa, ya) and (xb, yb) in R2 space.
         * 
         * @param xa x coordinate of the first endpoint.
         * @param ya y coordinate of the first endpoint.
         * @param xb x coordinate of the second endpoint.
         * @param yb y coordinate of the second endpoint.
         * @param color color of the lines.
         * @param sw half-thickness of the lines in pixels.
         * @param hsize size of the arrow-head in pixels.
         * @param angle angle of the arrow-head strokes in pixels.
         */
        void drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color, double sw, double hsize, double angle);
        /**
         * @brief draws an arrow between the endpoints (xa, ya) and (xb, yb) in R2 space. An unspecified color will draw a black line.
         * and a unspecified angle will default to 20 degrees.
         * 
         * @param xa x coordinate of the first endpoint.
         * @param ya y coordinate of the first endpoint.
         * @param xb x coordinate of the second endpoint.
         * @param yb y coordinate of the second endpoint.
         * @param sw half-thickness of the lines in pixels.
         * @param hsize size of the arrow-head in pixels.
         * @param angle angle of the arrow-head strokes in pixels.
         */
        void drawArrow(double xa, double ya, double xb, double yb, double sw, double hsize, double angle = 20);

        /**
         * @brief writes image to the specified path.
         * 
         * @param filename path to target image file.
         */
        void write(const char* filename);
};

#endif