file(GLOB SOURCES *.c)

idf_component_register(
	SRCS ${SOURCES}
	INCLUDE_DIRS .
	PRIV_REQUIRES "usb" "qmsd_board" "aw9523" "fatfs" "led_strip"
)
