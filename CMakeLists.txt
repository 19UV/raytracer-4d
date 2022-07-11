cmake_minimum_required(VERSION 3.10)
project(Raytracer4D
	LANGUAGES C
	VERSION 0.1
)

set(FILES ${FILES}
	src/main.c
)

add_executable(raytracer4d ${FILES})
target_include_directories(raytracer4d PRIVATE include/)
