cmake_minimum_required(VERSION 3.14)
project(pb)

set(LIB_SRC
    src/log.cpp
)

include_directories(src/)

add_library(pb SHARED ${LIB_SRC})

# 设置可执行文件的输出目录为 bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(test tests/test.cpp)
target_link_libraries(test pb)
