add_library(mycjson cJSON.c)
install(TARGETS mycjson DESTINATION bin)
install(FILES cJSON.h DESTINATION include)