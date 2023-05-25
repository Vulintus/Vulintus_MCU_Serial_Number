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


#ifndef VULINTUS_MCU_SERIAL_NUMBER_H
#define VULINTUS_MCU_SERIAL_NUMBER_H

#include "Arduino.h"                    //Main include file for the Arduino SDK.


// UNIVERSALLY UNIQUE IDENTIFIER (UUID) FORMAT ***********************************************************************//
#define UUID_CHAR_LEN                   (36u)
#define UUID

// MEMORY ADDRESSES **************************************************************************************************// 
#if defined(__SAMD11__) || defined(__SAMD21__) || defined(__SAML21__)
    #define MCU_SERIALNUM_NUM_BYTES     (16u)
    #define MCU_SERIALNUM_NUM_ADDR      (4u)
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x0080A00C)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x0080A040)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x0080A044)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x0080A048)
#elif defined(__SAMD51__) || defined(__SAME51__)
    #define MCU_SERIALNUM_NUM_BYTES     (16u)
    #define MCU_SERIALNUM_NUM_ADDR      (4u)
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x008061FC)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x00806010)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x00806014)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x00806018)
#else
    #error Target MCU is not recognized in the serial number library.
#endif


// CLASSES ***********************************************************************************************************// 
class Vulintus_MCU_Serial_Number {

	public:

		// Constructor. //
		Vulintus_MCU_Serial_Number(void);

        // Destructor. //
		~Vulintus_MCU_Serial_Number(void);

		// Functions. //
    void as_Bytes(uint8_t *buffer);
    void as_CString(char * const string_buffer);
    String as_String(void);

	private:

};

#endif      // #ifndef VULINTUS_MCU_SERIAL_NUMBER_H