set(OpenCV_DIR C:/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

add_library(cpplot
	src/cpplot.cpp
	src/drawing.cpp
	src/line.cpp
	src/arrow.cpp
	src/func.cpp
)

target_link_libraries(cpplot ${OpenCV_LIBS})