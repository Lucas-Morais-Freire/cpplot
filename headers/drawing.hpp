#ifndef CPPLOT_DRAWING_H
#define CPPLOT_DRAWING_H

#include <opencv2/opencv.hpp>

class Graph;

class Drawing {
    protected:
        cv::Vec3b _color;
        std::list<std::string> _keys;
        /**
         * @brief Responsible for validating parameter string using the keyword-argument pattern. calls this->assign()
         * so that the individual objects properly validate their format and perform assignment.
         * 
         * @param params std::string containing optional parameters for a given object type. keyword-argument pattern is used:
         * "key1 = val1, key2 = val2, ..."
         * @return std::list<std::string>* containing the keys that were not initialized by the user.
         */
        std::list<std::string>* init(std::string params);
        /**
         * @brief Responsible for assigning the value "arg" to the attribute "key". Validates "arg" and performs the correspondent
         * assignment.
         * 
         * @param key Name of the attribute that will be assigned the value of "arg".
         * @param arg Value that will be assigned to the correspondent "key".
         */
        virtual void assign(std::string key, std::string arg) = 0;
    public:
        virtual void draw(Graph* G) = 0;
        virtual ~Drawing() = default;
};

#endif