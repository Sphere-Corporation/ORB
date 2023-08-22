

int bin2dec(char s[]) {
// Convert binary string into decimal

int value = 0;
for (int i=0; i< strlen(s); i++)  // for every character in the string  strlen(s) returns the length of a char array
{
  value *= 2; // double the result so far
  if (s[i] == '1') value++;  //add 1 if needed
}
return value;
}

String padleft(String input, int length, String pad) {
// Pad string to left with supplied character

String output = input;
while (output.length() < length) {
  output = pad + output;
}
return output;
}


int chipselect(int chipSelectLines[]){
// Select a Memory chip (1-4) to be used.
int chip = 0;
int zeros = 0;
// Read chip select input pins
for (int i=3; i >=0; i--)
  {
  if (analogRead(chipSelectLines[i] ) == 0)
    { 
    chip = i+1; 
    zeros++;
    }
  }
//If more than one switch is set to "ON" chip selection is invalid
if (zeros > 1)
 chip = 0;
return chip;
}

int chipselectnew(int chipSelectLines[] ){

 
// Select a Memory chip (1-4) to be used.
int chip = 0;
int zeros = 0;
String binaryChip = "";
// Read chip select input pins
int CS1Value =  (analogRead(A6));   // read CS1 input pin
int CS2Value =  (analogRead(A7));   // read CS2 input pin

if (CS1Value == 0) {
  binaryChip = "0";
}
else
{
  binaryChip = "1";
}

if (CS2Value == 0) {
  binaryChip = binaryChip + "0";
}
else
{
    binaryChip = binaryChip + "1";
}
// 1 - 11
// 2 - 10
// 3 - 01
// 4 - 00
  if (binaryChip == "11")
    chip = 1;
  if (binaryChip == "10")
    chip = 2;
  if (binaryChip == "01")
    chip = 3;
  if (binaryChip == "00")
    chip = 4;
Serial.print(("A6=" + String(CS1Value) + "   A7=" + String (CS2Value)));

Serial.print("Chip=");
Serial.print(binaryChip);
Serial.println("     " + String(chip));
return chip;
}