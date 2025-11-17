cmake_minimum_required(VERSION 4.0)
project(koikoi)

set(CMAKE_CXX_STANDARD 20)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/main.cpp)
add_executable(koikoi ${ENGINE_SOURCES}


)

# Always include vendor headers
target_include_directories(koikoi PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glfw-3.4/include
        ${CMAKE_SOURCE_DIR}/vendor/glm
        ${CMAKE_SOURCE_DIR}/vendor/OpenXR-SDK/include
        ${CMAKE_SOURCE_DIR}/vendor/

)

if (WIN32)
    # Force console subsystem for MinGW with multiple flags
    target_link_options(koikoi PRIVATE
            -mconsole
            -Wl,--subsystem,console
    )

    # Vulkan setup
    target_include_directories(koikoi PRIVATE $ENV{VULKAN_SDK}/Include)

    # Set full paths to libraries
    set(GLFW_LIB "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4/src/libglfw3.a")
    set(VULKAN_LIB "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
    set(OPENXR_LIB "${CMAKE_SOURCE_DIR}/vendor/OpenXR-SDK/build/src/loader/libopenxr_loader.a")

    target_link_libraries(koikoi
            PRIVATE
            "${GLFW_LIB}"
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

add_custom_command(TARGET koikoi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:koikoi>/shaders
)
