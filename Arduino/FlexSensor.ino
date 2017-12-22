int flex_pin = A0;
int enA = 11;
int in1 = 12;
int in2 = 8;

void setup()
{
  Serial.begin(9600);
  pinMode(flex_pin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void loop()
{
  int V_read = analogRead(flex_pin);
  double flexV = V_read * 5.0 / 1023;
  Serial.println(V_read);
  Serial.println("Voltage at A0: " + String(flexV));
  int MotorSpeed = map(flexV, 227, 242, 0, 255);
  analogWrite(enA, MotorSpeed);
  delay(250);
  }
