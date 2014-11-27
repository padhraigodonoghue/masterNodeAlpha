void solenoider(String command)
{
  if (command.equals("damp") == true)
  {
    Serial.println("damp sound");
    digitalWrite(10, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(10, LOW);
    digitalWrite(ledPin, LOW);
  }
}
