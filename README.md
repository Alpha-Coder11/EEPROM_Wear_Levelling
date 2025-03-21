# EEPROM Wear Levelling

## Overview
This code provides a sector management system for EEPROM storage. It allows the user to maintain multiple sectors for data storage for levelling the read write cycles over multiple sectors

### Features
- **Sector-based Data Storage**: Divides EEPROM storage into multiple sectors.
- **Redundancy Handling**: Automatically identifies and loads valid data from active sectors.
- **Error Recovery**: Clears and reinitializes corrupted sectors.
- **Configurable Parameters**: Allows customization of the number of sectors and memory map.

---

## How It Works
The system manages data storage using a rolling sector approach:

1. **Initialization**:
   - It scans through the sectors to find the active one.
   - If no valid sector is found, it initializes the first sector.

2. **Read Operation**:
   - Finds the active sector.
   - Validates data integrity using CRC16.

3. **Write Operation**:
   - Marks the current sector inactive.
   - Activates the next sector and writes the new data.

4. **Clear Operation**:
   - Clears one or all sectors and resets their status.

---

## Project Structure

```
├── config.h                  // User-specific configuration (I2C, CRC, EEPROM APIs)
├── wear_levelling.c          // Core logic for wear levelling and sector management
├── wear_levelling.h          // Contains headers for the functions
```

---

## Configuration
Modify `config.h` to fit your system:

```c
// Define the EEPROM read/write functions
void eeprom_write(const struct_i2c_handle *i2c, uint16_t address, const uint8_t *data, uint32_t size);
void eeprom_read(const struct_i2c_handle *i2c, uint16_t address, uint8_t *data, uint32_t size);


// Define the CRC calculation function
uint16_t calculate_crc16(const uint8_t *data, uint32_t length);

// Define the I2C handle structure
typedef struct {
    // User-defined I2C parameters
} struct_i2c_handle;

// Define the system state structure
typedef struct {
    uint8_t data[64]; // User data
    uint16_t crc;   // CRC16 for error-checking
} struct_data_t;
```

---

## Usage Guide

### 1. Initialize the System
Ensure all sectors are cleared or properly initialized:

```c
struct_i2c_handle i2c;
eeprom_all_sectors_clear(&i2c);
```

### 2. Load System State
Retrieve the latest valid system state:

```c
uint8_t *buffer;
uint8_t active_sector = eeprom_sector_load(&i2c, buffer, sizeof(struct_data_t));
```

### 3. Write System State
Update the system state and switch to the next sector:

```c
active_sector = eeprom_sector_write(&i2c, buffer, sizeof(struct_data_t), active_sector);
```

### 4. Clear a Specific Sector
Clear a particular sector and mark it inactive:

```c
setting_sector_clear(&i2c, 0); // Clear sector 0
```

---

## Customization
1. **Number of Sectors**: Modify `NUMBER_OF_SECTORS` to change the number of rotating sectors.

2. **EEPROM Addresses**: Update `sector_status_address` and `sector_address` arrays to match your memory map.

3. **Data Structure**: Customize `struct_system_state_t` to fit your application's needs.

---

## Error Handling
- If no valid sector is found, the system clears and reinitializes the first sector.
- Ensure `calculate_crc16()` correctly handles data integrity checks.

---

## Notes
- Ensure your EEPROM read/write functions handle the I2C communication correctly.
- Use this module with any microcontroller by providing appropriate `config.h` settings.

