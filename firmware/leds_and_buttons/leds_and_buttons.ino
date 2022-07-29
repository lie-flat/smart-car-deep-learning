const int led1 = 32;  // Green LED
const int led2 = 33;  // Red LED
const int key1 = 35;
const int key2 = 34;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(key1, INPUT);
  pinMode(key2, INPUT);
}

void loop() {
  if (digitalRead(key1) == LOW) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
  if (digitalRead(key2) == HIGH) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }
}