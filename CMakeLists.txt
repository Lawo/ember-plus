cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ember-plus VERSION 1.8.3)

add_subdirectory(libs101)
add_subdirectory(libformula)

add_subdirectory(libember)
add_subdirectory(libember_slim)

add_subdirectory(tinyember)
