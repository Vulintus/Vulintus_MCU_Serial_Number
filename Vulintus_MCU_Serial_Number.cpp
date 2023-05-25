/*  Vulintus_MCU_Serial_Number.h - copyright Vulintus, Inc., 2023

    Most of the microcontrollers used in Vulintus products have unique serial
    numbers embedded in their nonvolatile memory. Vulintus uses the serial 
    number from the primary microcontroller in each device as the identifying
    serial number for entire for the device.

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

*/


#include "Vulintus_MCU_Serial_Number.h"


// CLASS FUNCTIONS ***********************************************************// 

// Class constructor.
Vulintus_MCU_Serial_Number::Vulintus_MCU_Serial_Number(void)
{

}


// Class destructor.
Vulintus_MCU_Serial_Number::~Vulintus_MCU_Serial_Number(void)
{

}


// Return the serial number as a byte array.
void Vulintus_MCU_Serial_Number::as_Bytes(uint8_t *buffer)
{
  //Load the serial number addresses into an array.
  uint32_t serialnum_addr[4];           
  addr[0] = MCU_SERIALNUM_ADDR_0;
  addr[1] = MCU_SERIALNUM_ADDR_1;
  addr[2] = MCU_SERIALNUM_ADDR_2;
  addr[3] = MCU_SERIALNUM_ADDR_3;

  //Break the 32bit words into individual bytes.
  for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_ADDR; i++)
  {
      buffer[i * 4 + 0] = (uint8_t)(pdwUniqueID[i] >> 24);
      buffer[i * 4 + 1] = (uint8_t)(pdwUniqueID[i] >> 16);
      buffer[i * 4 + 2] = (uint8_t)(pdwUniqueID[i] >> 8);
      buffer[i * 4 + 3] = (uint8_t)(pdwUniqueID[i] >> 0);
  }        
}


// Return the serial number as a hex C string.
void Vulintus_MCU_Serial_Number::as_CString(char * const string_buffer)
{
  //Instantiate the byte buffer
  uint8_t buffer[16];

  //Get the serial number as bytes.
  Vulintus_MCU_Serial_Number::as_Bytes(buffer);

  //Now convert the byte array to a C-string. The buffer must be 37-bytes exactly.
  for (uint8_t i = 0, j = 0; i < 16; i++)
  {
      //Place a "-" character before bytes (zero-based indexing) 4, 7, 9, and 11.
      if (i == 4 || i == 7 || i == 9 || i == 11)
      {
          string_buffer[j++] = '-';
      }

      //Note: we are processing 2 digits (one byte) in a single iteration of the loop.
      uint8_t current_byte = buffer[i];
      uint8_t low_nibble = current_byte & 0x0F;
      uint8_t high_nibble = (current_byte >> 4) & 0x0F;
      string_buffer[j++] = (low_nibble < 10) ? '0' + low_nibble : ('a' - 10) + low_nibble;
      string_buffer[j++] = (high_nibble < 10) ? '0' + high_nibble : ('a' - 10) + high_nibble;
  }

  //Make absolutely sure that the final byte of the string buffer is the null byte
  string_buffer[36] = 0;      
}


// Return the serial number as an Arduino string.
String Vulintus_MCU_Serial_Number::as_String(void)
{
  //Instantiate a C-string buffer to hold the unique ID
  char string_buffer[37];

  //Get the unique ID as a C-string
  Vulintus_MCU_Serial_Number::as_CString(string_buffer);

  //Convert it to a C++ Arduino String object and return it to the caller
  String result = String(string_buffer);
  return result;
}