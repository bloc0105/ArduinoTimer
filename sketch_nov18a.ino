int previous_val = HIGH;
int output_val = LOW;


void setup() {
  // put your setup code here, to run once:
  pinMode(11, OUTPUT);
  pinMode(0, INPUT_PULLUP);
}

void loop() {
  int current_val = digitalRead(0);
   if (current_val != previous_val && previous_val == HIGH)
   {
    if (output_val == LOW)
    {
      output_val = HIGH;
    }
    else 
    {
      output_val = LOW;
    }
   }
   current_val = previous_val;
   digitalWrite(11, output_val);
  // put your main code here, to run repeatedly:

}
