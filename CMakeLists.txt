cmake_minimum_required(VERSION 3.14)
project(pb)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置可执行文件的输出目录为 bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置库文件的输出目录为 lib/
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 使用aux_source_directory来收集源文件
aux_source_directory(src LIB_SRC)

include_directories(include/)

# 查找yaml-cpp库
find_library(YAMLCPP yaml-cpp REQUIRED)

# 添加pb共享库
add_library(pb SHARED ${LIB_SRC})
target_link_libraries(pb ${YAMLCPP})

add_executable(test_log tests/test_log.cpp)
target_link_libraries(test_log pb)


# 添加test_yaml可执行文件并链接到pb库
add_executable(test_yaml tests/test_yaml.cpp)
target_link_libraries(test_yaml pb)
