if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif ()

project(dash)
cmake_minimum_required(VERSION 3.11)
include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -fPIC -march=native")
set(CMAKE_ENABLE_COMPILE_COMMANDS "ON")

message("-- Release mode, all optimizations enabled")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -Ofast")


##################### PMDK ####################
set(PMDK_PREFIX "build/pmdk")
# ExternalProject_Add(PMDK
# 	      BUILD_IN_SOURCE 1
#         BUILD_COMMAND $(MAKE) EXTRA_CFLAGS=-Wno-error
#         PREFIX ${PMDK_PREFIX}
#         CONFIGURE_COMMAND ""
#         INSTALL_COMMAND ""
#         LOG_DOWNLOAD ON
#         LOG_CONFIGURE ON
#         LOG_BUILD ON
#         )

include_directories(${PMDK_PREFIX}/src/PMDK/src/include)
# if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  # link_directories(${PMDK_PREFIX}/src/PMDK/src/debug)
# else ()
link_directories(${PMDK_PREFIX}/src/PMDK/src/nondebug)
link_directories(${PMDK_PREFIX}/src/PMDK/src/libpmemobj)

include_directories(${PMDK_PREFIX}/src/PMDK/src/libpmemobj)

# endif ()

# FetchContent_Declare(
#         epoch_reclaimer
#         GIT_REPOSITORY https://github.com/XiangpengHao/epoch-reclaimer.git
#         GIT_TAG master
# )
# FetchContent_GetProperties(epoch_reclaimer)
# if (NOT epoch_reclaimer_POPULATED)
#   FetchContent_Populate(epoch_reclaimer)
include_directories(build/epoch_reclaimer)
# endif ()

option(USE_PMEM "enable persistent memory support" ON)

set(libs_to_link
        pthread gflags)

message(STATUS "persistent memory support enabled, going to build with PMDK")
add_definitions(-DPMEM)
# list(APPEND libs_to_link pmemobj pmem)
# link_libraries(libs_to_link pmemobj pmem)

# add_executable(test_pmem src/test_pmem.cpp)
# # target_link_libraries(test_pmem PRIVATE ${libs_to_link})
# target_link_libraries(example PRIVATE ${libs_to_link})
# # add_dependencies(test_pmem PMDK)
# add_dependencies(example PMDK)
# add_dependencies(dr PMDK)
# add_executable(dr src/d_dash.cpp)
# target_link_libraries(dr PRIVATE ${libs_to_link})


add_library(
	dash
	STATIC
	src/dash.cpp
)

install (TARGETS dash DESTINATION .)