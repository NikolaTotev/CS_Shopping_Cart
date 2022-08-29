cmake_minimum_required(VERSION 3.21.0)

project(RedEyeFiltering VERSION 1.0.0)


add_executable("${PROJECT_NAME}" "main.cpp")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)

install(FILES "main.cpp" DESTINATION src)