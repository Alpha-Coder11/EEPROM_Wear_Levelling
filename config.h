// config.h (Configuration Header for Project-Specific Customization)
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// User-defined sector information
#define NUMBER_OF_SECTORS 4             // Total number of sectors to divide the read-write cycles

// Define I2C structure (Modify this to fit your I2C implementation)
typedef struct {
    // Your I2C handle definition
} struct_i2c_handle;

// EEPROM read/write function signatures (Modify these for your EEPROM API)
void eeprom_write(const struct_i2c_handle *i2c, uint16_t address, const uint8_t *data, uint32_t size);
void eeprom_read(const struct_i2c_handle *i2c, uint16_t address, uint8_t *data, uint32_t size);


// CRC calculation function (User must implement it)
uint16_t calculate_crc16(const uint8_t *data, uint32_t length);

// Define the structure of the system state (Modify as needed)
typedef struct {
    uint8_t data[64]; // Example payload
    uint16_t crc;     // CRC for data integrity
} struct_data_t;


#endif // CONFIG_H