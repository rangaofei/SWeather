add_library(parseweather parseweather.c printweather.c)
install(TARGETS parseweather DESTINATION bin)
install(FILES parseweather.h DESTINATION include)
install(FILES printweather.h DESTINATION include)