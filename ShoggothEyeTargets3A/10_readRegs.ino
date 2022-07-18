/*
 * This function, along with the pulsePin() function below
 * will read the first PISO shift register
 * and update the rawData byte with the new information.
 */

void readReg(){

  pulsePin(loadPin,10);

  for (int bitPos = 0; bitPos < 8; bitPos++){
    bool bitVal = digitalRead(dataInPin);
    bitWrite (rawData, bitPos, bitVal);
    pulsePin(clockPin,10);
  }
}

void pulsePin(int pinName, int usTime){
  digitalWrite (pinName, LOW);
  delayMicroseconds (usTime);
  digitalWrite (pinName, HIGH);
  delayMicroseconds (usTime);
}
