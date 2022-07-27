const int freq = 1000;
const int chan = 0;
const int resolution = 8;
const int pin = 25;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(chan, freq, resolution);
  ledcAttachPin(pin, chan);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWriteTone(chan, 1000);
  ledcWrite(chan, 128);
  delay(1000);
  ledcWriteTone(chan, 2000);
  ledcWrite(chan, 255);
  delay(1000);
  ledcWrite(chan, 0);
  delay(1000);
}
