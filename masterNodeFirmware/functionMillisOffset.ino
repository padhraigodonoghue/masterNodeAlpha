unsigned long millisOffset()
{
  return (millis() + timeReferenceOffset);
}

void setTimeOffset()
{
  //set up Arduino pins for interfacing with RTC
  
  // variable for storing current minutes time (0-59)
  int minutesTime;
  // call minute() function from Time library
  minutesTime = minute();
  
  int minutesOffset = ((55 - minutesTime) + 60) % 60;
  
  minutesTime = 0;
  
  timeReferenceOffset = ((unsigned long) minutesTime) * ((unsigned long) 60000);
  
  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("current minutes time: ");
    Serial.println(minutesTime);
    Serial.print("timeReferenceOffset: ");
    Serial.println(timeReferenceOffset);
    Serial.println("");
  }
}
