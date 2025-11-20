cmake_minimum_required(VERSION 3.20)  # Not 4.0 - that doesn't exist yet
project(koikoi)

set(CMAKE_CXX_STANDARD 20)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/main.cpp)
add_executable(koikoi ${ENGINE_SOURCES})

# Always include vendor headers
target_include_directories(koikoi PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glm
        ${CMAKE_SOURCE_DIR}/vendor/OpenXR-SDK/include
        ${CMAKE_SOURCE_DIR}/vendor/
)

# Configure GLFW build options (disable docs, tests, examples)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Build GLFW from source
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw-3.4)

if (WIN32)
    # Force console subsystem for MinGW
    target_link_options(koikoi PRIVATE
            -mconsole
            -Wl,--subsystem,console
    )

    # Vulkan setup
    target_include_directories(koikoi PRIVATE $ENV{VULKAN_SDK}/Include)

    set(VULKAN_LIB "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
    set(OPENXR_LIB "${CMAKE_SOURCE_DIR}/vendor/OpenXR-SDK/build/src/loader/libopenxr_loader.a")

    target_link_libraries(koikoi
            PRIVATE
            glfw  # Use the CMake target, not the .a file path
            "${VULKAN_LIB}"
            "${OPENXR_LIB}"
    )

elseif (UNIX)
    find_package(Vulkan REQUIRED)
    target_include_directories(koikoi PRIVATE ${Vulkan_INCLUDE_DIRS})

    target_link_libraries(koikoi
            PRIVATE
            glfw
            ${Vulkan_LIBRARIES}
            OpenXR::openxr_loader
    )
endif()

target_compile_options(koikoi PRIVATE
        -Wall -Wextra -Wpedantic
)

add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying shader files"
)
add_dependencies(koikoi copy_shaders)
