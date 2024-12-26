file(GLOB SOURCES *.c)

idf_component_register(
	SRCS ${SOURCES}
	INCLUDE_DIRS .
	PRIV_REQUIRES "qmsd_board" esp-idf-rc522 nvs_flash esp_netif
)
