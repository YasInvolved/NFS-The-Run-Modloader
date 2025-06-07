cmake_minimum_required(VERSION 3.23)

project(NFS-The-Run-Modloader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT WIN32)
	message(FATAL_ERROR "This project can be built only for windows machines")
endif()

get_filename_component(NFS_MODLOADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
message(STATUS "NFS_MODLOADER_PATH: ${NFS_MODLOADER_PATH}")

add_subdirectory(3rdparty)

add_subdirectory(src/NFSModloader)