int frameTracker()
{
  // should always round down to current frame
  int currentFrame = (int) ((millis() % ((unsigned long) masterWrapTime)) / ((unsigned long) frameDuration));
  
  //  has the frame changed since last check
  if (currentFrame != lastAccessedFrame)
  {
    // reset count in this bin to zero
    superFrameArray[currentFrame] = 0;
    
    // update lastAccessedFrame variable
    lastAccessedFrame = currentFrame;
  }
  
  return currentFrame;
}
