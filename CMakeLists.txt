cmake_minimum_required(VERSION 3.1.0)

set(project_name model-view)

project(${project_name})

# ...

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_BUILD_TYPE RelWithDebInfo) # for breakpad
endif()

# Additional compiler flags

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# ...

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Lib WebAPI
# add_subdirectory(externals/ipso-webapi/library/)

# Lib Breakpad
# add_subdirectory(externals/ipso-crash-report/)

add_subdirectory(src)
