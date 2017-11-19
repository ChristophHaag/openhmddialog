cmake_minimum_required(VERSION 2.8)

project (openhmddialog)

#add_subdirectory(./external/kiss_sdl)

include_directories("${CMAKE_SOURCE_DIR}/external/nuklear_cross")

add_library(openhmddialog STATIC
  openhmddialog.cpp
)

find_package(SDL2 REQUIRED)
target_link_libraries(openhmddialog
  ${SDL2_LIBRARIES}
  GL
  openhmd
)

add_executable(openhmddialog-demo
  openhmddialog-demo.cpp
)
target_link_libraries(openhmddialog-demo
  openhmddialog
)
