/*
 * This function uses an eye's old focal point and new focal point
 * to map a specific point in the middle based on the animation's current frame.
 */

void animate(byte eye){

  int lookPointX = map(lookFrame[eye],0,99,lookXOld[eye],lookXNew[eye]);
  int lookPointY = map(lookFrame[eye],0,99,lookYOld[eye],lookYNew[eye]);
  
  xPulse = map(lookPointX,-100,100,xMin,xMax);
  lyPulse = map(lookPointY,-100,100,yMin,yMax);
  ryPulse = map(lookPointY,-100,100,yMax,yMin);
  
  upPulse = map(lookPointY,-100,100,uMax,uMin);
    upPulse += (caffine[eye]/2);
    upPulse = constrain(upPulse,uMin,uMax);
  loPulse = map(lookPointY,-100,100,lMin,lMax);
    loPulse += (caffine[eye]-40);
    loPulse = constrain(loPulse,lMin,lMax);

  if (lookFrame[eye] <= 7){
    upPulse = uMax;
    loPulse = lMax;
  }
//---------------------------------------------------

  updateServos(eye);

//-----------------------------------------------------
  if (millis() >= frameTick[eye] + frameDelay){
    lookFrame[eye]++;
    frameTick[eye] = millis();
  }
  if (lookFrame[eye] >= 100){
    isMoving[eye] = false;
    lookFrame[eye] = 0;
  }
}
