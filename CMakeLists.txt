cmake_minimum_required(VERSION 3.25)
project(R-Type)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS  "-g")
set(SERV_BIN_NAME Server)

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/Client/*.cpp")


file(GLOB_RECURSE CPP_SERV_SOURCES CONFIGURE_DEPENDS "src/Server/*.cpp")

set(SOURCES ${CPP_SOURCES})
set(SERV_SOURCES ${CPP_SERV_SOURCES})



set(INCLUDES "./include")
file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true "${INCLUDES}/*")
foreach(SUBFOLDER ${SUBFOLDERS})
    if(IS_DIRECTORY ${SUBFOLDER})
        list(APPEND INCLUDES ${SUBFOLDER})
    endif()
endforeach()

# Display lists
message("All client source files :")
foreach(SOURCE ${SOURCES})
    message("\t${SOURCE}")
endforeach()
message("All server source files :")
foreach(SOURCE ${SERV_SOURCES})
    message("\t${SOURCE}")
endforeach()
message("All include folders :")
foreach(INCLUDE ${INCLUDES})
    message("\t${INCLUDE}")
endforeach()


add_subdirectory("./lib/CrossPlatformNetwork/")
add_subdirectory("./lib/Raygui/")
add_subdirectory("./lib/NetworkProtocol/")
add_subdirectory("./lib/Clock/")
add_subdirectory("./lib/GameEngine/")
add_subdirectory("./lib/RaylibGraphic")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${SERV_BIN_NAME} ${SERV_SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_include_directories(${SERV_BIN_NAME} PRIVATE ${INCLUDES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_features(${SERV_BIN_NAME} PRIVATE cxx_std_17)

target_link_options(${PROJECT_NAME} PRIVATE -g3 -g)
target_link_options(${SERV_BIN_NAME} PRIVATE -g3 -g)

target_link_libraries(${PROJECT_NAME} PRIVATE CrossPlatformNetwork)
target_link_libraries(${SERV_BIN_NAME} PRIVATE CrossPlatformNetwork)
target_link_libraries(${PROJECT_NAME} PRIVATE Raygui)
target_link_libraries(${SERV_BIN_NAME} PRIVATE Raygui)
target_link_libraries(${PROJECT_NAME} PRIVATE NetworkProtocol)
target_link_libraries(${SERV_BIN_NAME} PRIVATE NetworkProtocol)
target_link_libraries(${PROJECT_NAME} PRIVATE Clock)
target_link_libraries(${SERV_BIN_NAME} PRIVATE Clock)
target_link_libraries(${PROJECT_NAME} PRIVATE GameEngine)
target_link_libraries(${SERV_BIN_NAME} PRIVATE GameEngine)
target_link_libraries(${PROJECT_NAME} PRIVATE RaylibGraphic)
target_link_libraries(${SERV_BIN_NAME} PRIVATE RaylibGraphic)

# RAYLIB

set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()

if(WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(Yellow "${Esc}[93m")
    set(Orange "${Esc}[33m")

    message("${Orange}")
    message("INFO: Windows being quite annoying with compatibility make sure you do the following things:\n")
    message("Use MinGW g++ compiler. You can force it by doing this command: ${Yellow}cmake -B build -G \"MinGW Makefiles\"${Orange}.")
    message("If you don't have it, here's a link to download MinGW: ${Yellow}https://winlibs.com/${Orange}.")
    message("Scrolldown to ${Yellow}release version${Orange} and download the ${Yellow}POSIX${Orange} latest one (MCF won't work).")
    message("Extract the zip file whenever you want and don't forget to ${Yellow}add the bin folder of MinGW into windows environnement variable path${Orange}.")
    message("${ColourReset}")
endif()
