cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE Sraphy_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB_RECURSE Sraphy_HEADERS "${CMAKE_CURRENT_LIST_DIR}/*.h")

# Add source to this project's executable.
if(${BUILD_SHARED_LIBS})
    list(REMOVE_ITEM Sraphy_SOURCES "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
    list(REMOVE_ITEM Sraphy_SOURCES "${CMAKE_CURRENT_LIST_DIR}/editor.cpp")
    add_library(Sraphy SHARED ${Sraphy_SOURCES})
else()
    add_executable(Sraphy ${Sraphy_SOURCES})
endif()

target_compile_features(Sraphy PRIVATE cxx_std_20)

target_link_libraries(
    Sraphy
        PRIVATE
            fmt::fmt
            glfw
            glm::glm
            glad::glad
            imgui::imgui
)

target_precompile_headers(Sraphy PRIVATE "srpch.h")
