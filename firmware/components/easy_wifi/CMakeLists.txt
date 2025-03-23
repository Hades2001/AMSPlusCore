file(GLOB SOURCES *.c)

idf_component_register(
	SRCS . easy_wifi.c
	INCLUDE_DIRS .

	PRIV_REQUIRES driver esp_wifi spi_flash esp_partition nvs_flash esp_http_server app_update json
)
