# Vulintus MCU Serial Number

An Arduino-compatible library for reading unique hardware identifiers from microcontrollers with embedded serial numbers.

_Disclaimer: This README file was AI-generated, but checked for accuracy by a human._

## Overview

This library provides a unified interface to access the unique hardware identifiers available on various microcontroller architectures. Most microcontrollers have unique serial numbers or device IDs embedded in their nonvolatile memory, and this library abstracts the platform-specific details to provide a consistent API across different MCU families.

## Supported Microcontrollers

### Microchip AVR
- **ATmega328PB**: 10-byte unique ID stored in signature bytes (datasheet section 32.5)
- **ATmega328P, ATmega2560, ATtiny85**: 9-byte ID stored in signature bytes (note: not guaranteed unique)

### Microchip SAM-D/SAM-E
- **SAMD11**: 128-bit unique serial number (datasheet section 9.6)
- **SAMD21**: 128-bit unique serial number (datasheet section 9.3.3)
- **SAML21**: 128-bit unique serial number (datasheet section 11.5)
- **SAMD5x/SAME5x**: 128-bit unique serial number (datasheet section 9.6)

### Nordic Semiconductor
- **nRF52840**: 64-bit unique device identifier from Factory Information Configuration Registers (FICR)

### NXP
- **RT1176** (Coral Dev Board): 64-bit unique device identifier from eFuses

## Installation

1. Download or clone this repository
2. Place the library folder in your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **macOS**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`
3. Restart the Arduino IDE
4. Include the library in your sketch: `#include <Vulintus_MCU_Serial_Number.h>`

## Usage

The library provides several functions to retrieve the MCU serial number in different formats:

### Basic Usage

```cpp
#include <Vulintus_MCU_Serial_Number.h>

void setup() {
  Serial.begin(115200);
  
  // Get serial number as a String (hexadecimal format)
  String serialNumber = Vulintus_MCU_Serial_Number::as_String();
  Serial.println("MCU Serial Number: " + serialNumber);
}

void loop() {
  // Your code here
}
```

### Available Functions

#### `as_String()`
Returns the serial number as an Arduino `String` object in hexadecimal format.

```cpp
String sn = Vulintus_MCU_Serial_Number::as_String();
```

#### `as_String(format)`
Returns the serial number as an Arduino `String` object in the specified format.

```cpp
String sn_hex = Vulintus_MCU_Serial_Number::as_String(HEX);  // Hexadecimal
String sn_uuid = Vulintus_MCU_Serial_Number::as_String(UUID); // UUID format (16-byte MCUs only)
```

#### `as_CString(buffer)`
Fills a character buffer with the serial number as a C-string in hexadecimal format.

```cpp
char buffer[33]; // For 16-byte MCUs (32 hex chars + null terminator)
Vulintus_MCU_Serial_Number::as_CString(buffer);
```

#### `as_CString(buffer, format)`
Fills a character buffer with the serial number as a C-string in the specified format.

```cpp
char hex_buffer[33];
char uuid_buffer[37]; // UUID format requires 37 characters (36 + null terminator)

Vulintus_MCU_Serial_Number::as_CString(hex_buffer, HEX);
Vulintus_MCU_Serial_Number::as_CString(uuid_buffer, UUID); // 16-byte MCUs only
```

#### `as_Bytes(buffer)`
Fills a byte array with the raw serial number bytes.

```cpp
uint8_t bytes[MCU_SERIALNUM_NUM_BYTES];
Vulintus_MCU_Serial_Number::as_Bytes(bytes);
```

### Output Formats

- **HEX**: Standard hexadecimal representation (e.g., `1A2B3C4D5E6F7890`)
- **UUID**: Standard UUID format with dashes (e.g., `1a2b3c4d-5e6f-7890-abcd-ef1234567890`) - *only available for 16-byte serial numbers*

### Constants

- `MCU_SERIALNUM_NUM_BYTES`: Number of bytes in the serial number for the current MCU
- `HEX`: Format constant for hexadecimal output
- `UUID`: Format constant for UUID output (16-byte MCUs only)

## Example

See the included example sketch `examples/Print_MCU_ID/Print_MCU_ID.ino` for a complete demonstration of all library functions.

## Technical Details

The library automatically detects the target microcontroller architecture and uses the appropriate method to read the hardware identifier:

- **Microchip AVR**: Reads from signature bytes using `boot_signature_byte_get()`
- **Microchip SAMD**: Reads from specific memory addresses containing the 128-bit unique ID
- **NNordic nRF52840**: Reads from FICR device ID registers
- **NXP RT1176**: Reads from eFuse registers

## Limitations

- AVR microcontrollers (except ATmega328PB) provide IDs that are not guaranteed to be unique
- UUID format is only available for microcontrollers with 16-byte serial numbers
- The library requires the target MCU to be supported (compilation will fail with an error message for unsupported MCUs)

## License

Copyright © 2023 Vulintus, Inc.

## Version History

- **2025-11-07**: Added support for the Coral Dev Board Micro
- **2025-03-20**: Added support for the Nordic nRF52840 SoC microcontroller
- **2024-02-21**: Added support for AVR microcontrollers
- **2023-05-24**: Initial release, adapted from SAMD-specific library

## Contributing

For bug reports, feature requests, or contributions, please contact Vulintus, Inc.
