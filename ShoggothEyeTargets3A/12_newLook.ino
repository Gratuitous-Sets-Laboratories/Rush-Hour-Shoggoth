/*
 * This function generates a new focal point for a set of eyes to look,
 * along with a level of "caffine".
 */

void newLook(byte eye){
  
    isMoving[eye] = true;
    lookTick[eye] = millis();
    lookDelay[eye] = random (lookMin, lookMax);

    lookXOld[eye] = lookXNew[eye];
    lookYOld[eye] = lookYNew[eye];
    
    lookXNew[eye] = random(-100,100);
    lookYNew[eye] = random(-50,100);
    caffine[eye] = random(-40,40);
}
