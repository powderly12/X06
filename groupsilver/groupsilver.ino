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
const byte LEYE = 2; // INTERUPT
const byte REYE = 3; // INTERUPT
const int LED = 13;
int distance;
int directions;
long duration;
int timer1;
int timer2=50000;
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double long cumError, rateError;
double out;
 

WiFiClient client;
IPAddress server(192,168,159,196);

// Motor B connections



void setup() {
  // Set all the motor control pins to outputs
  setPoint = 15;
 
  Serial.begin(9600); 
  WiFi.begin(ssid, pass);
  pinMode(LED, OUTPUT);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  pinMode(LED, LOW);
  if (client.connect(server,5200)){
    client.println("Banba is connected, Kp=8, Ki=0.35, Kd=0."); // successed connecting
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
  attachInterrupt(digitalPinToInterrupt(LEYE),IR_Sensors,RISING);
  attachInterrupt(digitalPinToInterrupt(REYE),IR_Sensors,RISING);
 
}
void forward () {  // PWM 0 = 0V (LOW) 255 = 5V HIGH SPEED VARIES INBETWEEN

  analogWrite(in1, 0);
  analogWrite(in2, output);
  analogWrite(in3, 0);
  analogWrite(in4, output);
  }

void reverse () {  // PWM 0 = 0V (LOW) 255 = 5V HIGH SPEED VARIES INBETWEEN

  analogWrite(in1, output);
  analogWrite(in2, 0);
  analogWrite(in3, output);
  analogWrite(in4, 0);
 
  }



void left() {
  analogWrite(in1, 0); //grounded
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, output);
  
  }

 void right () {
  analogWrite(in1, 0);
  analogWrite(in2, output);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  
  
  }

  void reverse_left() {
  analogWrite(in1, 0); //grounded
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, output);
  
  }

 void reverse_right () {
  analogWrite(in1, 0);
  analogWrite(in2, output);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  
  
  }

   void brakes () {
   
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
 
  }


double kp = 10;
double ki = 1;
double kd = 2;

double computePID(double inp){     
  currentTime = millis();                //get current time
  elapsedTime = currentTime - previousTime;        //compute time elapsed from previous computation
  

    double error = abs(setPoint - inp);
    
    cumError += (error * elapsedTime)/1000; 
  
  rateError = (error - lastError)/elapsedTime;   // compute derivative

  out = kp*error + ki*cumError + kd*rateError;  
    if(out>255){
      out=255;
      }
      if(out<0){
      out=0;
      }

    lastError = error;                                //remember current error
    previousTime = currentTime;//remember current time
    Serial.println(out);
    
 
    return out;
  
}


   
             
           
     
         



 int Objectdetected() {// US sensor
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration / 58;
    double distance2=distance;
    if (distance>30){
      distance2=30;
      }
    if (distance<6){
      distance2=0;
      }
         
      output=computePID(distance2);
     // client.println(output);
      
        return distance;
     }

      


  
  
void IR_Sensors (){ // checking the IR sensors status
  
   if (digitalRead( LEYE ) == HIGH  && digitalRead( REYE ) == HIGH){
        directions=1;
        
        
        }
    
      else if (digitalRead( LEYE ) == HIGH && digitalRead( REYE ) == LOW){
        directions=2;
        
        }
    
    
      else if (digitalRead( LEYE ) == LOW && digitalRead( REYE ) == HIGH){
        directions=3;
        
        
        }
      else {
        directions=4;
        
        
        }
  
  
  }
 
void forward_drive() {
    
        
        switch (directions){ // I used a switch statement just to mess around but it does the same thing and the series of if statements in IR_Sensors
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
           // brakes ();
          break;
          default:
          // brakes();
          break;
  
  
  }
}

  void reverse_drive() {
    
        
        switch (directions){ // I used a switch statement just to mess around but it does the same thing and the series of if statements in IR_Sensors
          case 1 :
            reverse ();
          break;
          case 2 :
           reverse_left();
          break;
          case 3 :
           reverse_right();
          break;
          case 4 :
           // brakes ();
          break;
          default:
           //brakes();
          break;
  
  
  }
  } 

void loop() {

   
  WiFi.status();
  c = client.read();// input from processing GUI
  timer1=millis();

  
  if (c_previous!=c && c!=255){// makes sure the Command is not repeated and is not a  null character from the server
    c_previous=c; 
    }
  
  if (c_previous=='G') { // only options for c_previous are G for go or S for stop
    int distance_reading=Objectdetected();
    if(abs(timer1-timer2)>3000){
      timer2=millis();
      
      if (distance_reading>30) {
        client.println("Object not seen.Accelerating forward to max speed");
        //cumError=0;
        }else if (distance_reading>20) {
        client.println("Object seen more than 20 cms away.Accelerating forward to achieve safe distance");
        }
      
      if (distance_reading==15) {
        client.println("Object seen 15 cms away.Moving to keep safe distance");
        }

      if (distance_reading<15) {
        client.println("Object seen less than 15 cms away.Reversing to achieve safe distance");
        }
      if (distance_reading<10&&distance_reading>5) {
        client.println("Object seen less than 10 cms away.Reversing to achieve safe distance");
        }
    }
    if (distance_reading>15) { 
      forward_drive();
          }
    if (distance_reading<15) { 
      reverse_drive();
      } if(distance_reading==15) {
        cumError=0;
        brakes ();
        
        }  
          
    
  } else{ // if c_previous is S for stop
   
    brakes ();
    
    }
 }

   
