// Left Motor connections
const int in1 = 5; // 2 on H brigde PMW
const int in2 = 6; // 6 on H brigde PMW
const int in3 = 10; // 10 on H brigde PMW
const int in4 = 9; // 15 on H brigde PMW
const int ECHO = 7; //
const int TRIG= 8; //
const int LEYE = 2; // INYERUPT
const int REYE = 3; // INYERUPT
const int LED = 13;

// Motor B connections



void setup() {
  // Set all the motor control pins to outputs
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(LED, OUTPUT);
  
  // Turn off motors - Initial state
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  
 
}
void forward () {  // PWM 0 = 0V (LOW) 255 = 5V HIGH SPEED VARIES INBETWEEN

  analogWrite(in1, 0);
  analogWrite(in2, 255);
  analogWrite(in3, 0);
  analogWrite(in4, 255);
  
  }



void left() {
  analogWrite(in1, 0); //grounded
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 200);
  
  }

 void right () {
  analogWrite(in1, 0);
  analogWrite(in2, 200);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  
  
  }

   void brakes () {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  
  
  }

void loop() {

  while (digitalRead( LEYE ) == HIGH  && digitalRead( REYE ) == HIGH){
    
    forward ();
    
    }

  while (digitalRead( LEYE ) == HIGH && digitalRead( REYE ) == LOW){
    
    right ();
    }


   while (digitalRead( LEYE ) == LOW && digitalRead( REYE ) == HIGH){
    left();
    
    }
    while (digitalRead( LEYE ) == LOW && digitalRead( REYE ) == LOW){
    brakes ();
    
    }

   

}

 
