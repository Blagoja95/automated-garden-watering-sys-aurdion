int nov = 7;
int releY= A3;

void setup() {
  // put your setup code here, to run once:
pinMode(nov, OUTPUT);
pinMode(releY, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(nov, HIGH);
  digitalWrite(releY, HIGH);
  delay(3000);
  digitalWrite(nov, LOW);
  digitalWrite(releY, LOW);
  delay(3000);
}
