cmake_minimum_required(VERSION 3.19)
project(hmi-service LANGUAGES CXX)
set(TARGET_NAME ${PROJECT_NAME})

# dependencies
#find_package(DOTS REQUIRED) (uncomment when dependency is not part of build tree)
#find_package(GspModel REQUIRED) (uncomment when dependency is not part of build tree)
find_package(OpenGL REQUIRED)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.88
)
# Check if population has already been performed
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

# target
add_executable(${TARGET_NAME})

# properties
target_sources(${TARGET_NAME}
    PRIVATE
        src/main.cpp
        src/MainWindow.cpp
        src/GlfwOpenGl3Backend.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(${TARGET_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
target_compile_options(${TARGET_NAME}
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:$<$<NOT:$<BOOL:${CMAKE_CXX_FLAGS}>>:-Wall -Wextra -Wpedantic -Werror>>
        $<$<CXX_COMPILER_ID:Clang>:$<$<NOT:$<BOOL:${CMAKE_CXX_FLAGS}>>:-Wall -Wextra -Wpedantic -Werror>>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)
target_compile_features(${TARGET_NAME}
    PRIVATE
        cxx_std_20
)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        DOTS::DOTS
        GspModel::GspModel
        fmt::fmt
        glfw
        ${OPENGL_LIBRARIES}
)

# install
install (TARGETS ${TARGET_NAME} DESTINATION sbin COMPONENT ${TARGET_NAME})
