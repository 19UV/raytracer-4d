cmake_minimum_required(VERSION 3.10)
project(Raytracer4D
	LANGUAGES C
	VERSION 0.1
)

################## 
# Setup Compiler # 
################## 

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
	set(DEFINE ${DEFINE} _CRT_SECURE_NO_WARNINGS)
endif()

if(NOT MSVC)
	# We need to link with the C maths library (m)
	# which Windows graciously proceeds to have a panic
	# attack if it comes within 50 feet of
	set(DEPEND ${DEPEND} m)
endif()

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
	src/malloc.c

	src/util/random.c

	src/objects/group.c
	src/objects/hypersphere.c
	src/objects/hyperplane.c

	src/scene_deserializer.c
	src/scene_print.c

	src/image.c

	src/main.c
)

add_executable(raytracer4d ${FILES})
target_include_directories(raytracer4d PRIVATE include/)
target_link_libraries(raytracer4d PRIVATE ${DEPEND})
target_compile_definitions(raytracer4d PRIVATE ${DEFINE})
