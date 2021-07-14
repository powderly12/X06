import controlP5.*;
import processing.net.*;
char whatClientSaid;
ControlP5 p5;
Button go;
Button stop;
Button turn_l;
Button turn_r;
Button forwards;
Button erase;
Server myServer;
long timer1;
long  timer2=50000;

char whatClientSaidprevious='X';

color buttonColor=color(0,250,0);
color buttonColor2=color(250,0,0);
char whatlastsaid='G';

int newY;
int newX;
int oldY=618;
int oldX=100;
String newY_string;
String newX_string;
int loops=1;
       




void setup() {
  
  myServer = new Server(this, 5200);

  size(800, 900);
  background(0,0,0);
  textSize(20);
  ControlFont cf1 = new ControlFont(createFont("Arial",30));

 
  
  p5= new ControlP5(this);
  go= p5.addButton("GO");
  go.setSize(150,150);
  go.setFont(cf1);
  go.setPosition(100,250);
  go.setColorActive(buttonColor);
  go.updateSize();
  
 

  stop=p5.addButton("STOP");
  stop.setSize(150,150);
  stop.setFont(cf1);
  stop.setPosition(550,250);
  stop.setColorActive(buttonColor2);
  stop.updateSize();
  
  
  turn_l=p5.addButton("TURN_LEFT");
  turn_l.setSize(150,150);
  turn_l.setFont(cf1);
  turn_l.setPosition(100,50);
  turn_l.setColorActive(buttonColor2);
  turn_l.updateSize();
  
  turn_r=p5.addButton("TURN_RIGHT");
  turn_r.setSize(150,150);
  turn_r.setFont(cf1);
  turn_r.setPosition(550,50);
  turn_r.setColorActive(buttonColor2);
  turn_r.updateSize();

  forwards=p5.addButton("Forwards");
  forwards.setSize(150,150);
 forwards.setFont(cf1);
  forwards.setPosition(325,50);
  forwards.setColorActive(buttonColor2);
  forwards.updateSize();
  
   erase=p5.addButton("Erase");
  erase.setSize(150,150);
  erase.setFont(cf1);
  erase.setPosition(325,250);
  erase.setColorActive(buttonColor2);
  erase.updateSize();
  fill(255,255,255);
  rect(100, 410, 588,416);
  fill(200,0,0);
  circle(oldX, oldY,10);
  
  
}



void draw() {
  
  
  Client thisClient = myServer.available();
  if (thisClient !=null) {
    //whatClientSaid='V';
   
     whatClientSaid = thisClient.readChar();
   
     whatClientSaidprevious=whatClientSaid;
    // println("test:");
   // println(whatClientSaid); 
   //println(whatClientSaidprevious);
    // println("end.");
      //
      if (whatClientSaid=='Y'){
        
        println("Banba Connected");
        
      }
        
     
      if (whatClientSaid=='H'){
        
        fill(255,0,0);
        delay(400);
        newX_string=thisClient.readString();
        if (newX_string!=null){
        newX=Integer.valueOf(newX_string);
        }
        delay(400);
        newY_string=thisClient.readString();
        
        if (newY_string!=null){
        newY=Integer.valueOf(newY_string);
        }
        circle(newX+100, 618-newY,10);
        fill(255,0,0);
        println("old");
        println(oldX); 
        println(oldY);
        println("new");
        println(newX); 
        println(newY);
        line(oldX,oldY,newX+100,618-newY);
        oldX=newX+100;
        oldY=618-newY;
        loops=1;
        
        
        
        
      }
      if (whatClientSaid=='J'&&loops==1){
        println("Banba has gone of the canvas please turn");
        loops=2;
        
      }
      if (whatClientSaid=='D'){
        loops=1;
        clear();
        println("Board has been erased please place the Banba back to the start");
        delay(2000);
        fill(255,255,255);
        oldY=618;
        oldX=100;
        newY=0;
        newX=0;
        rect(100, 410, 588,416);
        fill(200,0,0);
        circle(oldX, oldY,10);
  
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

public void TURN_LEFT() {
 
  println("Server says TURN_LEFT!");
   myServer.write("L");
   whatlastsaid='L';

}

public void TURN_RIGHT() {
 
  println("Server says TURN_RIGHT!");
   myServer.write("R");
   whatlastsaid='R';

}

public void Forwards() {
 
  println("Server says Forwards!");
   myServer.write("F");
   whatlastsaid='F';

}


public void Erase() {
 
  println("Server says Erase!");
   myServer.write("E");
   whatlastsaid='E';

}
