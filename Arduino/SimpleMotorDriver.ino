const int enB = 5;
const int in4 = 6;
const int in3 = 7;
const int in2 = 8;
const int in1 = 9;
const int enA = 10;
void setup() {
  pinMode(enB, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in1, OUTPUT);
}

void loop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 200);
  delay(2000);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
