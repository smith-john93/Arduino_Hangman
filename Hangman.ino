#include <stepper.h>

#define mPin1 2
#define mPin2 3
#define mPin3 4
#define mPin4 5
#define soundPin 9

#define NOTE_E5  659
#define NOTE_F7  2794
#define NOTE_F2  87

//declare an array to hold various words
const char* words[] = {"APPLE", "CUBE", "ARDUINO", "CALCULATE", "BEAGLE", "COFFMAN"};

//declare an array for the aplhabet
char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

//char* to hold the word
const char* selectedWord;

//declare a byte to hold the ordinal of the word
byte wordIndex;

//holds the index of the random word selected
byte alphaIndex = 0;

//change this to change the number of guesses allowed
byte availableGuesses = 8;

//tracks the number of remaining _
byte remainingSpaces = 0;

//used to track guesses that have been made
char guessedLetters[10];
byte guessedIndex = 0;

STEPPER stepper(mPin1, mPin2, mPin3, mPin4);

void setup() 
{ 
  //set pin modes
  //analog pin modes used for joystick
  pinMode( A0, INPUT);
  pinMode( A1, INPUT);

  //call the connect display fucntion to establish a connection
  connectDisplay();
  
  const char* setting = "Setting Up Game";
  writeString(setting);
  delay(500);
  
  //Set the random seed by reading from analog 5
  randomSeed(analogRead(5));
  
  //using setDisplay to clear the display
  setDisplay();
  delay(1000);

  setWord();
  
  displayCharacters();

}

void loop() 
{
  //read the analog values
  int xVal = analogRead(1);
  int yVal = analogRead(0); 

  //check if the user is trying to scroll 
  if(xVal < 400)
  {
    while(xVal < 400)
    {
      xVal = analogRead(1);
    }
    scroll(false);
  }

  //check if the user is trying to scroll 
  if(xVal > 700)
  {
    while(xVal > 700)
    {
      xVal = analogRead(1);
    }
    scroll(true);
  }
  //check if the user is trying to select 
  if(yVal < 300)
  {
    while(yVal < 300)
    {
      yVal = analogRead(0);
    }
    selectLetter();
  }
}

//Check the selected letter against the word
void selectLetter()
{
  bool match = false;

  //first we check to see if the letter has already been guessed
  for(int i = 0; i < sizeof(guessedLetters)/2; i++)
  {
    if(char(alphabet[alphaIndex]) == guessedLetters[i])
    {
      //that letter has been guessed already
      return;
    }
  }

  //the letter hasn't been guessed, check the word for the letter
  for(int i = 0; i < strlen(selectedWord); i++)
  {     
    if(selectedWord[i] == char(toupper(alphabet[alphaIndex])))
    {
      //track the letter guessed
      guessedLetters[guessedIndex++] = char(alphabet[alphaIndex]);
      
      //a match was made so we can break from this loop
      match=true;
      break;
    }
  }
  
  if(!match)
  {
    guessSound(false);
    stepper.forwards();
    --availableGuesses;

    //check to see if we etered a lose condition
    if(availableGuesses == 0)
    {
      loseCondition();
    }    
  }
  
  if(match)
  {
    setLetters();
  }   
}

//this is called for inital setup
void displayCharacters()
{
  //make sure the cursor is on the second row
  setCursorPosition(17);
  const char* alphabet = "Abcdefghijklmnop";
  writeString(alphabet); 
}

//scrolls the character string left or right one step
void scroll(bool right)
{
  //this is how wide the lcd is
  byte maxSpaces = 16;

  
  if(right)
  {
    //do not advance the string past 'z'
    if(alphaIndex == 25)
    {
      return;
    }    
    //incriment where we are in the alphabet
    ++alphaIndex;
  }
  else
  {
    //do not advance the string past 'a;
    if(alphaIndex == 0)
    {
      return;
    }
    //decriment where we are in the alphabet
    --alphaIndex;
  }

  byte tmpIndex = alphaIndex;

  //clear the bottom line
  setCursorPosition(17);
  const char* spaces = "                ";
  writeString(spaces);
  setCursorPosition(17);

  
  for(int i = 0; i < maxSpaces; i++)
  {
    if(i == 0)
    {      
      //write the first letter as a captial letter
      writeCharacter(alphabet[alphaIndex] - 32);
    }
    else
    {
      //if we are at the end of the alphabet, start writing spaces
      if(tmpIndex > 25)
      {
        writeCharacter(' ');
      }
      else
      {
        //write the current character in the array        
        writeCharacter(alphabet[alphaIndex + i]);
      }
    }
    ++tmpIndex;
  }
}

//called when the game is lost
//writes a new message to the screen
//and resets the stepper motor
void loseCondition()
{
  clearDisplay();
  setCursorPosition(1);
  const char* tryAgain = "Try again!";
  writeString(tryAgain);
  endGameSound(false);
  stepper.resetStepper();
}
//called when the game is won
//writes a new message to the screen
//and resets the stepper motor
void winCondition()
{
  clearDisplay();
  setCursorPosition(1);
  const char* youWin = "You Win!"; 
  writeString(youWin);
  endGameSound(true);
  stepper.resetStepper();  
  
}

//selects the word to be used
void setWord()
{
  //divide the size by 2
  //sizeof is returning 2-times the number or items in the array
  
  //set the word & record the index
  wordIndex = random(sizeof(words)/2);
  selectedWord = words[wordIndex];

  //set the number of remaining spaces
  remainingSpaces = strlen(selectedWord);

  //print underscores to the screen
  for(int i  = 0; i < strlen(selectedWord); i++)
  {
    writeCharacter('_');
  }
}


void setLetters()
{
  for(int i = 0; i < strlen(selectedWord); i++)
  {    
    if(selectedWord[i] == char(toupper(alphabet[alphaIndex])))
    {
      setCursorPosition(i+1);
      writeCharacter(toupper(alphabet[alphaIndex]));
      --remainingSpaces;                  
    }
  }
  if(remainingSpaces == 0)
  {
    delay(1000);
    winCondition();    
  }
  else
  {
    guessSound(true);
  }
}

void endGameSound(bool gameWin)
{
  for(int i = 0; i < 5; i++)
  {
    guessSound(gameWin);
    delay(100);
  }
}


void guessSound(bool guessed)
{
  if(guessed)
  {
    tone(soundPin,NOTE_E5,250);
    delay (100);
    tone(soundPin,NOTE_F7,250);
  }
  else
  {
    tone(soundPin, NOTE_F2, 500);
  }
}
