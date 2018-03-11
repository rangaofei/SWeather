
configure_file(
        "SimpleWeatherConfigure.h.in"
        "${PROJECT_BINARY_DIR}/SimpleWeatherConfigure.h"
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/CJson)
#include_directories(${PROJECT_SOURCE_DIR}/ParseWeather)
include_directories(${PROJECT_BINARY_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/GetWeather)
#include_directories(${PROJECT_SOURCE_DIR}/Location)
add_subdirectory(CJson)
add_subdirectory(ParseWeather)
add_subdirectory(GetWeather)
add_subdirectory(Location)
set(SOURCE_FILES main.c)
add_executable(sweather ${SOURCE_FILES})

FIND_PACKAGE(CURL)
IF (CURL_FOUND)
    MESSAGE(STATUS ${CURL_LIBRARY})
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
    target_link_libraries(sweather ${CURL_LIBRARY})
ELSE ()
    MESSAGE(FATAL_ERROR "CURL library not found,please install")
ENDIF ()

target_link_libraries(sweather location)
target_link_libraries(sweather mycjson)
target_link_libraries(sweather parseweather)
target_link_libraries(sweather getweather)

install(TARGETS sweather DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/SimpleWeatherConfigure.h" DESTINATION include)

