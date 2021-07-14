import controlP5.*;
import processing.net.*;
String whatClientSaid;
ControlP5 p5;
Button go;
Button stop;
Server myServer;
 long timer1;
 long  timer2=50000;

String whatClientSaidprevious="starting";

color buttonColor=color(0,250,0);
color buttonColor2=color(250,0,0);
char whatlastsaid='G';




void setup() {
  
  myServer = new Server(this, 5200);

  size(800, 600);
  background(0,0,0);
  textSize(20);
  ControlFont cf1 = new ControlFont(createFont("Arial",30));

 
  
  p5= new ControlP5(this);
  go= p5.addButton("GO");
  go.setSize(200,200);
  go.setFont(cf1);
  go.setPosition(150,150);
  go.setColorActive(buttonColor);
  go.updateSize();
  
 

  stop=p5.addButton("STOP");
  stop.setSize(200,200);
  stop.setFont(cf1);
  stop.setPosition(450,150);
  stop.setColorActive(buttonColor2);
  stop.updateSize();
  

  
  
}



void draw() {
  
  if(whatlastsaid=='G'){
  timer1=millis();
  Client thisClient = myServer.available();
  if (thisClient !=null) {
   
     whatClientSaid = thisClient.readStringUntil('.');
     /* test
     whatClientSaidprevious=whatClientSaid;
     println("test:");
     println(whatClientSaid); 
     println(whatClientSaidprevious);
     println("end.");
      */
     if (whatClientSaid != null &&  whatClientSaid.equals(whatClientSaidprevious)==false){
       if (abs(timer1-timer2)>500){
           clear();
           timer2=millis();
           println(whatClientSaid);
           text(whatClientSaid, 600/2, (2*600)/3);
           whatClientSaidprevious=whatClientSaid;
         }    
   
  
           
    } 
    
   
}
  }
}
  

public void GO() {
 
  println("Server says GO!");
   myServer.write("G");
   whatlastsaid='G';

}


public void STOP() {
 
  println("Server says Stop!");
   myServer.write("S");
   whatlastsaid='S';
}
  
