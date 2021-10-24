cmake_minimum_required(VERSION 3.0.0)
project(CG-Clip VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)
include_directories(headers)
link_directories(lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(SOURCE_FILES headers/Exception.hpp
                 headers/init.h
                 headers/Logger.h
                 headers/RenderableObject.h
                 headers/SimpleShaderCompiler.h
                 headers/InputHandler.h
                 headers/Component.h
                 headers/Window.h
                 headers/Mesh.h
                 headers/Clipper.h
                 headers/ScanLineAlgo.h
                 headers/EdgeAndPoint.h
                 headers/LineWidthComp.h
                 headers/PixSizeComp.h
                 headers/Gizmos.h
                 headers/PolygenController.h
                 
                 src/Logger.cpp
                 src/RenderableObject.cpp
                 src/SimpleShaderCompiler.cpp
                 src/InputHandler.cpp
                 src/Component.cpp
                 src/Mesh.cpp
                 src/Window.cpp
                 src/Clipper.cpp
                 src/ScanLineAlgo.cpp
                 src/PolygenController.cpp
                 src/DrawConfig.cpp
                 src/Gizmos.cpp
                 )

add_executable(CG-Clip main.cpp glad.c ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw3)