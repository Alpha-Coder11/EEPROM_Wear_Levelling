#include "wear_levelling.h"

#define SECTOR_INACTIVE    0            ///< Sector is inactive
#define SECTOR_ACTIVE      1            ///< Sector is active

#define NUMBER_OF_SECTORS  4            ///< Total Number of Sectors to divide the read-write cycles, 4 sectors are used in this case. It can be changed based on user's requirement

/*
+-------------+
|   Status    |
+-------------+
|   Sector 0  |
|             |
|             |
+-------------+
|   Status    |
+-------------+
|   Sector 1  |
|             |
|             |
+-------------+
|   Status    |
+-------------+
|   Sector 2  |
|             |
|             |
+-------------+
|   Status    |
+-------------+
|   Sector 3  |
|             |
|             |
+-------------+
 */

// Defining the addresses of status of the sectors
uint16_t sector_status_address[NUMBER_OF_SECTORS] = 
{
    0x0000, 0x1000, 0x2000, 0x3000                  // Address of the status of the sectors. These are example values, user can change them based on the EEPROM memory map
};

// Defining the address of the sectors
uint16_t sector_address[NUMBER_OF_SECTORS] =
{
    0x0002, 0x1002, 0x2002, 0x3002                  // Address of the sectors. These are example values, user can change them based on the EEPROM memory map
};

void setting_sector_clear(const struct_i2c_handle *i2c, uint8_t sector) 
{
    uint8_t status = SECTOR_INACTIVE;
    struct_data_t empty_sector = {0};

    eeprom_write(i2c, sector_status_address[sector], &status, sizeof(status));
    eeprom_write(i2c, sector_address[sector], (uint8_t *)&empty_sector, sizeof(empty_sector));
}

void eeprom_all_sectors_clear(const struct_i2c_handle *i2c) 
{
    for (uint8_t i = 0; i < NUMBER_OF_SECTORS; i++) 
    {
        setting_system_state_clear(i2c, i);
    }
}

uint8_t eeprom_sector_load(const struct_i2c_handle *i2c, uint8_t *buffer, uint32_t size) 
{
    struct_data_t sector = {0};
    uint8_t status = 0;
    uint8_t active_sector = 0;

    for (active_sector = 0; active_sector < NUMBER_OF_SECTORS; active_sector++) 
    {
        eeprom_read(i2c, sector_status_address[active_sector], &status, sizeof(status));

        if (status == SECTOR_ACTIVE) 
        {
            eeprom_read(i2c, sector_address[active_sector], (uint8_t *)&sector, size);
            if (calculate_crc16((uint8_t *)&sector, size - 2) == sector.crc) 
            {
                memcpy(buffer, &sector, size);
                return active_sector;
            }
        }
    }

    eeprom_all_sectors_clear(i2c);

    // Initialize the first sector if no valid sector is found
    status = SECTOR_ACTIVE;
    eeprom_write(i2c, sector_status_address[0], &status, sizeof(status));
    eeprom_write(i2c, sector_address[0], &sector, size);                     // Write the buffer to the first sector, User can use initial state to write to the first sector

    return 0; // Default to first sector
}

uint8_t eeprom_sector_write(struct_i2c_handle *i2c, uint8_t *buffer, uint32_t size, uint8_t current_sector) 
{
    uint8_t status = SECTOR_INACTIVE;

    // Deactivate current sector
    eeprom_write(i2c, sector_status_address[current_sector], &status, sizeof(status));

    // Activate next sector
    current_sector = (current_sector + 1) % NUMBER_OF_SECTORS;
    status = SECTOR_ACTIVE;
    eeprom_write(i2c, sector_status_address[current_sector], &status, sizeof(status));

    // Write new state to active sector
    eeprom_write(i2c, sector_address[current_sector], buffer, size);

    return current_sector;
}
