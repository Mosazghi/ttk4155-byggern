#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include "sram.h"
#include <stdlib.h>
#include "utility.h"



void ext_ram_init(){
  MCUCR |= (1 << SRE);        // Enable external memory
  SFIOR |= (1 << XMM2);       // Mask PC4-PC7, used for JTAG
}

void sram_write(uint16_t addr, uint8_t data){
  // debug log
  LOG_DBG("Writing to SRAM: addr = %04X (next = %04X), data = %02X", addr, addr + 1, data);
  volatile uint8_t *ext_ram = (uint8_t *) SRAM_START; // SRAM start address
  ext_ram[addr] = data;                           // Write data to SRAM
}

uint8_t sram_read(uint16_t addr){
  LOG_DBG("Reading from SRAM: addr = %04X", addr);
  volatile uint8_t *ext_ram = (uint8_t *) SRAM_START; // SRAM start address
  return ext_ram[addr];                           // Read data from SRAM
}

void sram_write_string(uint16_t addr, const char *data) {
  while (*data) {
    sram_write(addr++, *data++);
  }
  sram_write(addr, '\0'); // Null-terminate the string in SRAM
}

void sram_read_string(uint16_t addr, char *buffer, size_t max_length) {
  size_t i;
  for (i = 0; i < max_length - 1; i++) { // Leave space for null terminator
    buffer[i] = sram_read(addr + i);
    if (buffer[i] == '\0') {
      break; // Stop if we hit the null terminator
    }
  }
  buffer[i] = '\0'; // Ensure the string is null-terminated
}

void sram_test(void) {
  volatile char *ext_ram = (char *) SRAM_START; // Start address for the SRAM
  uint16_t ext_ram_size = SRAM_SIZE;
  uint16_t write_errors = 0;
  uint16_t retrieval_errors = 0;
  LOG_INF("Starting SRAM test...");
  // rand() stores some internal state, so calling this function in a loop will
  // yield different seeds each time (unless srand() is called before this function)
  uint16_t seed = rand();
  // Write phase: Immediately check that the correct value was stored
  srand(seed);
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    ext_ram[i] = some_value;
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      LOG_ERR("Write phase error: ext_ram[%4d] = %02X (should be %02X)", i, retreived_value, some_value);
      write_errors++;
    }
  }
  // Retrieval phase: Check that no values were changed during or after the write phase
  srand(seed); // reset the PRNG to the state it had before the write phase
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      LOG_ERR("Retrieval phase error: ext_ram[%4d] (%04X) = %02X (should be %02X)", i, (uint16_t)&ext_ram[i], retreived_value, some_value);
      retrieval_errors++;
    }
  }
  LOG_INF("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n", write_errors, retrieval_errors);
}