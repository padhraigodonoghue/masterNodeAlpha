// for activating solenoid (and updating flag that indicates whether it should be released after hold time)
void solenoider(int velocityIn, boolean pressurised)
{
  digitalWrite(solenoidPin, velocityIn);

  if (pressurised == true)
  {
    struckAtTime = millis();
    solenoidPressure = true;
  }
}

// non-blocking solenoid hold and release
void nonBlockingSolenoidRelease()
{
  unsigned long currentTime = millis();

  if (currentTime - struckAtTime >= solenoidHoldDuration)
  {
    Serial.println("solenoid should release!");
    digitalWrite(solenoidPin, LOW);
    solenoidPressure = false;
  }
}

// non-blocking pre-release and activation
void nonBlockingSolenoidTick()
{
  unsigned long currentTime = millis();
  
  // some debugging checks
  if (solenoidDebugMode == true)
  {
    Serial.print("time to tick: ");
    Serial.print(solenoidPreHoldDuration - (currentTime - coiledAtTime));
    Serial.print(" ms");
  }

  if (currentTime - coiledAtTime >= solenoidPreHoldDuration)
  {
    solenoider(tickVelocity, false);
    solenoidCoil = false;
  }
}


