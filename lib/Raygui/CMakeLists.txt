cmake_minimum_required(VERSION 3.25)
project(Raygui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS  "-g")
set(LIB_NAME "Raygui")

# Create source files list
file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/*.c")
set(SOURCES ${CPP_SOURCES})

# Create include folder list
set(INCLUDES "./include")
file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES}/*")
foreach(SUBFOLDER ${SUBFOLDERS})
    if(IS_DIRECTORY ${SUBFOLDER})
        list(APPEND INCLUDES ${SUBFOLDER})
    endif()
endforeach()

# Display lists
message("All source files :")
foreach(SOURCE ${SOURCES})
    message("\t${SOURCE}")
endforeach()
message("All include folders :")
foreach(INCLUDE ${INCLUDES})
    message("\t${INCLUDE}")
endforeach()

add_library(${LIB_NAME} STATIC ${SOURCES})
target_link_libraries(${LIB_NAME} PRIVATE raylib)
target_include_directories(${LIB_NAME} PUBLIC ${INCLUDES})
target_compile_options(${LIB_NAME} PUBLIC -U RAYGUI_STANDALONE)
