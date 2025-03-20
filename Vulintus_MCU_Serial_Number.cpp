/*  Vulintus_MCU_Serial_Number.h - copyright Vulintus, Inc., 2023

    Most of the microcontrollers used in Vulintus products have unique serial
    numbers embedded in their nonvolatile memory. Vulintus uses the serial 
    number from the primary microcontroller in each device as the identifying
    serial number for entire for the device.

    * Microchip AVR microcontrollers.
        - The ATmega328PB microcontroller has a 10-byte unique ID stored in its 
          signature bytes starting at address 0x0E (datasheet section 32.5).
        - The ATmega328P, ATmega2560, and ATtiny85 have a 9-byte ID, which is
          not unique, stored in their signature bytes starting at address 0x0E 
          (not specified in datasheet), which skips address 0x14;

    * Microchip SAM-D SAM-E microcontrollers.
        - Each SAM microcontroller has a unique 128-bit serial number which is a
          concatenation of four 32-bit words.
        - The uniqueness of the serial number is guaranteed only when using all 
          128 bits.
        - The memory locations for each SAM variant are found in the following 
          datasheet sections:
            * SAMD11 -          Section 9.6.
            * SAMD21 -          Section 9.3.3.
            * SAML21 -          Section 11.5.
            * SAMD5x / SAME5x - Section 9.6.


    UPDATE LOG:
      2023-05-24 - Drew Sloan - Library adapted from David Pruitt's HabiTrak-
                                specific SAMD serial number library.
      2024-02-21 - Drew Sloan - Added support for some AVR microcontrollers.

*/


#include "./Vulintus_MCU_Serial_Number.h"


namespace Vulintus_MCU_Serial_Number
{

  // Return the serial number as a byte array.
  void as_Bytes (uint8_t *buffer)
  {

    
  #if defined(ARDUINO_ARCH_AVR)   // AVR microcontrollers.         

    // Grab the AVR signature bytes.
    uint16_t addr = SIGNATURE_BYTE_START;
    for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_BYTES; i++) {
      buffer[i] = boot_signature_byte_get(addr++);
    #if !defined(__AVR_ATmega328PB__)
      if (addr == SKIP_ZPOINTER_ADDRESS) {
        addr++;
      }
    #endif
    }
    
  #elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)   // SAMD microcontrollers.
    
    //Load the serial number addresses into an array.
    uint32_t serialnum_addr[4];           
    serialnum_addr[0] = MCU_SERIALNUM_ADDR_0;
    serialnum_addr[1] = MCU_SERIALNUM_ADDR_1;
    serialnum_addr[2] = MCU_SERIALNUM_ADDR_2;
    serialnum_addr[3] = MCU_SERIALNUM_ADDR_3;

    //Break the 32bit words into individual bytes.
    for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_ADDR; i++)
    {
        buffer[i * 4 + 0] = (uint8_t)(serialnum_addr[i] >> 24);
        buffer[i * 4 + 1] = (uint8_t)(serialnum_addr[i] >> 16);
        buffer[i * 4 + 2] = (uint8_t)(serialnum_addr[i] >> 8);
        buffer[i * 4 + 3] = (uint8_t)(serialnum_addr[i] >> 0);
    }        

  #elif defined(ARDUINO_ARCH_NRF52840)  // Nordic NRF52840 microcontrollers.

    //Grab the NRF52840 unique ID bytes.
    for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_ADDR; i++) {
      for (uint8_t j = 0; j < 4; j++) {
        buffer[i * 4 + j] = NRF_FICR->DEVICEID[i] >> (8 * (3 - j));
      }
    }

  #endif
  }


  // Return the serial number as a hex C string, with format unspecified.
  void as_CString (char * const string_buffer)
  {
    uint8_t format = HEX;                 //Default to the hexadecimal ouput, with no UUID dashes.
    as_CString(string_buffer, format);
  }


  // Return the serial number as a hex C string, with format specified.
  void as_CString (char * const string_buffer, uint8_t format)
  {
    //Instantiate the byte buffer
    uint8_t buffer[MCU_SERIALNUM_NUM_BYTES];

    //Get the serial number as bytes.
    as_Bytes(buffer);

    //Now convert the byte array to a C-string. The buffer must be 33-bytes exactly.
    uint8_t j = 0;
    for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_BYTES; i++) {

    #if (MCU_SERIALNUM_NUM_BYTES == 16) 
      //For the UUID format, place a "-" character before bytes (zero-based indexing) 4, 7, 9, and 11.
      if (format == UUID)
      {          
        if (i == UUID_DASH_0 || i == UUID_DASH_1 || i == UUID_DASH_2 || i == UUID_DASH_3)
        {
            string_buffer[j++] = '-';
        }
      }
    #endif

      //Note: we are processing 2 digits (one byte) in a single iteration of the loop.
      uint8_t current_byte = buffer[i];
      uint8_t low_nibble = current_byte & 0x0F;
      uint8_t high_nibble = (current_byte >> 4) & 0x0F;
      char base_char = 'A';               //Use upper-case hexadecimal by default.
    #if (MCU_SERIALNUM_NUM_BYTES == 16) 
      if (format == UUID)                 
      {        
        base_char = 'a';                  //Use lower-case hexadecimal for UUID format.
      }
    #endif
      string_buffer[j++] = (high_nibble < 10) ? '0' + high_nibble : (base_char - 10) + high_nibble;
      string_buffer[j++] = (low_nibble < 10) ? '0' + low_nibble : (base_char - 10) + low_nibble;      
    }

    //Make absolutely sure that the final byte of the string buffer is the null byte
    string_buffer[j] = 0;               //Default hexadecimal format has 2 characters for each byte (last character is null).
  }


  // Return the serial number as an Arduino string, with format unspecified.
  String as_String (void)
  {
    uint8_t format = HEX;                 //Default to the hexadecimal ouput, with no UUID dashes.
    String result = as_String(format);
    return result;
  }


  // Return the serial number as an Arduino string, with format specified.
  String as_String (uint8_t format)
  {
    //Instantiate a C-string buffer to hold the unique ID.
    char string_buffer[37];  //Create a buffer with 37 characters to hold the UUID format (we'll trim it down if otherwise).

    //Get the unique ID as a C-string
    as_CString(string_buffer, format);

    //Convert it to a C++ Arduino String object and return it to the caller
    String result = String(string_buffer);
    result.trim();
    return result;
  }

}