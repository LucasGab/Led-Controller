const int red_control = 2;          
const int blue_control = 4;
const int green_control = 3;
const int thresh = 300;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(red_control, OUTPUT);
  pinMode(blue_control, OUTPUT);
  pinMode(green_control, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned int sample;
  int red_color = 0;
  int blue_color = 0;
  int green_color = 0;
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
   // collect data for 50 mS
   // 506 MAX
   while (millis() - startMillis < sampleWindow){
      sample = analogRead(A0);
      if (sample < 1024) { // toss out spurious readings
         if (sample > signalMax){
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin){
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;
   
   pitchConv(peakToPeak,&red_color,&blue_color,&green_color);  
   
   analogWrite(red_control, red_color);
   analogWrite(blue_control, blue_color);
   analogWrite(green_control, green_color);
   delay(30);
}
/**
 * Converts the analog brightness reading into a percentage
 * 100% brightness is 614.. about 3 volts based on frequency to voltage converter circuit
 * The resulting percentage can simply be multiplied on the rgb values when setting our colors,
 * for example black is (0,0,0) so when volume is off we get 0v and all colors are black (leds are off)
 */
double convBrightness(int b) {
  double c = b /(double) thresh;
  Serial.println(b);
  Serial.println(c);
  if( c < 0.2 ) {
    c = 0;
  }
  else if(c > 1) {
    c = 1.00;
  }
  return c;
}

void setColor(int a,int ba,int c,int *r,int *b, int *g){
  *(r) = a;
  *(b) = ba;
  *(g) = c;
}

void pitchConv(int p,int *r,int *b,int *g){
  double bright = convBrightness(p);
 
 if(p <= 70) {
    int cor_b = p*255.0/70.0;
     setColor(255, 0, cor_b,r,b,g);
  }
  else if(p <= 100) {
    int cor_r = p*255.0/100.0;
    setColor(0, cor_r, 255,r,b,g);
  }
  else if(p <= 150) {
    int cor_g = p*255.0/150.0;
    setColor(255, cor_g, 0,r,b,g);
  }
  else if(p <= 200) {
    int cor_r = p*255.0/200.0;
    setColor(cor_r, 255, 100,r,b,g);
  }
  else if(p <= 230) {
    int cor_b = p*(255.0/230.0);
    setColor(255, 100, cor_b,r,b,g);
  }
  else if(p <= 270) {
    int rb = 255-((p-200)*2);
    setColor(rb, 255, rb,r,b,g);
  }
  else if(p <= 300  ) {
    setColor(0, 255-(p-210), (p-210),r,b,g);
  }
  else {
    setColor(255, 0, 0,r,b,g);
  }
  *(r) = *(r)*bright;
  *(b) = *(b)*bright;
  *(g) = *(g)*bright;
  
}
