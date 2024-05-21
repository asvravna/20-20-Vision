
#include <FastLED.h> // Biblioteket som kontrollerer LED-stripen
#define NUM_LEDS 10 // 10 LEDs på stripen
#define DATA_PIN 3 // Den digitale pinnen til LED-data
#define BI_PIN 5  // BI- backup data 
// et LED-array basert på antall LED-er
CRGB leds[NUM_LEDS];

// pin konstanter
const int knapp1Pin = 11;
const int knapp2Pin = 12;
const int knapp3Pin = 13; 
const int potentPin = A0; //skal regulere verdi

// tidsintervaller for millis()
long arbeidsIntervall = 1200000; // 20 min i millisekunder
long pauseIntervall = 0;
unsigned long previousTime = 0;



void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  Serial.begin(9600);

  pinMode(knapp1Pin, INPUT_PULLUP);
  pinMode(knapp2Pin, INPUT_PULLUP);
  pinMode(knapp3Pin, INPUT_PULLUP);
 
}

  void loop() {
    //1: 
    
    //tidsProgram();

    //2: 
    // dim();

    //3:
    diskoModus();

    //




  }
  






void dim(){
  int styrkeRange = analogRead(potentPin) >> 2; //range til HSV skal være 2^8 fremfor 2^10
  for(int i = 0; i < NUM_LEDS; i ++){
    leds[i] = CHSV(10, 80, styrkeRange);
    Serial.println(analogRead(potentPin));
  }
  FastLED.show();


}

void tidsProgram(){
  int knapp1 = digitalRead(knapp1Pin);
  int knapp2 = digitalRead(knapp2Pin);
  int knapp3 = digitalRead(knapp3Pin);
  

  if (knapp1 == LOW){ //pin11
    leds[0] = CRGB::Green; FastLED.show();
    pauseIntervall = 300000; // 5 min i millisekunder
    previousTime = millis();
    Serial.println("Knapp 1 lyser grønt, 5 min interval");
    lysHvitt();

  } else if (knapp2 == LOW){ //pin12
    leds[0] = CRGB::Blue; FastLED.show();
    pauseIntervall = 60000; // 1 min
    previousTime = millis();
    Serial.println("Knapp 2 lyser blått, 1 min interval");
    lysHvitt();

  } else if (knapp3 == LOW){ //pin13
    leds[0] = CRGB::Red; FastLED.show();
    pauseIntervall = 200; // 20 sek
    previousTime = millis();
    Serial.println("Knapp 3 lyser rødt, 20 sek interval"); //etter 20 sek, skru seg av:
    lysHvitt();
  }
  else {
    skruAvLEDstripe();
  }
  

  if (millis() - previousTime >= pauseIntervall) {
    skruAvLEDstripe();
  } else {
    tidsProgram();
  }
}

void skruAvLEDstripe(){
  for(int i = 0; i < NUM_LEDS; i ++){
  leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
  
}

void skruPaaLEDstripe(){
  for(int i = 0; i < NUM_LEDS; i ++){
    leds[i] = CHSV(255, 255, 255); //CHSV for å kunne regulere value
  }
  
  FastLED.show();
  delay(1000000);
    
}

void lysHvitt(){ 
  //leds[0] = CRGB::Red; FastLED.show();
  for(int i = 1; i < NUM_LEDS; i ++) {
    leds[i] = CRGB::White; FastLED.show();


  }
}


void diskoModus(){
  int fargeRange = analogRead(potentPin) >> 2; //range til HSV skal være 2^8 fremfor 2^10
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(fargeRange * i % 256, 255, 255);
      FastLED.show();
  }

}









