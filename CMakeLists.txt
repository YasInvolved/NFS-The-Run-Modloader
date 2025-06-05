cmake_minimum_required(VERSION 3.23)

project(NFS-The-Run-Modloader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT WIN32)
	message(FATAL_ERROR "This project can be built only for windows machines")
endif()

add_subdirectory(3rdparty)

add_subdirectory(src/NFSModloader)