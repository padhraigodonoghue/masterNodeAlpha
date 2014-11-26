// executes a delay before sending rule number over Serial Port
void playNote(int waitTimeIn, int ruleNumberIn)
{
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

