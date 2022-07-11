cmake_minimum_required(VERSION 3.10)
project(Raytracer4D
	LANGUAGES C
	VERSION 0.1
)

set(EXTERN_CGLM extern/cglm)

############## 
# Build CGLM # 
############## 

add_subdirectory(${EXTERN_CGLM})
set(DEPEND ${DEPEND} cglm)

############# 
# Add Files # 
############# 

set(FILES ${FILES}
	src/image.c

	src/main.c
)

add_executable(raytracer4d ${FILES})
target_include_directories(raytracer4d PRIVATE include/)
target_link_libraries(raytracer4d PRIVATE ${DEPEND})
