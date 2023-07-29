const int t1 = 5;
const int e1= 18;

const int t2 = 20;
const int e2= 21;

float d1 =0;
float d2 =0;
int student =0;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(t1, OUTPUT); // Sets the trigPin as an Output
  pinMode(e1, INPUT); // Sets the echoPin as an Input4
  pinMode(t2, OUTPUT); // Sets the trigPin as an Output
  pinMode(e2, INPUT); // Sets the echoPin as an Input
}

void loop() {
d1 =getdistance(t1,e1);
d2 =getdistance(t2,d2);

if(d1<5){
//student enters the class room
 delay(1000);
 student++;
}

else if (d2<5){
//student leave the class
if(student>0){
  student--;
  delay(1000);
  }

}

  
}


float getdistance(int trigPin, int echopin){
 int trigpin;
 int echopin;

    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
   
   return distanceCm;

}
