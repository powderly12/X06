
#include <WiFiNINA.h>

char ssid[] = "36a Bow Lane";
char pass[] = "letmein123";
int a;

// Left Motor connections
char c;
char c_previous;
const int in1 = 5; // 2 on H brigde PMW
const int in2 = 6; // 6 on H brigde PMW
const int in3 = 10; // 10 on H brigde PMW
const int in4 = 9; // 15 on H brigde PMW
const int ECHO = 7; //
const int TRIG= 8; //
const int LEYE = 2; // INTERUPT
const int REYE = 3; // INTERUPT
const int LED = 13;
int distance;
long duration;
WiFiClient client;
IPAddress server(192,168,159,196);

// Motor B connections



void setup() {
  // Set all the motor control pins to outputs
  Serial.begin(9600); 
  WiFi.begin(ssid, pass);
  pinMode(LED, OUTPUT);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);

 // while(client.connect(server,5200)){
    //Serial.println("Connecting...");
    
    //}
  if (client.connect(server,5200)){
    client.write("Banba is connected");
    }else {
      Serial.println("Fail");
      pinMode(LED, HIGH);
      }
  
  c_previous='L';
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  
  
  // Turn off motors - Initial state
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);

  pinMode( ECHO, INPUT );
  pinMode( TRIG, OUTPUT);
  
 
}
void forward () {  // PWM 0 = 0V (LOW) 255 = 5V HIGH SPEED VARIES INBETWEEN

  analogWrite(in1, 0);
  analogWrite(in2, 200);
  analogWrite(in3, 0);
  analogWrite(in4, 200);
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

 int Objectdetected() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration / 58;
    
    if (distance<=15) {
      client.write("stopping for obstacle at 15cm distance");
      brakes ();
      
     }
     return distance;


  
  }
int IR_Sensors (){
   if (digitalRead( LEYE ) == HIGH  && digitalRead( REYE ) == HIGH){
        
        return 1;
        
        }
    
      else if (digitalRead( LEYE ) == HIGH && digitalRead( REYE ) == LOW){
        
        return 2;
        }
    
    
      else if (digitalRead( LEYE ) == LOW && digitalRead( REYE ) == HIGH){
         return 3;
        
        }
      else {
         return 4;
        
        }
  
  
  }
  

void loop() {

   
  Serial.write(WiFi.status());
  c = client.read();
  
  
  
  if (c_previous!=c && c!=255){
    c_previous=c;
    }
  
  if (c_previous=='G') {
    if (Objectdetected()>15) {
     
        a=IR_Sensors ();

        switch (a){
          case 1 :
            forward ();
          break;
          case 2 :
            right ();
          break;
          case 3 :
            left ();
          break;
          case 4 :
            brakes ();
          break;
          default:
            brakes();
          break;
          }
     }
  } else{
   
    brakes ();
    
    }
 }

   
