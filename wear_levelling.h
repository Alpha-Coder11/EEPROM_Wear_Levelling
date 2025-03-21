/**
 * @file wear_levelling.h
 * @brief EEPROM Wear-Leveling Library Header
 *
 * This library provides functions to implement wear-leveling on EEPROM by managing
 * multiple storage sectors. It enhances the lifespan of EEPROM by distributing
 * read/write operations across multiple sectors and ensuring data integrity using CRC16.
 *
 * @note Ensure to configure `config.h` for platform-specific settings.
 *
 * Features:
 * - Supports up to `NUMBER_OF_SECTORS` for wear leveling (default: 4).
 * - Automatic CRC16-based data integrity check.
 * - Cyclic sector switching for balanced wear distribution.
 * - Supports initialization and recovery from invalid sectors.
 *
 * Usage:
 * - Call `eeprom_sector_load()` to retrieve the most recent valid data.
 * - Use `eeprom_sector_write()` to store new data.
 * - Clear sectors using `setting_sector_clear()` or `eeprom_all_sectors_clear()`.
 *
 * @author Qazi Mashood
 * @date March 2025
 */

 #ifndef WEAR_LEVELLING_H
 #define WEAR_LEVELLING_H
 
 #include "config.h"
 #include <string.h>
 
 // Sector status definitions
 #define SECTOR_INACTIVE    0    ///< Sector is inactive
 #define SECTOR_ACTIVE      1    ///< Sector is active
 
 // Number of EEPROM sectors for wear leveling (modifiable)
 #define NUMBER_OF_SECTORS  4    ///< Total number of EEPROM sectors
 
 /**
  * EEPROM Memory Map:
  * +-------------+
  * |   Status    |  -> Each sector has a status byte (active/inactive)
  * +-------------+
  * |   Sector 0  |  -> Data storage for Sector 0
  * |             |
  * +-------------+
  * |   Status    |
  * +-------------+
  * |   Sector 1  |
  * |             |
  * +-------------+
  * |   Status    |
  * +-------------+
  * |   Sector 2  |
  * |             |
  * +-------------+
  * |   Status    |
  * +-------------+
  * |   Sector 3  |
  * |             |
  * +-------------+
  */
 
 /**
  * @brief Clears a specific EEPROM sector.
  *
  * Marks the specified sector as inactive and erases its contents.
  *
  * @param i2c Pointer to the I2C handle structure.
  * @param sector Sector index (0 to NUMBER_OF_SECTORS-1).
  */
 void setting_sector_clear(const struct_i2c_handle *i2c, uint8_t sector);
 
 /**
  * @brief Clears all EEPROM sectors.
  *
  * Iterates through all sectors, marking them inactive and erasing their contents.
  *
  * @param i2c Pointer to the I2C handle structure.
  */
 void eeprom_all_sectors_clear(const struct_i2c_handle *i2c);
 
 /**
  * @brief Loads the most recent valid state from EEPROM.
  *
  * Scans all sectors for an active one with a valid CRC. If no valid sector is found,
  * it initializes the first sector with the provided buffer.
  *
  * @param i2c Pointer to the I2C handle structure.
  * @param buffer Pointer to the buffer where the state will be loaded.
  * @param size Size of the state structure.
  * @return The active sector index (0 to NUMBER_OF_SECTORS-1).
  */
 uint8_t eeprom_sector_load(const struct_i2c_handle *i2c, uint8_t *buffer, uint32_t size);
 
 /**
  * @brief Writes a new state to the next sector using wear-leveling.
  *
  * Marks the current sector as inactive and writes the new state to the next sector.
  *
  * @param i2c Pointer to the I2C handle structure.
  * @param buffer Pointer to the data to be written.
  * @param size Size of the data in bytes.
  * @param current_sector Index of the currently active sector.
  * @return The new active sector index.
  */
 uint8_t eeprom_sector_write(struct_i2c_handle *i2c, uint8_t *buffer, uint32_t size, uint8_t current_sector);
 
 #endif // WEAR_LEVELLING_H
 