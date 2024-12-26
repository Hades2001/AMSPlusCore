file(GLOB SOURCES *.c)

idf_component_register(
	SRCS . ams_ntag.c
	INCLUDE_DIRS .

	PRIV_REQUIRES driver json
)
