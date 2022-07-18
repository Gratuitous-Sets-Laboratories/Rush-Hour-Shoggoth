/*
 * This function uses the rawData byte to track if an eye has been shot.
 * If it has been long enough since the eye was shot, it will "heal".
 */
 
void gunshots(byte eye){
  
  for (int eye = 0; eye < 4; eye++){
    bool bitVal = bitRead (rawData, eye);
    if (!bitVal){
      isShot[eye] = true;
      shotTick[eye] = millis();
    }
    if (millis() >= shotTick[eye] + healDelay){
      isShot[eye] = false;
    }
  }
}
