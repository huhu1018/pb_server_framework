cmake_minimum_required(VERSION 3.14)
project(pb)


# 设置可执行文件的输出目录为 bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置库文件的输出目录为 lib/
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(LIB_SRC
    src/log.cpp
)

include_directories(src/)

add_library(pb SHARED ${LIB_SRC})


add_executable(test tests/test.cpp)
target_link_libraries(test pb)

add_executable(test_boost tests/test_boost.cpp)
add_executable(test_yaml tests/test_yaml.cpp)
target_link_libraries(test_yaml yaml-cpp)