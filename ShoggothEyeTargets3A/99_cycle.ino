/*
 * This function updates old second state variables with new information.
 */

void cycleReset(){
  
  oldData = rawData;
  for (int eye = 0; eye < 4; eye++){
    wasShot[eye] = isShot[eye];
  }
}
