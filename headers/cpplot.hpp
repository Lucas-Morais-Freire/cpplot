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
        Graph& setRes(int xres, int yres);
        /**
         * @brief returns the horizontal length of the image.
         * 
         * @return unsigned int
         */
        int xres();
        /**
         * @brief Sets the horizontal resolution of the canvas.
         * 
         * @param xres Horizontal resolution of the canvas.
         * @return Graph& Reference to the Graph object
         */
        Graph& setXres(int xres);
        /**
         * @brief returns the vertical length of the image.
         * 
         * @return unsigned int
         */
        int yres();
        /**
         * @brief Sets the vertical resolution of the canvas.
         * 
         * @param yres Vertical resolution of the canvas.
         * @return Graph& Reference to the Graph object.
         */
        Graph& setYres(int yres);

        //functions pertaining to range of what is being shown on the graph
        /**
         * @brief Sets the range of values in R2 space that will be displayed on the image.
         * 
         * @param xmin image will only display x values greater than this.
         * @param xmax image will only display x values less than this.
         * @param ymin image will only display y values greater than this.
         * @param ymax image will only display y values less than this.
         * 
         * @return Graph& Reference to the Graph object.
         */
        Graph& setRange(double xmin, double xmax, double ymin, double ymax);
        /**
         * @brief returns the upper horizontal bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double xmax();
        /**
         * @brief Sets the high horizontal bound in R2 space of what will be shown in the image.
         * @param xmax The image will only display x values less than this.
         * 
         * @return Graph& Reference to the Graph object.
         */
        Graph& setXmax(int xmax);
        /**
         * @brief returns the lower horizontal bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double xmin();
        /**
         * @brief Sets the lower horizontal bound in R2 space of what will be shown in the image.
         * @param xmin image will only display x values greater than this.
         * 
         * @return Graph& 
         */
        Graph& setXmin(int xmin);
        /**
         * @brief Sets the horizontal bounds in R2 space of what will be shown in the image
         * 
         * @param xmin The image will only display x values greater than this.
         * @param xmax The image will only display x values less than this.
         * @return Graph& Reference to the Graph object.
         */
        Graph& setXbounds(int xmin, int xmax);
        /**
         * @brief returns the upper vertical bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double ymax();
        /**
         * @brief Sets the upper vertical bound in R2 space of what will be shown in the image.
         * 
         * @param ymax The image will only display y values less than this.
         * @return Graph& Reference to the Graph object.
         */
        Graph& setYmax(int ymax);
        /**
         * @brief returns the lower vertical bound in R2 space of what will be shown in the image.
         * 
         * @return double 
         */
        double ymin();
        /**
         * @brief Sets the lower vertical bound in R2 space of what will be shown in the image.
         * 
         * @param ymin The image will only display y values greater than this.
         * @return Graph& Reference to the Graph object.
         */
        Graph& setYmin(int ymin);
        /**
         * @brief Sets the vertical bounds in R2 space of what will be shown in the image
         * 
         * @param ymin The image will only display y values greater than this.
         * @param ymax The image will only display y values less than this.
         * @return Graph& Reference to the Graph object.
         */ 
        Graph& setYbounds(int ymin, int ymax);

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
         * @brief returns cv::Vec3b object containing background color values in BGR format.
         * 
         * @return cv::Vec3b
         */
        cv::Vec3b bgColor();
        /**
         * @brief sets image background color to desired value.
         * 
         * @param bgColor cv::Vec3b object containing color values in BGR format.
         * 
         * @return Graph& Reference to the Graph object.
         */
        Graph& setBGColor(cv::Vec3b bgColor);

        cv::Mat* getCanvas();

        //drawLine and its overloads.

        Line& drawLine(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0});

<<<<<<< HEAD
        Arrow& drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0});
=======
        //drawArrow and its overloads.
        void drawArrow(double xa, double ya, double xb, double yb, std::string params, cv::Vec3b color = {0,0,0});
        void drawArrow(double xa, double ya, double xb, double yb, cv::Vec3b color = {0,0,0});
        
        //drawFunc and its overloads.
        void drawFunc(double (*func)(double), std::string params, cv::Vec3b color = {0,0,0});
        void drawFunc(double (*func)(double), cv::Vec3b color = {0,0,0});
        void drawFunc(std::vector<double>& x, std::vector<double>& y, std::string params, cv::Vec3b color = {0,0,0});
        void drawFunc(std::vector<double>& x, std::vector<double>& y, cv::Vec3b color = {0,0,0});
>>>>>>> origin/temp

        /**
         * @brief writes image to the specified path.
         * 
         * @param filename path to target image file.
         */
        void write(const char* filename);
};

#endif