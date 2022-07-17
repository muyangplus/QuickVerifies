# CMakeList.txt: QuickVerifies 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("QuickVerifies")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(EXECUTABLE_NAME "demo")
	set(OpenCV_DIR "D:/opencv/build")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(EXECUTABLE_NAME "demo")
	set(OpenCV_DIR "/usr/local/share/opencv4/")
endif ()

find_package(OpenCV 4.5.5 REQUIRED)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS}/opencv2)

set(SOURCE_FILES
	src/core/capture.h
	src/core/register.h
	src/core/face_recognize.h
	src/core/mask_recognize.h
	src/core/result_back.h
	
	src/core/capture.cpp
	src/core/register.cpp
	src/core/face_recognize.cpp
	src/core/mask_recognize.cpp
	src/core/result_back.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} "src/core/register.cpp")

add_executable(${EXECUTABLE_NAME} test/main.cpp "src/core/register.cpp")

target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME} ${OpenCV_LIBS})


