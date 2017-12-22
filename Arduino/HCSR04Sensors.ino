int trigPin_front = 10;    
int echoPin_front = 11;
int trigPin_side = 12;    
int echoPin_side = 13;
int left_sensor 
double duration, distance;
 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin_front, OUTPUT);
  pinMode(echoPin_front, INPUT);
  pinMode(trigPin_side, OUTPUT);
  pinMode(echoPin_side, INPUT);
}
double uvSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
return distance;
}
void loop()
{
  double frontUV = uvSensor(trigPin_front, echoPin_front);
  double sideUV = uvSensor(trigPin_side, echoPin_side);

  Serial.print(frontUV);
  Serial.print(" - ");
  Serial.println(sideUV);
  delay(100);
}



