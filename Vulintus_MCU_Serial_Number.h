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


#ifndef __VULINTUS_MCU_SERIAL_NUMBER_H__
#define __VULINTUS_MCU_SERIAL_NUMBER_H__

#include "Arduino.h"                    // Main include file for the Arduino SDK.

#if defined(ARDUINO_ARCH_AVR)           // AVR microcontrollers.
  #include <avr/boot.h>
#endif


// MEMORY ADDRESSES **************************************************************************************************//
#if defined(ARDUINO_ARCH_AVR)           // AVR microcontrollers.
  #if defined(__AVR_ATmega328PB__)
    #define MCU_SERIALNUM_NUM_BYTES     (10u)
  #else
    #define MCU_SERIALNUM_NUM_BYTES     (9u)
    #define SKIP_ZPOINTER_ADDRESS       0x0014
  #endif
  #define SIGNATURE_BYTE_START          0x000E
#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
  #define MCU_SERIALNUM_NUM_BYTES       (16u)
  #define MCU_SERIALNUM_NUM_ADDR        (4u)
  #if defined(__SAMD11__) || defined(__SAMD21__) || defined(__SAML21__)    
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x0080A00C)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x0080A040)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x0080A044)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x0080A048)
  #elif defined(__SAMD51__) || defined(__SAME51__)
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x008061FC)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x00806010)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x00806014)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x00806018)
  #endif
#else
  #error "Target MCU is not recognized in the serial number library!"
#endif


// OUTPUT FORMATS ****************************************************************************************************//
#ifndef HEX
  #define HEX                           16
#endif
#if (MCU_SERIALNUM_NUM_BYTES == 16)     // UUID outputs are only available for 16-byte serial numbers.
  #ifndef UUID                
    #define UUID                        36
  #endif
  #define UUID_CHAR_LEN                 (36u)
  #define UUID_DASH_0                   (4u)
  #define UUID_DASH_1                   (7u)
  #define UUID_DASH_2                   (9u)
  #define UUID_DASH_3                   (11u)
#endif


// NAMESPACE *********************************************************************************************************// 
namespace Vulintus_MCU_Serial_Number
{
  void as_Bytes (uint8_t *buffer);
  void as_CString (char * const string_buffer);
  String as_String (void);
  void as_CString (char * const string_buffer, uint8_t format);
  String as_String (uint8_t format);
}

#endif      // #ifndef __VULINTUS_MCU_SERIAL_NUMBER_H__