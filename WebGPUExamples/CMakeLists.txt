add_executable(HelloTriangleSimple Examples/HelloTriangleSimple.cpp)
add_executable(HelloTriangleAdvanced Examples/HelloTriangleAdvanced.cpp)


target_link_libraries(HelloTriangleSimple PRIVATE SomeApp)
target_link_libraries(HelloTriangleAdvanced PRIVATE SomeApp)


# Copy resources to binary directory

CMAKE_LANGUAGE(DEFER CALL
        execute_process
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)


# Use absolute path to resources during development

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(WEBGPU_EXAMPLES_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
else ()
    set(WEBGPU_EXAMPLES_RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)
endif ()


target_compile_definitions(HelloTriangleSimple PRIVATE RESOURCES="${WEBGPU_EXAMPLES_RESOURCE_DIR}")
target_compile_definitions(HelloTriangleAdvanced PRIVATE RESOURCES="${WEBGPU_EXAMPLES_RESOURCE_DIR}")
