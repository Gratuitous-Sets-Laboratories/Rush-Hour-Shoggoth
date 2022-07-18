/*
 * This function sends the specific pulse widths to each set of eyes on both servoBars.
 */
 
void updateServos(int eye){
  
  if (eye<2){
    servoBar1.setPWM((eye%2)*8+0,0,xPulse);
    servoBar1.setPWM((eye%2)*8+1,0,lyPulse);
    servoBar1.setPWM((eye%2)*8+2,0,xPulse);
    servoBar1.setPWM((eye%2)*8+3,0,ryPulse);
    servoBar1.setPWM((eye%2)*8+4,0,upPulse);
    servoBar1.setPWM((eye%2)*8+5,0,loPulse);
  }
  else{
    servoBar2.setPWM((eye%2)*8+0,0,xPulse);
    servoBar2.setPWM((eye%2)*8+1,0,lyPulse);
    servoBar2.setPWM((eye%2)*8+2,0,xPulse);
    servoBar2.setPWM((eye%2)*8+3,0,ryPulse);
    servoBar2.setPWM((eye%2)*8+4,0,upPulse);
    servoBar2.setPWM((eye%2)*8+5,0,loPulse);
  }
}
