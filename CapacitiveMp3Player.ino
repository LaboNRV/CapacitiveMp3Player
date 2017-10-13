/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <Adafruit_CAP1188.h>


/************************************ VS1053 DEFINES *****************************************/
// define the pins used
#define CLK 13       // SPI Clock, shared with SD card
#define MISO 12      // Input data, from VS1053/SD card
#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  A0      // VS1053 reset pin (output)
#define BREAKOUT_CS     A3     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    A2      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 10     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

/************************************ CAP1188 DEFINES *****************************************/
// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

/************************************************************************************************/


Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  //Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Or...Use I2C, with reset pin



unsigned long WAIT_PERIOD = 1000;
unsigned long ts = 0;

int AiMax = 3;
int BiMax = 3;
int CiMax = 3;
int DiMax = 3;

int Ai = 0;
int Bi = 0;
int Ci = 0;
int Di = 0;

bool AReleased = true;





  
void setup() {
  
  
  Serial.begin(9600);
  
  /******************************** VS1053 Test ****************************************/
  
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));

  /******************************** SD Card Test ****************************************/
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);


  
    
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  //Serial.println(F("Playing track 001"));
  //musicPlayer.playFullFile("SOUND.mp3");
  // Play another file in the background, REQUIRES interrupts!
  //Serial.println(F("Playing track 002"));
  //musicPlayer.startPlayingFile("SOUND2.mp3");
  //musicPlayer.startPlayingFile("SOUND.mp3");
}

void loop() {
  // File is playing in the background
  /*if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
    if (c == 'l') {
       musicPlayer.setVolume(20,200);
    }
    if (c == 'r') {
       musicPlayer.setVolume(200,20);
    }
  }*/

  int sensorValue = analogRead(A5);
  Serial.println(sensorValue);




  if(sensorValue == 0)
    AReleased = true;

  
  if (sensorValue == 0) {
    // No touch detected
  }else if(millis()-ts >= WAIT_PERIOD){
    //Serial.println(millis()-ts);
    ts = millis();

    if(sensorValue > 0 && AReleased){// trigger 
      AReleased = false;
      
      long rd = random(4);
        
      if(rd == 0){//A
        
        String fileToPlay = String("A");
        fileToPlay += Ai+1;
        fileToPlay += ".mp3";
        Serial.println(fileToPlay);
        musicPlayer.stopPlaying();
        musicPlayer.startPlayingFile(fileToPlay.c_str());
        Ai++;
        if(Ai>=AiMax)
          Ai = 0;
      }else if(rd == 1){//B
        String fileToPlay = String("B");
        fileToPlay += Bi+1;
        fileToPlay += ".mp3";
        Serial.println(fileToPlay);
        musicPlayer.stopPlaying();
        musicPlayer.startPlayingFile(fileToPlay.c_str());
        Bi++;
        if(Bi>=BiMax)
          Bi = 0;
      }else if(rd == 2){//C
        String fileToPlay = String("C");
        fileToPlay += Ci+1;
        fileToPlay += ".mp3";
        Serial.println(fileToPlay);
        musicPlayer.stopPlaying();
        musicPlayer.startPlayingFile(fileToPlay.c_str());
        Ci++;
        if(Ci>=CiMax)
          Ci = 0;
      }else if(rd == 3){//D
        String fileToPlay = String("D");
        fileToPlay += Di+1;
        fileToPlay += ".mp3";
        Serial.println(fileToPlay);
        musicPlayer.stopPlaying();
        musicPlayer.startPlayingFile(fileToPlay.c_str());
        Di++;
        if(Di>=DiMax)
          Di = 0;
      }
    }
  }
  

  delay(100);
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

