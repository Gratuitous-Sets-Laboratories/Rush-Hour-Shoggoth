/*
 * This function reports data along the serial monitor.
 */

void debugTroubleshoot(){
  
  if(rawData != oldData){
    Serial.print("rawData: "); 
    Serial.println(rawData%16,BIN);
    for (int eye = 0; eye < 4; eye++){
      Serial.print("eye #");
      Serial.print(eye+1);
      Serial.print(": ");
      Serial.print(isShot[eye]);
      Serial.println(" ");
    }
  }
//  Serial.println(lookFrame[0]);
/*
  if (rawData != oldData){
    Serial.print("rawData: ");
    Serial.print(rawData);
    Serial.println();
  }
*/
/*
  for (int eye = 0; eye < 4; eye++){
    Serial.print("Eye#");
    Serial.print(eye);
    Serial.print(" | lookXnew: ");
    Serial.print(lookXNew[eye]);
    Serial.print(" | lookYNew: ");
    Serial.print(lookYNew[eye]);
    Serial.println();
  }
  Serial.println();
  Serial.println();
*/
}
