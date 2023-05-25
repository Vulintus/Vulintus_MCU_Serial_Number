/*  Print_MCU_ID.ino - Vulintus, Inc.

  Super-simple sketch to demonstrate the Vulintus_MCU_Serial_Number library.

  UPDATE LOG:
    2023-05-24 - Drew Sloan - Sketch first created.

*/


//Included libraries.//
#include <Vulintus_MCU_Serial_Number.h>           //Vulintus' MCU serial number library.

//Serial communication constants.//
#define SERIAL_BAUD_RATE  115200        //Serial baud rate.

//System information variables. //
Vulintus_MCU_Serial_Number serialnum = Vulintus_MCU_Serial_Number();


//INITIALIZATION ************************************************************//
void setup() {

  //Initialize serial data transmission.
  Serial.begin(SERIAL_BAUD_RATE);             
  while (!Serial);
                 
  //Show the sketch filename and upload date for a sanity check.
  Print_FW_Filename();          
  Print_FW_Date();

  HEX;

}


//MAIN LOOP *****************************************************************//
void loop() {

  Serial.println("\nSERIAL NUMBER:");

  //Fetch the serial number as bytes.
  uint8_t sn_bytes[16];
  serialnum.as_Bytes(sn_bytes);
  Serial.print("\tBYTES:          ");
  for (uint8_t i = 0; i < sizeof(sn_bytes); i++) {
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
  serialnum.as_CString(sn_cstring);
  Serial.print("\tCSTRING (HEX):  ");
  Serial.println(sn_cstring);
  // for (uint8_t i = 0; i < (nchar - 1); i++) {
  //   Serial.print(sn_cstring[i]);
  //   if ((!((i+1) % 8)) && (i != (nchar - 2))) {
  //     Serial.print("-");
  //   }
  // }
  // Serial.println();
  

  //Fetch the serial number as an Arduino string in hexadecimal format.
  String sn_string = serialnum.as_String();
  Serial.print("\tSTRING (HEX):   ");
  Serial.println(sn_string);
  // for (uint8_t i = 0; i < sn_string.length(); i++) {
  //   Serial.print(sn_string.charAt(i));
  //   if ((!((i+1) % 8)) && (i != (nchar - 2))) {
  //     Serial.print("-");
  //   }
  // }
  // Serial.println();


  //Fetch the serial number as a C string in UUID format.
  sn_cstring[UUID_CHAR_LEN];
  serialnum.as_CString(sn_cstring,UUID);
  Serial.print("\tCSTRING (UUID): ");
  Serial.println(sn_cstring);
  

  //Fetch the serial number as an Arduino string in UUID format.
  sn_string = serialnum.as_String(UUID);
  Serial.print("\tSTRING (UUID):  ");
  Serial.println(sn_string);

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