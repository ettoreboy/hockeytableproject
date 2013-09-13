#include "pitches.h"
int speaker = 10;
int laserPort = 12; 
int led1 = 11; 
int led2 = 7;
int resumeButton = 9;

int onLevel = -1; 
int offLevel = -1; 
boolean tripped = false; 

boolean resume = false;
int goal_1= 0;
int goal_2 = 0;

void setup() { 
  pinMode(laserPort, OUTPUT); 
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(resumeButton, INPUT);
  
  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW); 
  

  // Check the state of the phototransistor with the laser on 
  // and off.  I delay a bit because the rise time on the 
  // pins appears to be between 1/10 and 1/100 of a second.
  digitalWrite(laserPort, LOW); 
  delay(1000);  
  offLevel = analogRead(A0); 

  digitalWrite(laserPort, HIGH); 
  delay(1000); 
  onLevel = analogRead(A0); 
playIntro();
} 

//Procedure for playing the game Intro on the speaker.
void playIntro() {
  // notes in the melody:
  int intro []  = {
    NOTE_AS3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_AS3, NOTE_F3, NOTE_G3, NOTE_A3,
    NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_AS3,
    NOTE_C4, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4,
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_G5, NOTE_E5, NOTE_G5 };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations []  = {
  3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 6, 6, 5, 5, 2};

  for (int thisNote = 0; thisNote < 32; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(speaker, intro[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speaker);
  }

}

//Countdown effect after resume of the game.
void playCountdown(){
   tone(speaker, NOTE_G2 , 500);
   delay(1200);
   tone(speaker, NOTE_G2 , 500);
   delay(1200);
   tone(speaker, NOTE_G3 , 1000);
}

//Procedure to stop the game, until a button is pressed.
void playBuzz(){
  tone(speaker, NOTE_G5 , 1500);
  resume = false;
  while(!(resume)){
   int buttonState = digitalRead(resumeButton);
    if(buttonState == HIGH){
      playCountdown();
      resume=true;
    }
  }
  tripped=false;                               //reset the tripwire in false state.
}

void loop() { 

  //##########################################################################
  //Procedure that checks for goals.
  delay(1);  
  // It must be restart by pressing the resume button afterwards. 
  if (!(tripped)) { 
    int currentLevel = analogRead(A0); 
    // Trip if the light level drops below 25% of on vs. ambient. 
    if (currentLevel < onLevel - (onLevel - offLevel) / 4) { 
      tripped = true; 
      switch(goal_1){
      case 0: 
        goal_1 = 1;
        digitalWrite(led1, HIGH);
        playBuzz();
        break;
      case 1:
        goal_1  = 2;
        digitalWrite(led2, HIGH);
        playBuzz();
        break;
      case 2:
        goal_1 = 3;
        playBuzz();
        break;
      }
      
      delay(1);
    }   
  }   


} 



