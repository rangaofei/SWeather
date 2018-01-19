add_library(getweather getweather.c)
install(TARGETS getweather DESTINATION bin)
install(FILES getweather.h DESTINATION include)