add_library(getweather getweather.c)
target_link_libraries(getweather mycjson parseweather location)
install(TARGETS getweather DESTINATION bin)
install(FILES getweather.h DESTINATION include)