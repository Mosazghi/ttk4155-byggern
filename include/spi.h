#pragma once
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdint.h>
typedef enum {
  F_DIV_4 = 0,
  F_DIV_16,
  F_DIV_64,
  F_DIV_128,
  F_DIV_2,
  F_DIV_8,
  F_DIV_32,
} spi_clock_div_t;
typedef struct {
  uint8_t mosi_pin_num;
  uint8_t sck_pin_num;
  uint8_t miso_pin_num;
  spi_clock_div_t clock_div;
} spi_config_t;

typedef struct {
  const void *tx_buf;
  uint8_t *rx_buf;
  int len;
} spi_transfer_t;

typedef struct {
  volatile uint8_t *ss_port_num;
  uint8_t ss_pin_num;
} spi_device_handle_t;

/**
 * @brief Initializes the SPI interface.
 * @param handle Pointer to the SPI configuration structure.
 */
void spi_init(spi_config_t *handle);
/**
 * @brief Performs a SPI transfer (both read and write).
 * @param dev Pointer to the SPI device handle.
 * @param transfer Pointer to the SPI transfer structure.
 */
void spi_transfer(spi_device_handle_t *dev, spi_transfer_t *transfer);

/**
 * @brief Selects the SPI slave device.
 * @param dev Pointer to the SPI device handle.
 */
void spi_slave_select(spi_device_handle_t *dev);
/**
 * @brief Deselects the SPI slave device.
 * @param dev Pointer to the SPI device handle.
 */
void spi_slave_deselect(spi_device_handle_t *dev);
