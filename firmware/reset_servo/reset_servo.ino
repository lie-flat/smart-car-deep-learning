const int servoPin = 13;
const int freq = 50;
const int resolution = 8;
const int servoChannel = 0;

void setup() {
  Serial.begin(115200);
  ledcSetup(servoChannel, freq, resolution);
  ledcAttachPin(servoPin, servoChannel);
  ledcWrite(servoChannel, 20); // 7 -> 32
  Serial.println("Successfully reset the servo position.");
}


void loop(){
  
}
