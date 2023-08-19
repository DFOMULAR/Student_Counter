#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID ""  //input wifi name
#define WIFI_PASSWORD "" //input wifi password

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY ""  //enter your firebase API key here

/* 3. Define the RTDB URL */
#define DATABASE_URL "" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL ""
#define USER_PASSWORD ""

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

//ultrasonic sensor pin_def
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




void setup(){
  Serial.begin(115200);
  pinMode(t1, OUTPUT); // Sets the trigpin1 as an Output
  pinMode(e1, INPUT); // Sets the echoPin as an Input4
  pinMode(t2, OUTPUT); // Sets the trigPin as an Output
  pinMode(e2, INPUT); // Sets the echoPin as an Input

 #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  multi.addAP(WIFI_SSID, WIFI_PASSWORD);
  multi.run();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    if (millis() - ms > 10000)
      break;
#endif
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // The WiFi credentials are required for Pico W
  // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  config.wifi.clearAP();
  config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

#if defined(ESP8266)
                                                      // required for large file data, increase Rx size as needed.
  fbdo.setBSSLBufferSize(1024 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
  int p = 55;

   Firebase.RTDB.setInt(&fbdo, "/Incubator/No", 8);
}

void loop(){

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
 Firebase.RTDB.setInt(&fbdo, "/Incubator/No", student);

}

  // if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
  //   sendDataPrevMillis = millis();
  //   // Write an Int number on the database path test/int
  //   if (Firebase.RTDB.setInt(&fbdo, "Incubator/No", student)){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  //   //count++;
    
  //   // Write an Float number on the database path test/float
  //   if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  // }

}






float getdistance(int t, int e){

    // Clears the trigPin
  digitalWrite(t, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(t, HIGH);
  delayMicroseconds(10);
  digitalWrite(t, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(e, HIGH);
  
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
