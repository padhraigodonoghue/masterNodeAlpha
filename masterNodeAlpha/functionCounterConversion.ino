// derive ruleNumber by comparing count of relevant bin (i.e. frame) to a reference maximum
int countConversion(int index)
{
  int ruleOut;

  int binCount = superFrameArray[index];

  if (binCount != -1)
  {
    // high-water-mark approach to calibrating reference maximum
    if (binCount > maxFrameTraffic)
    {
      maxFrameTraffic = binCount;
    }

    // cast int data types to float for more precise rationing
    float floatyBinCount        = (float) binCount;
    float floatyMaxFrameTraffic = (float) maxFrameTraffic;

    float ratio  = floatyBinCount / floatyMaxFrameTraffic;
    float scaled = ratio * (float) (numberOfRules - 1);

    ruleOut = round(scaled);

    // some debugging checks
    if (debugMode == true)
    {
      Serial.print(" ### floatyBinCount: ");
      Serial.print(floatyBinCount);
      Serial.print(" ### floatyMaxFrameTraffic: ");
      Serial.print(floatyMaxFrameTraffic);
      Serial.print(" ### ratio: ");
      Serial.print(ratio);
      Serial.print(" ### scaled: ");
      Serial.println(scaled);
    }
  }
  else
  {
    ruleOut = 255;
  }
  
  return ruleOut;
}
