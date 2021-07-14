#include <Arduino_LSM6DS3.h>
#include <WiFiNINA.h>


char ssid[] = "36a Bow Lane";
char pass[] = "letmein123";
int a;


// Left Motor connections

float gX, gY, gZ;
char c;
int loops3=1;
const double pi=3.14159265358979;
const int in1 = 5; // 2 on H brigde PMW
const int in2 = 6; // 6 on H brigde PMW
const int in3 = 10; // 10 on H brigde PMW
const int in4 = 9; // 15 on H brigde PMW
const int ECHO = 7; //
const int TRIG= 8; //
const int LEYE = 2; // INTERUPT
const int REYE = 3; // INTERUPT
const int LED = 13;
double  timer1,drawing_time,turning_timer;
long gZcumulated=0;
int counter=0;
long radainsPerSec;
int current_coordinates[2];
int previous_coordinates[2];
int drawing_distance;
int loops2=1;
char c_previous;
bool switchs=false;
int loops=1;
double time_Angle;
double true_time_Angle;
double true_drawing_time;
float currentAngle=0;
float previousAngle=0;

 

WiFiClient client;
IPAddress server(192,168,159,196);

// Motor B connections



void setup() {
  // Set all the motor control pins to outputs
 
  current_coordinates[1]=0;
  previous_coordinates[1]=0;
  current_coordinates[0]=0;
  previous_coordinates[0]=0;
  Serial.begin(9600); 
  WiFi.begin(ssid, pass);
  pinMode(LED, OUTPUT);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  pinMode(LED, LOW);
  if (client.connect(server,5200)){
    client.print('Y'); // successed connecting
    pinMode(LED, HIGH);
    }else {
      Serial.print("Fail.");
       pinMode(LED, LOW);
     }
     
  

   if (!IMU.begin()){

     Serial.println ("failed to initialise IMU");
      while (true);
     } else{
      
       Serial.println ("initialised IMU");
      }
    

  
  c_previous='p';
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



void brakes () {
   
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
 
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

void calculate_angle(){
   
   
   currentAngle= (pi/180)*(radainsPerSec*true_time_Angle);
   
   }
 
void breakpoint(){
    
    client.print('H');//draws circle

    calculate_angle();
    
    current_coordinates[0]= ceil(drawing_distance*cos(currentAngle+(1.1*previousAngle))+previous_coordinates[0]);
    current_coordinates[1]= ceil(drawing_distance*sin(currentAngle+(1.1*previousAngle))+previous_coordinates[1]);
    Serial.println(drawing_distance);
    previousAngle=currentAngle;

    previous_coordinates[0]=current_coordinates[0];
    previous_coordinates[1]=current_coordinates[1];

   String    x_coordinate = String(current_coordinates[0]);
   String  y_coordinate = String(current_coordinates[1]);
         
    client.print(x_coordinate);
    delay(600);
    client.print(y_coordinate);
   // Serial.print(current_coordinates[0]);
    //Serial.println(current_coordinates[1]);
    drawing_distance=0;
    gZcumulated=0;
    counter=0;
    radainsPerSec=0;

  
  }  

 



  void forward(){

    if (IR_Sensors ()==1){
      analogWrite(in1, 0);
      analogWrite(in2, 100);
      analogWrite(in3, 0);
      analogWrite(in4, 100);
      loops3=1;
      //Serial.println(currentAngle);

      drawing_time=+(micros()-timer1)/1000000;
      if (drawing_time>0){
      true_drawing_time=drawing_time;}
      drawing_distance=(true_drawing_time)*(588/8);
    }
    if (IR_Sensors ()!=1){
      if(loops3==1){
      client.print('J');
      }
      loops3=2;
      brakes();
      }

    
 }
      
         
  
  void turn_left(){ 

     /* if (loops==1){
      breakpoint();
      loops=2;
      }*/ 

      
     // drawing_distance=0;
      analogWrite(in1, 100); //grounded
      analogWrite(in2, 0);
      analogWrite(in3, 0);
      analogWrite(in4, 100);
      
      time_Angle=+(micros()-turning_timer)/1000000;
      if (time_Angle>0 && time_Angle<1000 ){
      true_time_Angle=time_Angle;}
      //Serial.println(true_time_Angle);
       if (IMU.gyroscopeAvailable()){
  
          IMU.readGyroscope(gX, gY, gZ);
          gZcumulated=gZcumulated+gZ;
          counter++;
          radainsPerSec=gZcumulated/counter;
       }
    //Serial.println(radainsPerSec);
    
  }

     
    
   
  void turn_Right(){
      /* if (loops==1){
      breakpoint();
      loops=2;
      }*/
      //drawing_distance=0;     
      analogWrite(in1, 0); //grounded
      analogWrite(in2, 100);
      analogWrite(in3, 100);
      analogWrite(in4, 0);

        time_Angle=+(micros()-turning_timer)/1000000;
      if (time_Angle>0 &&time_Angle<1000){
      true_time_Angle=time_Angle;}
      

       if (IMU.gyroscopeAvailable()){
  
    IMU.readGyroscope(gX, gY, gZ);
    gZcumulated=gZcumulated+gZ;
    counter++;
    radainsPerSec=gZcumulated/counter;
    
  }
   // Serial.println(gZcumulated);
  }
  void erase(){
      if (loops2==1){
      //breakpoint();
      loops=2;
      }
      if (IR_Sensors ()==1){
      analogWrite(in1, 0);
      analogWrite(in2, 200);
      analogWrite(in3, 0);
      analogWrite(in4, 200);
      }else{
        client.print('D');
        current_coordinates[1]=0;
        previous_coordinates[1]=0;
        current_coordinates[0]=0;
        previous_coordinates[0]=0;
        brakes();
        switchs=false;
        loops=1;
        loops2=1;
        currentAngle=0;
        previousAngle=0;
        drawing_distance=0;
        gZcumulated=0;
        counter=0;
        radainsPerSec=0;
        timer1=0;
        drawing_time=0;
        turning_timer=0;
        time_Angle=0;
        true_time_Angle=0;
        true_drawing_time=0;
        loops3=1;

        
      
      }
}
      



        
      
     
  

void loop() {
  WiFi.status();

 
  c = client.read();// input from processing GUI
  
   
  
  if (c_previous!=c && c!=255){// makes sure the Command is not repeated and is not a  null character from the server
    c_previous=c;
    loops=1;
    
    }
    //IMU.readGyroscope(gX, gY, gZ);
    //Serial.println(gZ);
  if(c_previous=='G') {
    
      switchs=true;
      
      

 

    
    
    }else if(c_previous=='S'){
      
      switchs=false;
      if (loops==1){
      breakpoint();
      loops=2;
      }
   
      
   }

   //Serial.println(switchs);
  
  
  if (switchs==true) { // only options for c_previous are G for go or S for stop
    
  
    if (c_previous=='F') { 
      if (loops==1){
          timer1=micros();
          loops=2;
      }
          loops2=1;
          forward();
      
          }
    if (c_previous=='L') { 
        if(loops2==1){
          turning_timer=micros();
          loops2=2;
          }
          turn_left();
          calculate_angle();
          }
    if (c_previous=='R') {
          if(loops2==1){
          turning_timer=micros();
          loops2=2;
          } 
          turn_Right();
          calculate_angle();
          
          }

     if (c_previous=='E') { 

          erase();
          
          } 
        
        
    
  } else{ // if c_previous is S for stop
    
    brakes ();
    
    
 }
}
  
  



  
  
  

   
