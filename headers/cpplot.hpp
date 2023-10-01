#ifndef CPPLOT_H
#define CPPLOT_H

#include "arrow.hpp"
#include "func.hpp"
#include "line.hpp"
#include "drawing.hpp"
#include "axis.hpp"
#include <opencv2/opencv.hpp>

/**
 * @brief WIP. Plotter for the C++ language.
 */
class Graph {
    protected:
        /**
         * @brief cv::Mat object that contains image data.
         */
        cv::Mat* _canvas;
        /**
         * @brief variables that specify the range of R2 values present in _canvas (protected).
         */
        double _xmin, _xmax, _ymin, _ymax;
        /**
         * @brief cv::Vec3b object specifying background color.
         */
        cv::Vec3b _bgColor;
        /**
         * @brief list of objects drawn on _canvas (protected).
         */
        std::list<Drawing*>* _drawOrder;

        std::list<std::string> _keys = {"xres", "yres"};
         int _xres, _yres;

        void init(const char* params);
        void assign(std::string key, std::string arg);
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
        Graph(double xmin, double xmax, double ymin, double ymax, cv::Vec3b bgColor = {255,255,255});
        /**
         * @brief Construct a new Graph object. Not specifying a resolution will instantiate a 854x480 px canvas.
         * 
         * @param xmin image will only display x values greater than this.
         * @param xmax image will only display x values less than this.
         * @param ymin image will only display y values greater than this.
         * @param ymax image will only display y values less than this.
         * @param bgColor cv::Vec3b object containing color values for the background in BGR format.
         */
        Graph(double xmin, double xmax, double ymin, double ymax, const char* params, cv::Vec3b bgColor = {255,255,255});
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
        void setRes(int xres, int yres);
        /**
         * @brief returns the horizontal length of the image.
         * 
         * @return unsigned int
         */
        int xres();
        /**
         * @brief returns the vertical length of the image.
         * 
         * @return unsigned int
         */
        int yres();

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

        void drawLine(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0,0,0});
        void drawLine(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0});

        //drawArrow and its overloads.
        void drawArrow(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0,0,0});
        void drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0});
        
        //drawFunc and its overloads.
        void drawFunc(double (*func)(double), std::string params, cv::Vec3b color = {0,0,0});
        void drawFunc(double (*func)(double), cv::Vec3b color = {0,0,0});

        //drawAxis and its overloads.
        void drawAxis(bool x_axis, std::string params, cv::Vec3b color = {0,0,0});
        void drawAxis(bool x_axis, cv::Vec3b color = {0,0,0});
        /**
         * @brief writes image to the specified path.
         * 
         * @param filename path to target image file.
         */
        void write(const char* filename);
};

#endif