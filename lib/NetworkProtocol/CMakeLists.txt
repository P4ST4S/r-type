cmake_minimum_required(VERSION 3.25)
project(Cross-Platform-Network-Lib)

set(LIB_NAME "NetworkProtocol")

# Create source files list
file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
set(SOURCES ${CPP_SOURCES})

# Create include folder list
set(INCLUDES "include")
file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES}/*")
foreach(SUBFOLDER ${SUBFOLDERS})
    if(IS_DIRECTORY ${SUBFOLDER})
        list(APPEND INCLUDES ${SUBFOLDER})
    endif()
endforeach()
list(APPEND INCLUDES "../CrossPlatformNetwork/include/")
file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "../CrossPlatformNetwork/include/*")
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
target_include_directories(${LIB_NAME} PUBLIC ${INCLUDES})

# add_subdirectory("../CrossPlatformNetwork/" "CrossPlatformNetwork_build")
target_link_libraries(${LIB_NAME} PRIVATE CrossPlatformNetwork)
