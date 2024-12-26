file(GLOB SOURCES *.c)

idf_component_register(
	SRCS . ams_mqtt.c
	INCLUDE_DIRS .

	PRIV_REQUIRES driver mqtt spi_flash esp_partition json nvs_flash esp_netif
)
