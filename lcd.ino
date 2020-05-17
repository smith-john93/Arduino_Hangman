#define D4Pin 13
#define D5Pin 10
#define D6Pin 11
#define D7Pin 12
#define EPin 8
#define RSPin 7

#define CLR_DISPLAY 0x01
#define nextLine 0xC0
#define cursorHome 0x02
#define blinkCursor 0x0F
#define advanceCursor 0x14

int pinArry[] = {D4Pin, D5Pin, D6Pin, D7Pin};
void connectDisplay() 
{

  pinMode(D4Pin, OUTPUT);
  pinMode(D5Pin, OUTPUT);
  pinMode(D6Pin, OUTPUT);
  pinMode(D7Pin, OUTPUT);
  pinMode(EPin, OUTPUT);
  pinMode(RSPin, OUTPUT);
    
  digitalWrite(EPin, LOW);
  digitalWrite(RSPin, LOW);
  initDisplay();
  setDisplay();
  delay(1000);
}

void clearDisplay()
{
  writeCommand(CLR_DISPLAY);
  delay(2000);
}

//this is the handshake between the LCD and the arduino
void initDisplay()
{
  delay(15);
  write4Bits(0x03);
  delay(5);
  write4Bits(0x03);
  delay(100);
  write4Bits(0x03);
  write4Bits(0x02);
}

void setDisplay()
{
  writeCommand(0x28);
  writeCommand(0x0C);
  writeCommand(0x06);
  writeCommand(0x01);
  delay(2);
  writeCommand(0x80);
}

void write4Bits(byte val)
{
  for(int i = 0; i<4; i++)
  {
    digitalWrite(pinArry[i], val&0x01);
    val >>= 1;
  }

  digitalWrite(EPin, HIGH);
  digitalWrite(EPin, LOW);
  delayMicroseconds(100);
}

void writeCommand(int command)
{
  digitalWrite(RSPin, LOW);
  write4Bits(command>>4);
  write4Bits(command);
}

void writeCharacter(int character)
{
  digitalWrite(RSPin, HIGH);
  write4Bits(character>>4);
  write4Bits(character);
}

void writeString(const char* string)
{
  while(*string)
  {
    writeCharacter(*string);
    string++;
  }
}

void setCursorPosition(int location)
{
  /*
   * There are 32 available spaces, 16 per row
   * for the sake of this funciton:
   *  1-16 is the top
   *  17-32 is the bottom
   * -------------------------------------------------
   * | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|
   * |17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|
   * -------------------------------------------------
   */

  //send the cursor home 
  writeCommand(cursorHome);
  delay(100);
  if(location < 17)
  {
    for(int i = 0; i<location-1; i++)
    {
      writeCommand(advanceCursor);
      delay(100);
    }
  }
  else
  {
    writeCommand(nextLine);
    delay(100);
    int pos = location - 16;
    for(int i = 0; i<pos-1; i++)
    {
      writeCommand(advanceCursor);
      delay(100);
    }
  }
}
