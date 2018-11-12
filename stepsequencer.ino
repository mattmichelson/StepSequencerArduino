//Matthew Michelson
//Lab 7
//Step Sequencer + Teensy Audio Library (no delay)

//these are all the variables
int ledPins[4] = { 32, 31, 30, 29 };
int buttonPins[4] = { 25, 26, 27, 28};
int switchPin = 24;

int currentStep = 0;
unsigned long lastStepTime = 0;
int tempo = 0;

boolean lastButtonStates[4] = {LOW};
boolean buttonStates[4] = {LOW};
boolean on[4] = {false};

void setup() {
  
  //pinModes
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins [i], OUTPUT);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins [i], INPUT);
  }
  
  pinMode(switchPin, INPUT);
}

void loop() {
  checkButtons();
  setLeds();
  sequence();
  
}

void sequence() {

  tempo = analogRead(A14);

  if (digitalRead(switchPin) == LOW) {
    forwardStep(tempo);
  }

  if (digitalRead(switchPin) == HIGH) {
    backwardStep(tempo);
  }
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonStates [i] = buttonStates [i];    
    buttonStates [i] = digitalRead(buttonPins [i]);
    
      if(buttonStates [i] == HIGH && lastButtonStates [i] == LOW) {
        if(on[i] == false) {
          on[i] = true;
          } 
        else if(on [i] == true) {
          on[i] = false;
        }
      }
  }  
}

void setLeds() {
  for (int i = 0; i < 4; i++) {
    
    if(on [i] == true || i == currentStep) {
        digitalWrite(ledPins [i], HIGH);
      } 
      
      else if(on [i] == false || i == currentStep) {
          digitalWrite(ledPins [i], LOW);
      }  
  }
}


void forwardStep(int potentiometer) {
  //calls the pitch and actually plays it
  
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(ledPins[currentStep], LOW);  //turn off the current led
    usbMIDI.sendNoteOff(60, 0, 1); //send midi note on


    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    } 
    //digitalWrite(ledPins[currentStep], HIGH); //turn on the new led
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOn(60, 127, 1); //send midi note on
      usbMIDI.sendNoteOff(60, 0, 1); //send midi note off
    }
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void backwardStep(int potentiometer) {
  //calls the pitch and plays it backwards

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(ledPins[currentStep], LOW);  //turn off the current led
    usbMIDI.sendNoteOff(60, 0, 1); //send Midi note off

    currentStep = currentStep - 1;         //increment to the next step
    if (currentStep < 0) {
      currentStep = 3;
    }
    //digitalWrite(ledPins[currentStep], HIGH); //turn on the new led
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOn(60, 127, 1); //send midi note on
      usbMIDI.sendNoteOff(60, 0, 1);//send midi note off
    }

    lastStepTime = millis();               //set lastStepTime to the current time
  }
}
