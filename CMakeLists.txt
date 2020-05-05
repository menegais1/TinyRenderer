cmake_minimum_required(VERSION 3.16)
project(TinyRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(TinyRenderer Vectors/Vector3.h
        Vectors/Vector2.h Vectors/Vector4.h main.cpp tgaimage.cpp tgaimage.h
        Model.cpp Model.h Render.cpp Render.h Vectors/Matrix.h Camera.cpp Camera.h IShader.h)