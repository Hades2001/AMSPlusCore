file(GLOB SOURCES *.c)

idf_component_register(
	SRCS . aht10.c
	INCLUDE_DIRS .
	REQUIRES driver bus i2c_bus
)
