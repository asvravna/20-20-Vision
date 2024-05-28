#include <FastLED.h> // Biblioteket som kontrollerer LED-stripen

#define NUM_LEDS 10 // 10 LEDs på stripen
#define DATA_PIN 3 // Den digitale pinnen til LED-data
#define BI_PIN 5  // BI- backup data 
// et LED-array basert på antall LED-er
CRGB leds[NUM_LEDS];

// pin konstanter
const int potentPin = A0; //skal regulere verdi
const int femtenMinKnapp = 11;
const int femMinKnapp = 12;
const int diskoKnapp = 13;


// tidsintervaller for millis()
long arbeidsIntervall = 10000;// 20 min i millisekunder 1 200 000 1200000;
long femMinPause =  5000; //300000
long femtenMinPause = 15000; //900000
long pauseIntervall; 
unsigned long startTid; 


//dimming: 
int styrkeRange = analogRead(potentPin) >> 2; //range til HSV skal være 2^8 fremfor 2^10




void setup() {
  //det første som skjer er at den skrus på, med en enkel led på utsiden:
  paaKnapp();
  //delay for å gi millis() tid til å starte:
  delay(1000);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  Serial.begin(9600);
//pullup for å forhindre flagrende input:
  pinMode(femtenMinKnapp, INPUT_PULLUP);
  pinMode(femMinKnapp, INPUT_PULLUP);
  pinMode(diskoKnapp, INPUT_PULLUP);

  start();

}

//hovedprogram:
void loop() {  
  startArbeidsperiode();

  settPause();

  reset();

 
 
}
long sjekkKnapp(){
  if(digitalRead(femMinKnapp) == LOW){
    pauseIntervall = femMinPause;  
  }
  else if (digitalRead(femtenMinKnapp) == LOW) {
    pauseIntervall = femtenMinPause; 
  }
  else if (digitalRead(femtenMinKnapp) == LOW && digitalRead(femMinKnapp) == LOW) {
    pauseIntervall = femtenMinPause; 
  }
  else {
    //hvis ingen er på:)
    diskoModus();
  }
  return pauseIntervall; 
}


//denne metoden skal gjøre alt som foregår i arbeidsperiode: 
void startArbeidsperiode(){

  Serial.println("Starter arbeidsperiode ->");
  startTid = millis();
  //skal lyse hvitt i 20 min: 
  while(millis() - startTid <= arbeidsIntervall){
   
    int styrkeRange = analogRead(potentPin) >> 2;
    for(int i = 0; i < NUM_LEDS; i ++){
      leds[i] = CHSV(29, 170, styrkeRange); //gyllent lys med dimming
    }
      FastLED.show();
  } //pause: 
   Serial.println(arbeidsIntervall);//hvor lang pausen var
   Serial.println("Arbeidsintervall er over.");
}



//setter pause avhengig av knappetrykk: 
void settPause(){
  Serial.println("Starter pause: "); 
  startTid = millis(); //starter tellingen av pausen: 
  while(millis() - startTid <= pauseIntervall){
    for(int i = 0; i < NUM_LEDS; i ++){
    leds[i] = CHSV(210, 255, 200-styrkeRange); //lyser rosa
  }
    FastLED.show();
  }

}


void skruAvLEDstripe(){
  for(int i = 0; i < NUM_LEDS; i ++){
  leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();

}

void reset(){
  pauseIntervall = 0;
  skruAvLEDstripe();
  start();
}
 
  
void start(){
  paaKnapp();
  while(digitalRead(femtenMinKnapp) == HIGH && digitalRead(femMinKnapp) == HIGH 
  && digitalRead(diskoKnapp) == HIGH){
  Serial.println("Venter på knapp ...");
  //gjør ingenting
  } 
  sjekkKnapp(); //lagrer valgt pause som 'pauseIntervall'
  Serial.print("Du har valgt: ");
  Serial.println(sjekkKnapp());

}


void diskoModus(){ //så lenge kulen er på
  while(digitalRead(diskoKnapp) == LOW) {

    int fargeRange = analogRead(potentPin) >> 2; //range til HSV skal være 2^8 fremfor 2^10
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(fargeRange * i % 256, 255, 255);
      FastLED.show();   
    //når brukeren tar av kulen skrur diskomodus seg av
    }
  }
  reset();  
  
}
//feedback til brukeren om at artefakten er på: 
void paaKnapp(){
  Serial.println("Er i ventemodus");

//skrur på en led
  leds[9] = CHSV(39, 255, 90);
  FastLED.show(); 
  
}