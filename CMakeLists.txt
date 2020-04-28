cmake_minimum_required(VERSION 3.16)
project(TinyRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(TinyRenderer Vectors/Vector3.cpp Vectors/Vector3.h Vectors/Vector2.cpp Vectors/Vector2.h main.cpp tgaimage.cpp tgaimage.h Model.cpp Model.h Render.cpp Render.h Vectors/Matrix.cpp Vectors/Matrix.h)