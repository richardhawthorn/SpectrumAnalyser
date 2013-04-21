//Spectrum Analyser using MSGEQ7

const int ANALOG = 1;
const int STROBE = 2;
const int RESET = 3;
int outputValues[7];

int barValue = 0;
int outputValuesMax[7];
int countLoop = 0;
int countLoopMax = 1;

void setup() {
  
    //setup MSGEQ7
  pinMode(ANALOG, INPUT);
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);

  analogReference(DEFAULT);

  digitalWrite(RESET, LOW);
  digitalWrite(STROBE, HIGH);
  
  Serial.begin(9600);
  
  matrix.begin(0x70);  // pass in the address
}


void loop() {
  
  //reset chip
  digitalWrite(RESET, HIGH);
  delayMicroseconds(1);
  digitalWrite(RESET, LOW);
  
  countLoop++;
  if (countLoop > countLoopMax){
     countLoop = 0; 
  }
  
  delay(100);
  
  //load the data from the msgeq7
  for (int i = 0; i < 7; i++) {
    digitalWrite(STROBE, LOW);
    delayMicroseconds(30);
    outputValues[i] = analogRead(ANALOG);
    digitalWrite(STROBE, HIGH);
    
    outputValuesMax[i] = max(outputValuesMax[i], outputValues[i]);
  }
  
  //if we are at the end of our average loop, then output the value.
  if (countLoop == 0){
    for (int i = 0; i < 7; i++) {
      
      //normalise the value
      barValue = outputValues[i] - 150;
      if (barValue < 0){barValue = 0;}
      
      barValue = round(barValue / 130);
      if (barValue > 7){
         barValue = 7; 
      } else if (barValue < 1){
         barValue = 0; 
      }
      
      Serial.print(outputValues[i]);      
      Serial.print(" ");
      
      outputValuesMax[i] = 0;
    }
    
    Serial.println(" ");
    
  }

}