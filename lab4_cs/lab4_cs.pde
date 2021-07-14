void setup() {
  size(800, 600);
}


int value = 0;
int mouseXprev=0;
int mouseYprev=0;
int loop=0;
color lines = color(0, 0, 0);


void mousePressed() {

  line(mouseX-50, mouseY, mouseX+50, mouseY);
  stroke(lines);
  strokeWeight(30);
  if (loop>0) {
    fill(lines);
    rect(mouseX-50, mouseY, abs((mouseXprev-50)-(mouseXprev+50)), abs(mouseYprev-mouseY));
  }
  mouseXprev=mouseX;
  mouseYprev=mouseY;
  loop++;
}




void draw() {


  if (mousePressed==true) {
    mousePressed();
  } else {
    loop=0;
  }

  switch (key) {

  case '1':
    lines = color(200, 0, 0);
    break;

  case '2':
    lines = color(0, 200, 0);
    break;
  case '3':
    lines = color(0, 0, 200);
    break;
  case '4':
    lines = color(50, 200, 100);
    break;
  case '5':
    lines = color(200, 100, 50);
    break;

  case '6':
    lines = color(100, 200, 50);
    break;

  case '7':
    lines = color(200, 50, 100);
    break;
  case '8':
    lines = color(5, 12, 200);
    break;
  case '9':
    lines = color(12, 200, 12);
    break;

    default:
      lines = color(0, 0, 0);
  }
}
