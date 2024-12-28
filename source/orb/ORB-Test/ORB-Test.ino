//
//           ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄      ▀▀█▀▀ ░█▀▀▀ ░█▀▀▀█ ▀▀█▀▀       
//          ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░▌     ─░█── ░█▀▀▀ ─▀▀▀▄▄ ─░█──  
//          ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌    ─░█── ░█▄▄▄ ░█▄▄▄█ ─░█──
//          ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌ 
//          ▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌
//          ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░▌
//          ▐░▌       ▐░▌▐░█▀▀▀▀█░█▀▀ ▐░█▀▀▀▀▀▀▀█░▌
//          ▐░▌       ▐░▌▐░▌     ▐░▌  ▐░▌       ▐░▌
//          ▐░█▄▄▄▄▄▄▄█░▌▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄█░▌
//          ▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░▌
//           ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀
//
//          Project ORB - Test
//
//                        Test Rig for Project ORB
//          Author:       Andrew Shapton
//
//          Credits:      Ben Zott for the ROM image binary
//
//          Revision:     0.1         Initial Release 

// Define DEBUG and output settings
#define DEBUG 1
#define OUTP  1

#if DEBUG == 1
  #define debug(txt) Serial.println(txt)
  #define debugnolf(txt) Serial.print(txt)
  #define debugbegin() Serial.begin(9600); // Any baud rate should work
#else
  #define debug(txt) 
  #define debugnolf(txt)
  #define debugbegin()
#endif 

#if OUTP == 1
  #define debugbegin() Serial.begin(9600); // Any baud rate should work
  #define out(txt) Serial.println(txt)
#else
  #define debugbegin() Serial.begin(9600); // Any baud rate should work
  #define out(txt)
#endif

// Include PDS V3N ROM image
#include "pds-v3n.h"

// Include functions
#include "functions.h"

// Include pin layouts for interface to Arduino Nano
#include "pinlayouts.h"

// Constants
constexpr int CHIP_SIZE = 256;             // Size of 1702A EPROM chip in bytes

constexpr int numberOfAddressPins = 8;     // Number of pins on the address bus
constexpr int numberOfChipSelectPins = 4;  // Number of pins on the CS line (Chip Select)
constexpr int numberOfDataPins = 8;        // Number of pins on the data bus

char char_array[numberOfAddressPins] = {}; // Character array to store address from the address bus
int loc;                                   // Location within a ROM image   
int opcode;                                // value of a specified location in a ROM image


String chipSelectOutputs[]  = { "00", "10", "01", "11"}; // Chip Select lines values


void setup() {
debugbegin()


  // Initialise the Chip Select pins
  for (int thisPin = 0; thisPin < numberOfChipSelectPins; thisPin++) {
    pinMode(chipSelectPin[thisPin], OUTPUT);    // sets the digital pin as OUTPUT
  } 
    // Initialise the address bus pins
  for (int thisPin = 0; thisPin < numberOfAddressPins; thisPin++) {
    pinMode(addressPin[thisPin], OUTPUT);       // sets the digital pin as output
  }

  // Write a LOW signal to the Data Lines (thus initialising them to an analogue read status)
  for (int thisPin = 0; thisPin < numberOfDataPins; thisPin++) {
    pinMode(dataPin[thisPin], INPUT);           // sets the digital pin as output 
  }
}

void loop() {
  bool done = false;
  while (done == false)
    {
    out("Starting Test");
    debugnolf("Starting Test in ");
    for (int d = 5; d > 0; d--){
        delay(1000);
        debugnolf(String(d) + "  ...  ");
      }
    debug();
    debug("CHIP\t\tBinary\t\tData\tAdd\tOpcode");

    for (int value = 0; value < numberOfChipSelectPins; value++) {    

      String outputs = chipSelectOutputs[value];
      String one = String(outputs.charAt(0));
      String two = String(outputs.charAt(1));
      
      if (one == "1")
        digitalWrite(chipSelectPin[0], HIGH);
      else
        digitalWrite(chipSelectPin[0], LOW);

      if (two == "1")
        digitalWrite(chipSelectPin[1], HIGH);
      else
        digitalWrite(chipSelectPin[1], LOW);
      delay(500);
      
      for (int loc = 0; loc < CHIP_SIZE; loc++)
      {
        // Left pad with zeroes to length of data bus
        String valueBinary = (padleft(String(loc, BIN),numberOfAddressPins,"0"));
        // Write an output signal to each of the 8 address lines
        for (int thisPin = 0; thisPin < numberOfAddressPins; thisPin++) 
          {
            if (thisPin > 252)
              delay(10000);
            char thisBin = valueBinary.charAt(thisPin);
            if (thisBin == '1')
              {      
                digitalWrite(addressPin[thisPin], HIGH);
              }
            else
              {
                digitalWrite(addressPin[thisPin], LOW);
              }
          }
        // Read the input signals from each of the 8 data lines
        String data = "";
        for (int thisPin = 0; thisPin < numberOfDataPins; thisPin++) 
        {
          int x = digitalRead(dataPin[thisPin]);
          data = data + String(x);
        }
        data = data + "\0";
        data.toCharArray(char_array, numberOfAddressPins+1);
        int locn = bin2dec(char_array);
          
        // Retrieve value from ROM image
        int address = loc + (((value) * CHIP_SIZE));    // Ensure the selected chip is taken into consideration
        opcode = pgm_read_byte_near( &(ROM[address]) );  // Note that the ROM image is stored in program memory
        if (locn >  (CHIP_SIZE * numberOfChipSelectPins )-1) 
          address = "***";
        
        debug("CHIP=" + String(value) + "\t\t"  + data + "\t" + String(locn) + "\t" + String(address) + "\t" + String(opcode));
        }
      }
      debug("DONE");
      out("DONE");
      done = true;
      delay(100000);
      }
  }

