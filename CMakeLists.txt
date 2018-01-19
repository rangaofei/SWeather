cmake_minimum_required(VERSION 3.8)
project(SWeather)
# set the version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 1)

configure_file(
        "${PROJECT_SOURCE_DIR}/SimpleWeatherConfigure.h.in"
        "${PROJECT_BINARY_DIR}/SimpleWeatherConfigure.h"
)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} ")
set(CMAKE_BUILD_TYPE Release)
IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DENABLE_DEBUG)
ENDIF (CMAKE_BUILD_TYPE STREQUAL Debug)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/CJson)
include_directories(${PROJECT_SOURCE_DIR}/ParseWeather)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/GetWeather)
include_directories(${PROJECT_SOURCE_DIR}/Location)
add_subdirectory(CJson)
add_subdirectory(ParseWeather)
add_subdirectory(GetWeather)
add_subdirectory(Location)
set(SOURCE_FILES main.c)
add_executable(sweather ${SOURCE_FILES})
target_link_libraries(sweather libcurl.dylib location)
target_link_libraries(sweather mycjson)
target_link_libraries(sweather parseweather)
target_link_libraries(sweather getweather)

install(TARGETS sweather DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/SimpleWeatherConfigure.h" DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_SOURCE_DIR}/pack)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(PROJECT_VERSION_FULL ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES
        ${PROJECT_SOURCE_DIR}/build
        ${PROJECT_SOURCE_DIR}/cmake-build-debug
        ${PROJECT_SOURCE_DIR}/pack
        ${PROJECT_SOURCE_DIR}/.idea
        ${PROJECT_SOURCE_DIR}/.DS_Store
        ${PROJECT_SOURCE_DIR}/.git
        ${PROJECT_SOURCE_DIR}/.gitignore)
set(CPACK_SOURCE_PACKAGE_FILE_NAME
        sweahter-${PROJECT_VERSION_FULL})

set(CPACK_SOURCE_GENERATOR "TGZ" )
include(CPack)




