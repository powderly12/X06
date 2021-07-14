
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
const int TRIG = 8; //
const int LEYE = 2; // INTERUPT
const int REYE = 3; // INTERUPT
const int LED = 13;
int distance;
int distance_reading_prev=1000;
long duration;
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
  setPoint = 10;
 
  Serial.begin(9600); 
  WiFi.begin(ssid, pass);
  pinMode(LED, OUTPUT);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  pinMode(LED, LOW);
  if (client.connect(server,5200)){
    client.print("Banba is connected, Kp=8, Ki=35/100, Kd=0."); // successed connecting
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
  analogWrite(in2, output);
  analogWrite(in3, 0);
  analogWrite(in4, output);
  }

void reverse (){
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

   void brakes () {
   
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
 
  }


double kp = 8.5;
double ki = 0.35;
double kd = 0;

double computePID(double inp){ //PID control was not the most suitable for this challenge so we just used PI control this will be explain more in report
  // we did this on advice of one of the teaching staff so we are not trying to break the rules!! 
  currentTime = millis();                //get current time
  elapsedTime = currentTime - previousTime;        //compute time elapsed from previous computation
    if(inp>30){
      inp=30;
      
      }

    double error = (inp - setPoint);
    
    cumError += ((error * elapsedTime)); 
  
  //rateError = ((error - lastError)/(elapsedTime));   // compute derivative not used PI control for buggy

  out = kp*abs(error) + ki*cumError;// + kd*rateError; 
  out=abs(out); 
  if(cumError>255){//keeps cumError from reaching a number too large
      cumError=255;
      }
    if(cumError<-255){
      cumError=-255;
      }
      
    //lastError = error;  //remember current error
    previousTime = currentTime;  //remember current time
    /Serial.println(out);/

      out=abs(out);
      if(out>255){ //keeps out in range of 0-255
      out=255;
      }
   
    
    
 
    return out;
  
}
   
             
           
     
         



 int Objectdetected() {// US sensor
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = ceil(duration / 58);
    
    if (distance<5){
      distance=0;
      }
   
       output=computePID(distance);

      
      
        return distance;
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
 
void forward_drive() {
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
           // brakes ();
          break;
          default:
          // brakes();
          break;
  
  
  }
}


   
           
     
  

void loop() {

   

   
  WiFi.status();
  c = client.read();// input from processing GUI

  int distance_reading=Objectdetected();
  
  if (c_previous!=c && c!=255){// makes sure the Command is not repeated and is not a  null character from the server
    c_previous=c; 
    }
  
  if (c_previous=='G') { // only options for c_previous are G for go or S for stop
    
  
    if (distance_reading>10) { 
      forward_drive();
          }
    if (distance_reading<=10) { 
       
        brakes ();
    }
    if (distance_reading>30) {
        client.println("Object not seen.");
       
    }
    if(distance_reading!=0 &&distance_reading<30 ) { //strings together out put with updated distance
        String myString;
        myString.concat("Object seen ");
        myString.concat(distance_reading-10);
        myString.concat(" cm away.");
        client.print(myString);
        distance_reading_prev=distance_reading;
     }  
    
  } else{ // if c_previous is S for stop
   
    brakes ();
    
    }
 }

   
