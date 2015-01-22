int frameTracker()
{
  // should always round down to current frame
  int currentFrame = (int) ((millisOffset() % ((unsigned long) masterWrapTime)) / ((unsigned long) frameDuration));
  
  //  has the frame changed since last check
  if (currentFrame != lastAccessedFrame)
  {
    // reset count in this bin to zero
    superFrameArray[currentFrame] = 0;
    
    // update lastAccessedFrame variable
    lastAccessedFrame = currentFrame;
    
    // some debugging checks
    if (debugMode == true)
    {
      Serial.print("FRAME CHANGE! ... currentFrame: ");
      Serial.println(currentFrame);
      Serial.println("");
    }
  }
  
  return currentFrame;
}
