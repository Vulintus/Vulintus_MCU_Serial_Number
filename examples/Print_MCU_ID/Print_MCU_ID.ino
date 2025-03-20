/*  Print_MCU_ID.ino - Vulintus, Inc.

  Super-simple sketch to demonstrate the Vulintus_MCU_Serial_Number library.

  UPDATE LOG:
    2023-05-24 - Drew Sloan - Sketch first created.

*/


//Included libraries.//
#include <Vulintus_MCU_Serial_Number.h>   //Vulintus' MCU serial number library.

//Serial communication constants.//
#define SERIAL_BAUD_RATE  115200          //Serial baud rate.


//INITIALIZATION ************************************************************//
void setup() {

  //Initialize serial data transmission.
  Serial.begin(SERIAL_BAUD_RATE);             
  while (!Serial);
                 
  //Show the sketch filename and upload date for a sanity check.
  Print_FW_Filename();          
  Print_FW_Date();

}


//MAIN LOOP *****************************************************************//
void loop() {

  Serial.println("\nSERIAL NUMBER:");

  //Fetch the serial number as bytes.
  uint8_t sn_bytes[MCU_SERIALNUM_NUM_BYTES];
  Vulintus_MCU_Serial_Number::as_Bytes(sn_bytes);
  Serial.print("\tBYTES:          ");
  for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_BYTES; i++) {
    Serial.print(sn_bytes[i], DEC);
    if (i < (sizeof(sn_bytes) - 1)) {
      Serial.print(" - ");      
    }
    else {
      Serial.println();
    }
  }

  //Fetch the serial number as a C string in hexadecimal format.
  uint8_t nchar = 2*MCU_SERIALNUM_NUM_BYTES + 1;
  char sn_cstring[nchar];
  Vulintus_MCU_Serial_Number::as_CString(sn_cstring);
  Serial.print("\tCSTRING (HEX):  ");
  Serial.println(sn_cstring);
  

  //Fetch the serial number as an Arduino string in hexadecimal format.
  String sn_string = Vulintus_MCU_Serial_Number::as_String();
  Serial.print("\tSTRING (HEX):   ");
  Serial.println(sn_string);

#if (MCU_SERIALNUM_NUM_BYTES == 16)     // UUID output options required 16-byte serial numbers.

  //Fetch the serial number as a C string in UUID format.
  sn_cstring[UUID_CHAR_LEN];
  Vulintus_MCU_Serial_Number::as_CString(sn_cstring,UUID);
  Serial.print("\tCSTRING (UUID): ");
  Serial.println(sn_cstring);
  

  //Fetch the serial number as an Arduino string in UUID format.
  sn_string = Vulintus_MCU_Serial_Number::as_String(UUID);
  Serial.print("\tSTRING (UUID):  ");
  Serial.println(sn_string);

#endif

  delay(5000);

}


//SUBFUNCTIONS **************************************************************//

//Print the sketch filename from the macro.
void Print_FW_Filename() {
  char s[] = __FILE__;                  //Grab the filename from the macro.
  uint8_t b = sizeof(s);                //Find the number of characters in the filename.
  while ((b > 0) && (s[b] != '\\')) {   //Loop until a forward slash is found.
    b--;                                //Step backward through the filename.
  }   
  char *u = &s[++b];                    //Create a pointer to start at the filename.
  Serial.println(u);                    //Print the filename to the serial line.
}


//Print the sketch upload date from the macro.
void Print_FW_Date() {
  char fw_date[] = __DATE__;            //Grab the upload date from the macro.
  char fw_time[] = __TIME__;            //Grab the upload time from the macro.
  Serial.print(fw_date);              //Print the upload date to the serial line.
  Serial.print(F(", "));                //Print a comma.
  Serial.println(fw_time);              //Print the upload time to the TFT display.
}