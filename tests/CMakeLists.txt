cmake_minimum_required(VERSION 3.25)
project(Tests)

include (FetchContent)
FetchContent_Declare (
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS "--coverage")
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )



# Create source files list
file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
set(SOURCES ${CPP_SOURCES})

# Create include folder list
set(INCLUDES "../include")
set(INCLUDES_TEST "./include")
set(INCLUDES_LIB "../lib")

file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES_TEST}/*")
foreach(SUBFOLDER ${SUBFOLDERS})
if(IS_DIRECTORY ${SUBFOLDER})
list(APPEND INCLUDES ${SUBFOLDER})
endif()
endforeach()
list(APPEND INCLUDES ${INCLUDES_TEST})

# file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES}/*")
# foreach(SUBFOLDER ${SUBFOLDERS})
#     if(IS_DIRECTORY ${SUBFOLDER})
#         list(APPEND INCLUDES ${SUBFOLDER})
#     endif()
# endforeach()

# file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES_LIB}/*")
# foreach(SUBFOLDER ${SUBFOLDERS})
#     if(IS_DIRECTORY ${SUBFOLDER})
#         list(APPEND INCLUDES ${SUBFOLDER})
#     endif()
# endforeach()

# Display lists
message("All source files :")
foreach(SOURCE ${SOURCES})
    message("\t${SOURCE}")
endforeach()
message("All include folders :")
foreach(INCLUDE ${INCLUDES})
    message("\t${INCLUDE}")
endforeach()
message("\n\n")

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC
Catch2::Catch2
Catch2::Catch2WithMain)


add_subdirectory("../lib/CrossPlatformNetwork/" "CrossPlatformNetwork_build")
target_link_libraries(${PROJECT_NAME} PRIVATE CrossPlatformNetwork)

add_subdirectory("../lib/NetworkProtocol/" "NetworkProtocol_build")
target_link_libraries(${PROJECT_NAME} PRIVATE NetworkProtocol)

add_subdirectory("../lib/GameEngine/" "EntityComponentSystem_build")
target_link_libraries(${PROJECT_NAME} PRIVATE GameEngine)

target_link_options(${PROJECT_NAME} PRIVATE -g -g3 --std=c++17)
