
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
bool switchs=true;
String statement="object seen.";
int timer1;
int timer2;

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
  pinMode(LED, LOW);
  if (client.connect(server,5200)){
    client.print("Banba is connected."); // successed connecting
    pinMode(LED, HIGH);
    }else {
      Serial.print("Fail.");
       pinMode(LED, LOW);
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

  timer2=millis();
  if(abs(timer2-timer1)>500&&switchs==true){// this loops makes sure that it print after 0.5 s of stopping (bc the us sensor reports stoping alot before actually coming to a full stop)
    client.print("Path Clear.");
    switchs=false;
    }
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

 int Objectdetected() {// US sensor
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration / 58;
    
    
    if (distance<=10) {
     timer1=millis();
        
     if (switchs==false &&abs(timer2-timer1)>500){ //same reasons the us sensor is unsure of the distance and repeat so we wait 0.5s to ensure the distance is definately less than 10cm
        client.print(statement);
        switchs=true; //switch is just to stop the "object seen" statement being repeated. more for the path clear statement in the forward function above
        
      }
       
       brakes ();
       
      
        return distance;
     }

      


  
  }
int IR_Sensors (){ // checking the IR sensors status
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

   
  //WiFi.status();
  c = client.read();// input from processing GUI
  

  
  if (c_previous!=c && c!=255){// makes sure the Command is not repeated and is not a  null character from the server
    c_previous=c; 
    }
  
  if (c_previous=='G') { // only options for c_previous are G for go or S for stop
    if (Objectdetected()>10) { //if the object detected is more than 10
      // This series of if statements creates leveling in the programing so the ir sensors are only checked if the us sensor is more than 10cm away 
      // and the Us sensor is only checked if the server says go. This reduces the processing power used at an instant and sensors are only called when needed
      
        a=IR_Sensors ();
        
        switch (a){ // I used a switch statement just to mess around but it does the same thing and the series of if statements in IR_Sensors
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
  } else{ // if c_previous is S for stop
   
    brakes ();
    
    }
 }

   
