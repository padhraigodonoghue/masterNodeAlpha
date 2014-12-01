void serviceSerial()
{
  // some debugging checks
  if (debugMode == true)
  {
//    Serial.println("checking Serial Port buffer...");
  }

  while (Serial.available())
  {
    // in debug mode, Serial Port will be received the value of the ACSCII code for the character '1', i.e. 49
    if (debugMode == true)
    {
      if (Serial.read() == 49)
      {
        // increment count on whatever frame for every byte in the serial port buffer
        incrementTrafficCount();
      }
    }
    else
    {
      if (Serial.read() == 1)
      {
        // increment count on whatever frame for every byte in the serial port buffer
        incrementTrafficCount();
        Serial.println("received bang!");
      }
    }
  }
}
