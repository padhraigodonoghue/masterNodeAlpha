// increment the int at the index of array corresponding to current "frame"
// if a new "frame" has been entered, reset to 0 before incrementing
void incrementTrafficCount()
{
  int dynamicIndex = frameTracker();
  superFrameArray[dynamicIndex]++;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### superFrameArray[");
    Serial.print(dynamicIndex);
    Serial.print("]: ");
    Serial.println(superFrameArray[dynamicIndex]);
    Serial.println("");
  }
  
  if (dataTicking == true)
  {
    solenoidCoil = true;
    coiledAtTime = millis();
  }
}
