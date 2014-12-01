// for activating solenoid (and updating flag that indicates whether it should be released after hold time)
void solenoider(int velocityIn, boolean pressurised)
{
  analogWrite(solenoidPin, velocityIn);

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

  if (currentTime - struckAtTime >= ((unsigned long) solenoidHoldDuration))
  {
    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.println("solenoid should release!");
      Serial.println("");
    }
    analogWrite(solenoidPin, LOW);
    solenoidPressure = false;
  }
  else
  {
    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.print("time before release: ");
      Serial.print(solenoidHoldDuration - (currentTime - struckAtTime));
      Serial.println(" ms");
    }
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
    int timeRemaining = (int) (((unsigned long) solenoidPreHoldDuration) - (currentTime - coiledAtTime));
    Serial.print(timeRemaining);
    Serial.println(" ms");
  }

  if (currentTime - coiledAtTime >= solenoidPreHoldDuration)
  {
    solenoider(tickVelocity, false);
    solenoidCoil = false;
    solenoidDamp = true;

    dampedAtTime = millis();

    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.println("THUCK!");
    }
  }
  else
  {
    solenoider(LOW, false);
    solenoidDamp = false;
  }
}

// non-blocking pre-release and activation
void nonBlockingDamping()
{
  unsigned long currentTime = millis();

  // some debugging checks
  if (solenoidDebugMode == true && ((millis() % ((unsigned long) 100)) == (unsigned long) 0))
  {
    Serial.print("time to undamp: ");
    int timeRemaining = (int) (((unsigned long) solenoidDampDuration) - (currentTime - dampedAtTime));
    Serial.print(timeRemaining);
    Serial.println(" ms");
  }

  if (currentTime - dampedAtTime >= solenoidDampDuration)
  {
    solenoider(LOW, false);
    solenoidDamp = false;

    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.println("solenoid relax");
      Serial.println("");
    }
  }
}
