// executes a delay before sending rule number over Serial Port
void playNote(int waitTimeIn, int ruleNumberIn)
{
  // in case Arduino falls behind unexpectedly, do not allow a negative wait value reach 
  if (waitTimeIn < 0)
  {
    waitTimeIn = 0;
  }
  
  delay(waitTimeIn);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("waitTimeIn: ");
    Serial.print(waitTimeIn);
    Serial.print(" ### SENDING RULE: ");
    Serial.println(ruleNumberIn);
    Serial.println("");
  }
  else
  {
    Serial.write(ruleNumberIn);
  }
}

