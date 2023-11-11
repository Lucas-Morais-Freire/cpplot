set(OpenCV_DIR C:/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

add_compile_options(/W4)

add_library(cpplot
	src/cpplot.cpp
	src/line.cpp
	src/arrow.cpp
	src/func.cpp
	src/axis.cpp
)

target_link_libraries(cpplot ${OpenCV_LIBS})