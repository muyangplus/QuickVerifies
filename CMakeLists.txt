# CMakeList.txt: QuickVerifies 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。

# 需要 Cmake 3.8 及以上版本
cmake_minimum_required (VERSION 3.8)

# 项目名
project ("QuickVerifies")

# 根据系统设置 OpenCV 的路径
# 需要手动配置
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(OpenCV_DIR "D:/opencv/build")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(OpenCV_DIR "/usr/local/share/opencv4/")
endif ()

# 根据模式设置程序名
set(EXECUTABLE_NAME "demo")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(EXECUTABLE_NAME "${EXECUTABLE_NAME}_debug")
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
	set(EXECUTABLE_NAME "${EXECUTABLE_NAME}_release")
endif ()

# 需要 OpenCV 4.5.5 库
find_package(OpenCV 4.5.5 REQUIRED)

# 包含目录
include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS}/opencv2)

# 资源文件
set(SOURCE_FILES
	src/core/capture.h
	src/core/chcp.h
	src/core/register.h
	src/core/face_recognize.h
	src/core/mask_recognize.h
	src/core/result_back.h
	
	src/core/capture.cpp
	src/core/chcp.cpp
	src/core/register.cpp
	src/core/face_recognize.cpp
	src/core/mask_recognize.cpp
	src/core/result_back.cpp
)

# 连接静态库
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# 设置可执行文件
add_executable(${EXECUTABLE_NAME} test/main.cpp)

# 连接 OpenCV 库
target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME} ${OpenCV_LIBS})

# 设置生成文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)




