cmake_minimum_required(VERSION 3.29)
project(WebGPUExamples)
set(CMAKE_CXX_STANDARD 23)

find_package(Dawn)
find_package(glfw3)

message(STATUS "${PROJECT_NAME} - Using GLFW: Version ${glfw3_VERSION}")

add_executable(HelloTriangleSimple HelloTriangleSimple.cpp)
target_link_libraries(HelloTriangleSimple PRIVATE dawn::webgpu_dawn glfw SomeApp)
add_custom_command(TARGET HelloTriangleSimple
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_executable(HelloTriangleAdvanced HelloTriangleAdvanced.cpp)
target_link_libraries(HelloTriangleAdvanced PRIVATE dawn::webgpu_dawn glfw SomeApp)
add_custom_command(TARGET HelloTriangleAdvanced
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(WEBGPU_EXAMPLES_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
else ()
    set(WEBGPU_EXAMPLES_RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)
endif ()

target_compile_definitions(HelloTriangleSimple PRIVATE RESOURCES="${WEBGPU_EXAMPLES_RESOURCE_DIR}")
target_compile_definitions(HelloTriangleAdvanced PRIVATE RESOURCES="${WEBGPU_EXAMPLES_RESOURCE_DIR}")
