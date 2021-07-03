////////////////////////////////////////////////////////////////////////
//  Room MODULE 2019////////////////////////////////////////////////////
/*
 * PIN SETTING INPUT / OUTPUT

 * LED PWM          - 6
 * CLOCKPIN P       - 7
 * DATAPIN P        - 8
 * INPIN BUTTON     - 14
 * INPIN BUTTON     - 15
 * INPIN BUTTON     - 16
 * INPIN BUTTON     - 17
 * INPIN BUTTON     - 18
 * INPIN BUTTON     - 19
 * INPIN BUTTON     - 50
 * INPIN BUTTON     - 45
 * INPIN BUTTON     - 2
 * INPIN BUTTON     - 3
 * OUTPUT WEB       - 22
 * OUTPUT WEB       - 23
 * OUTPUT WEB       - 24
 * OUTPUT WEB       - 25
 * OUTPUT WEB       - 26
 * OUTPUT WEB       - 27
 * OUTPUT WEB       - 28
 * OUTPUT WEB       - 29
 * OUTPUT WEB       - 30
 * OUTPUT WEB       - 31
 * OUTPUT WEB       - 32
 * OUTPUT WEB       - 33
 * OUTPUT WEB       - 34
 * OUTPUT WEB       - 35
 * OUTPUT WEB       - 36
 * OUTPUT WEB       - 37
 * MOTOR CONTROL    - 42
 * MOTOR CONTROL    - 43
 * MOTOR MODULE 1   - 38
 * MOTOR MODULE 1   - 39
 * MOTOR MODULE 2   - 40
 * MOTOR MODULE 2   - 41
 * PIR VOORDEUR     - 5
 * TEMP KAMER       - A0
 * TEMP HAL         - A15
 */
////////////////////////////////////////////////////////////////////////
#include <Ethernet.h>  // Ethernet Commands
#include <SPI.h>       // SPI Commands
#include <PubSubClient.h>//mqtt 
#include <FastLED.h>

#define    NUM_LEDS             3      // P9813 module: string is one l
#define    CLOCK_PIN            7      // Clock and data pins for LED data
#define    DATA_PIN             8 
CRGB leds[NUM_LEDS];
#include <DHT.h>
#define DHTPIN1 A0 // Analog Pin sensor is connected to
#define DHTPIN2 A15 // Analog Pin sensor is connected to
#define DHTTYPE DHT11 //21 or 22 also an option
DHT dht1(DHTPIN1, DHTTYPE);// DHT11 temperature and humidty sensor
DHT dht2(DHTPIN2, DHTTYPE);// DHT11 temperature and humidty sensor
//CONFIGURATION Room MODULE ///////////////////////////////////////////
//IP manual settings
byte ip[] = {  192, 168, 178, 63 }; // NORMAL 63 / TEST 30 SETUP
byte gateway[] = {  192, 168, 178, 1 }; //Manual setup only
byte subnet[] = {  255, 255, 255, 0 }; //Manual setup only
byte mac[] = {   0x02, 0xEE, 0xFF, 0xDE, 0xAD, 0x01 };// orginele room config. mac adres.
byte Server2[] = {192,168,178,70};// IP address of MQTT server
const char * clientID = "led_Room";
const char * user = "openhab";
const char * passw = "Ard2018EmU";
//Ethernet Port
EthernetServer server = EthernetServer(80); //default html port 80
EthernetClient ethClient;
PubSubClient client1(Server2, 1883, ethClient);
//********************************************************************
int outputQuantity = 8; ////The number of outputs going to be switched.
int outputQuantity1 = 8; //The number of outputs going to be switched.
int outputQuantity2 = 4; //The number of outputs going to be switched.
//Invert the output of the leds
boolean outputInverted = true; //true or false
boolean outputInverted1 = true; //true or false
boolean outputInverted2 = false; //true or false
// This is done in case the relay board triggers the relay on negative, rather then on positive supply
//Html page refresh
int refreshPage = 15; //default is 10sec.
//Beware that if you make it refresh too fast, the page could become inacessable.
int outputAddress[]  = {22, 23, 24, 25, 34, 35, 36, 37 };
int outputAddress1[] = {26, 27, 28, 29, 30, 31, 32, 33 };
int outputAddress2[] = {46, 47, 48, 49 };
String buttonText[8] = {
  "Love",
  "Tv Meubel",
  "Klok Dolphine",
  "fans heaters",
  "RGB Strip",
  "Bureau",
  "Kamer",
  "Beam Bureau"
};
String buttonText1[8] = {
  "Plant sens",
  "Relay6",
  "Relay7",
  "Relay8",
  "Relay13",
  "Relay14",
  "Relay15",
  "Relay16",
};
String buttonText2[4] = {
  "EXT 1",
  "EXT 2",
  "EXT 3",
  "EXT 4",
};
// Set the output to retain the last status after power recycle.
int retainOutputStatus[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //1-retain the last status. 0-will be off after power cut.
int retainOutputStatus1[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //1-retain the last status. 0-will be off after power cut.
int retainOutputStatus2[4] = {0, 0, 0, 0}; //1-retain the last status. 0-will be off after power cut.
////////////////////////////////////////////////////////////////////////
//VARIABLES DECLARATION Room MODULE  //////////////////////////////////
////////////////////////////////////////////////////////////////////////
int outp = 0;
int ldrstatus = 0;// flag for ldr status from other arduino
int spmode = 0;
boolean printLastCommandOnce = false;
boolean printButtonMenuOnce = false;
boolean initialPrint = true;
boolean reading = false;
boolean outputStatus[8]; //Create a boolean array for the maximum ammount.
boolean outputStatus1[8]; //Create a boolean array for the maximum ammount.
boolean outputStatus2[4]; //Create a boolean array for the maximum ammount.
unsigned long timeConnectedAt;
boolean writeTo = false;
// Room PUSH BUTTON SWITCH RELATED //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int inPin0 = 11;//NOT CONNECTED
int inPin1 = 12;//
int inPin2 = 14;//wand wiek
int inPin3 = 15;//rgb strip
int inPin4 = 16;//bureau
int inPin5 = 17;//
int inPin6 = 18;// USED
int inPin7 = 19;// DESK LIGHT
int inPin8 = 2;
int inPin9 = 3;
int inPin10 = 44;
int inPin11 = 45;
int plant = 1;
bool state0 = LOW; //= HIGH;
bool state1 = LOW;
bool state2 = LOW; //= HIGH;
bool state3 = LOW;
bool state4 = LOW; //= HIGH;
bool state5 = LOW;
bool state6 = LOW; //= HIGH;
bool state7 = LOW;
bool state8 = LOW; //= HIGH;
bool state9 = LOW;
bool state10 = LOW; //= HIGH;
bool state11 = LOW;
bool state12 = LOW; //= HIGH;
bool state13 = LOW;
bool state14 = LOW; //= HIGH;
bool state15 = LOW;
bool state16 = LOW; //= HIGH;
bool state17 = LOW;
bool state18 = LOW; //= HIGH;
bool state19 = LOW;
bool state20 = LOW; //= HIGH;
bool state21 = LOW;
int read0;
int read1;
int read2;
int read3;
int read4;
int read5;
int read6;
int read7;
int read8;
int read9;
int read10;
int read11;
int previous0;  //= LOW;
int previous1;
int previous2;  //= LOW;
int previous3;
int previous4;  //= LOW;
int previous5;
int previous6;  //= LOW;
int previous7;
int previous8;  //= LOW;
int previous9;
int previous10;  //= LOW;
int previous11;
int previous12;  //= LOW;
int previous13;
int previous14;  //= LOW;
int previous15;
int previous16;  //= LOW;
int previous17;
int previous18;  //= LOW;
int previous19;
int previous20;  //= LOW;
int previous21;
int pre0;  //= LOW;
int pre1;
int pre2;  //= LOW;
int pre3;
int pre4;  //= LOW;
int pre5;
int pre6;  //= LOW;
int pre7;
int pre8;  //= LOW;
int pre9;
int pre10;  //= LOW;
int pre11;
int pre12;  //= LOW;
int pre13;
int pre14;  //= LOW;
int pre15;
int pre16;  //= LOW;
int pre17;
int pre18;  //= LOW;
int pre19;
int pre20;  //= LOW;
int pre21;
int pre22;  //= LOW;
int pre23;
int pre24;  //= LOW;
int pre25;
int Allon = 1;
int Alloff = 0;
/////////////////////////////////////////////////////////////////////////////////////////////
// time settings ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
long time = 0;
long debounce = 200;
unsigned long StartMillis; // start millis on startup arduino is zero
unsigned long currentMillis;
unsigned long StartMillis6; // start millis on startup arduino is zero
unsigned long StartMillis8; // start millis on startup arduino is zero
unsigned long StartMillis9; // start millis on startup arduino is zero
unsigned long readTime;
unsigned long StartMillis5; // start millis on startup arduino is zero
unsigned long StartMillis4; // start millis on startup arduino is zero
long lastReconnectAttempt = 0;
//////////////////////////////////////////////////////////////////////////////////////////////
// MOTOR STOP UP STOP DOWN STOP CODE. ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
int motorPin1 = 42;     // motor push switch is connected to pin ?
int motorPin2 = 43;     // motor push switch is connected to pin ?
int out1 = 38;          // L298N IN1  MOTOR 1 UP
int out2 = 39;          // L298N IN2  MOTOR 1 DOWN
int out3 = 40;          // L298N IN3  MOTOR 2 UP
int out4 = 41;          // L298N IN4  MOTOR 2 DOWN
int out5 = 20;
int out6 = 21;
//////////////////////////////////////////////////////////////////////////////////////////////
int DataR;
int DataG;
int DataB;
int DataP;
int PWM = 9;
int sensorPin = 6;  //hek sensor open close voor
int pirPin1 = 5; //pin for the pir sensor at the front door
int pir1old = 0;  
int pir1 = 0;
int flag1old = 0;
int flag1 = 0;
bool alarm0 = 0;
int alarmold0 = 0;
int sensor0 = 0;
int sensorold0 = 0;
// VARIABLES /////////////////////////////////////////////////////////////////////////////////
      // variables to hold the parsed data
float t1;
float h1;
float hic1;
float t2;
float h2;
float hic2;
boolean newData = false;
//////////////////////////////////////////////////////////////////////////////////////////////
int motor0;                    // variable for reading the pin status
int motor1;                    // variable for reading the delayed status
int motor2;                    // variable for reading the pin status
int motor3;                    // variable for reading the delayed status
int motorState;                // variable to hold the button state
int motorState1;               // variable to hold the button state
int motorMode = 0;             // What mode is the motor in?
int motorMode1 = 0;            // what mode is the motor in?
/////////////////////////////////////////////////////////////////////
// ANALOG INPUTS Related Reading KAMER MODULE ///////////////////////
/////////////////////////////////////////////////////////////////////
//const int tempInPin    = A0;// analog input temperature NOT CONNECTED
const int voltage      = A1;// analog input voltage 12V
const int amperage     = A3;// analog input amperage 12V
const int voltage1     = A2;// analog input voltage 5V
const int amperage1    = A4;// analog input amperage 5V
int plant1       = A5;// plant read input NC
int plant2       = A6;// plant read input NC
int plant3       = A7;// plant read input NC
int plant4       = A8;// plant read input NC
int plant5       = A9;// plant read input NC
int plant6       = A10;// plant read input NC
int plant7       = A11;// plant read input NC
int plant8       = A12;// plant read input NC
int plant9       = A13;// plant read input NC
int plant10      = A14;// plant read input NC
//int plant11      = A15;// plant read input NC
int Plant1       = 0;// plant read input NC
int Plant2       = 0;// plant read input NC
int Plant3       = 0;// plant read input NC
int Plant4       = 0;// plant read input NC
int Plant5       = 0;// plant read input NC
int Plant6       = 0;// plant read input NC
int Plant7       = 0;// plant read input NC
int Plant8       = 0;// plant read input NC
int Plant9       = 0;// plant read input NC
int Plant10      = 0;// plant read input NC

int voltInValue        = 0; //voltage read 1
int voltInValue1       = 0; //voltage read 2
int ampInValue         = 0; //amperage read 1
int ampInValue1        = 0; //amperage read 2
double voltOutValue       = 0; //voltage formatted
double voltOutValue1      = 0; //voltage formatted
double volt12        = 0.0; //
double volt5         = 0.0; //
double amp1          = 0.0; //
double amp2          = 0.0; //

String readString;  //
////////////////////////////////////////////////////////////////////////
//RUN ONCE Room MODULE ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//Beginning of Program
void setup() {
  Serial.begin(115200);
 // LEDS.addLeds<P9813, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);  // initialize LED control object
  pinMode(inPin0, INPUT);  // input push button 
  pinMode(inPin1, INPUT);  // input push button 
  pinMode(inPin2, INPUT);  // input push button  
  pinMode(inPin3, INPUT);  // input push button 
  pinMode(inPin4, INPUT);  // input push button 
  pinMode(inPin5, INPUT);  // input push button 
  pinMode(inPin6, INPUT);  // input push button 
  pinMode(inPin7, INPUT);  // input push button 
  pinMode(inPin8, INPUT);  // input push button 
  pinMode(inPin9, INPUT);  // input push button 
  pinMode(inPin10, INPUT);  // input push button 
  pinMode(inPin11, INPUT);  // input push button 
  pinMode(pirPin1, INPUT);
  digitalWrite(pirPin1, LOW);  
  pinMode(sensorPin, INPUT); // input push button
  digitalWrite(sensorPin, HIGH);// input write low   
  // motor inputs and outputs ///////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  pinMode(motorPin1, INPUT);    // Set the motor switch pin as input 42
  pinMode(motorPin2, INPUT);    // Set the motor switch pin as input 43
  pinMode(out1, OUTPUT);        // L298N MOTOR 1 UP
  pinMode(out2, OUTPUT);        // L298N MOTOR 1 DOWN
  pinMode(out3, OUTPUT);        // L298N MOTOR 2 UP
  pinMode(out4, OUTPUT);        // L298N MOTOR 2 DOWN
  pinMode(out5, OUTPUT);        // L298N MOTOR 2 UP
  digitalWrite(out5, HIGH);  

  pinMode(out6, OUTPUT);        // L298N MOTOR 2 DOWN
  digitalWrite(out6, HIGH);  
  
  ///////////////////////////////////////////////////////////////////////
  
  // set motorstatusses /////////////////////////////////////////////////
  motorState  = digitalRead(motorPin1);   // read the initial state
  motorState1 = digitalRead(motorPin2);   // read the initial state
  //Set pins as Outputs /////////////////////////////////////////////////
  boolean currentState = false;
  int var;
  for (int i = 0; i < outputQuantity; i++) {
    pinMode(outputAddress[i], OUTPUT);
    var = outputAddress[i];
    //Switch all outputs to either on or off on Startup
    if (outputInverted == true) {
      if (outputStatus[i] == 0) {
        currentState = true;
      }
      else {
        currentState = false; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var, currentState);
    }
    else {
      if (outputStatus[i] == 0) {
        currentState = false;
      }
      else {
        currentState = true; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var, currentState);
    }

  }
  boolean currentState1 = false;
  int var1;
  for (int j = 0; j < outputQuantity1; j++) {
    pinMode(outputAddress1[j], OUTPUT);
    var1 = outputAddress1[j];
    //Switch all outputs to either on or off on Startup
    if (outputInverted1 == true) {
      //digitalWrite(outputAddress[var], HIGH);
      if (outputStatus1[j] == 0) {
        currentState1 = true;
      }
      else {
        currentState1 = false; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var1, currentState1);
    }
    else {
      //digitalWrite(outputAddress[var], LOW);
      if (outputStatus1[j] == 0) {
        currentState1 = false;
      }
      else {
        currentState1 = true; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var1, currentState1);
    }
  }
  boolean currentState2 = false;
  int var2;
  for (int k = 0; k < outputQuantity2; k++) {
    pinMode(outputAddress2[k], OUTPUT);
    var2 = outputAddress2[k];
    //Switch all outputs to either on or off on Startup
    if (outputInverted2 == true) {
      //digitalWrite(outputAddress2[var], HIGH);
      if (outputStatus2[k] == 0) {
        currentState2 = true;
      }
      else {
        currentState2 = false; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var2, currentState2);
    }
    else {
      //digitalWrite(outputAddress2[var], LOW);
      if (outputStatus2[k] == 0) {
        currentState2 = false;
      }
      else {
        currentState2 = true; //check outputStatus if off, switch output accordingly
      }
      digitalWrite(var2, currentState2);
    }
  }    
  //Setting up the IP address. Comment out the one you dont need.
  client1.setServer(Server2, 1883);
  client1.setCallback(callback);  
  Ethernet.begin(mac, ip, gateway, subnet); //for manual setup. (Address is the one configured above.)
  dht1.begin();
  dht2.begin();
  server.begin();
  Serial.print("Server started at ");
  Serial.println(Ethernet.localIP());
  lastReconnectAttempt = 0;
  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);  // initialize LED control object
  Serial.println("p9813 ge-init");
   readTime = 0;
  delay(500);
}
////////////////////////////////////////////////////////////////////////
//LOOP Room MODULE ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void loop() {
  currentMillis = millis();
  plants();
//  rgbserial();
  analoginput();      //temperature void and other stuff related analog input  
  sensormain();       // BUTTON LOOP 
  sensorRead();
  motor();            // TWO MOTOR DRIVE FOR CURTAINS 
  pir();
  checkForClient();   // CLIENT LOOP 
  mqtt();

}
/*void rgbserial() {
  Serial.println("loop started");
    recvWithStartEndMarkers();
    if (newData == true) {
      Serial.println("new data == true");
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        Serial.println("parsed data showed");
        newData = false;
        Serial.println("new data is false");
    }
    leds[0].setRGB( DataRed, DataGreen, DataBlue);
    FastLED.show(); 
    delay(30); 
    Serial.println("init leds ");
    delay(1000);
}*/
void mqtt() {
 
     if (!client1.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000){
        lastReconnectAttempt = now;
        if ( mqttReConnect()){
          lastReconnectAttempt = 0;
        }
      }
   }
    else{
      client1.loop();
      } 
    if (currentMillis - StartMillis5 >= 30000){
    char msgBuffer[20];  
    client1.publish("Ro/I/mq49/st", (state0 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq50/st", (state1 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq51/st", (state2 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq52/st", (state3 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq53/st", (state4 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq54/st", (state5 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq55/st", (state6 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq56/st", (state7 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq57/st", (state12 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq58/st", (state13 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq59/st", (state14 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq60/st", (state15 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq61/st", (state16 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq62/st", (state17 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq63/st", (state18 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq64/st", (state19 == LOW) ? "ON" : "OFF");
    client1.publish("Ro/I/mq65/st", (state8 == HIGH) ? "ON" : "OFF");
    client1.publish("Ro/I/mq66/st", (state9 == HIGH) ? "ON" : "OFF");
    client1.publish("Ro/I/mq67/st", (state10 == HIGH) ? "ON" : "OFF");
    client1.publish("Ro/I/mq68/st", (state11 == HIGH) ? "ON" : "OFF"); 
    client1.publish("Ro/I/mq69/st", (state20 == HIGH) ? "ON" : "OFF");
    client1.publish("Ro/I/mq70/st", (state21 == HIGH) ? "ON" : "OFF");     
    client1.publish("Ro/I/RVolt1/st", dtostrf(volt12, 3, 2 , msgBuffer ));
    client1.publish("Ro/I/RAmp1/st", dtostrf(amp1, 3, 2, msgBuffer ));
    client1.publish("Ro/I/RVolt2/st", dtostrf(volt5, 3, 2 , msgBuffer ));
    client1.publish("Ro/I/RAmp2/st", dtostrf(amp2, 3, 2 , msgBuffer ));
    client1.publish("Ro/I/RTMP/st",dtostrf(t1,3, 2, msgBuffer));
    client1.publish("Ro/I/RHUM/st", dtostrf(h1,3, 2, msgBuffer));
    client1.publish("Ro/I/HTMP/st",dtostrf(t2,3, 2, msgBuffer));
    client1.publish("Ro/I/HHUM/st", dtostrf(h2,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl1/st",dtostrf(plant1,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl2/st",dtostrf(plant2,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl3/st",dtostrf(plant3,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl4/st",dtostrf(plant4,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl5/st",dtostrf(plant5,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl6/st",dtostrf(plant6,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl7/st",dtostrf(plant7,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl8/st",dtostrf(plant8,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl9/st",dtostrf(plant9,3, 2, msgBuffer));
    client1.publish("Ro/I/RPl10/st",dtostrf(plant10,3, 2, msgBuffer));
    Serial.print("\n");        
    StartMillis5 = currentMillis;
  }
}
void sensorRead(){ 
  if (currentMillis - StartMillis6 >= 25000){ 
   readTime = millis();
 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h1 = dht1.readHumidity();
  h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  t1 = dht1.readTemperature();
  t2 = dht2.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1)) {
   // Serial.println("Failed to read from DHT1 sensor!");
    return;
  }
  if (isnan(h2) || isnan(t2)) {
   // Serial.println("Failed to read from DHT2 sensor!");
    return;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  hic1 = dht1.computeHeatIndex(t1, h1, false);
  hic2 = dht2.computeHeatIndex(t2, h2, false);
  StartMillis6 = currentMillis;

}
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] payload: ");
char *cstring = (char *) payload;
cstring[length] = '\0';
 // Serial.println(cstring);    
 // Serial.println();
 // char message[5];
  if (strcmp(topic, "Ro/O/mq49/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state0 = LOW;
      client1.publish("Ro/I/mq49/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state0 = HIGH;
      client1.publish("Ro/I/mq49/st","OFF");
   }
 
 }
  if (strcmp(topic, "Ro/O/mq50/cm") == 0 ) {
     if (strcmp(cstring,"ON")==0) {
        state1 = LOW;
      client1.publish("Ro/I/mq50/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state1 = HIGH;
      client1.publish("Ro/I/mq50/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq51/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state2 = LOW;
      client1.publish("Ro/I/mq51/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state2 = HIGH;
      client1.publish("Ro/I/mq51/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq52/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state3 = LOW;
      client1.publish("Ro/I/mq52/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state3 = HIGH;
      client1.publish("Ro/I/mq52/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq53/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state4 = LOW;
      client1.publish("Ro/I/mq53/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state4 = HIGH;
      client1.publish("Ro/I/mq53/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq54/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state5 = LOW;
      client1.publish("Ro/I/mq54/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state5 = HIGH;
      client1.publish("Ro/I/mq54/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq55/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state6 = LOW;
      client1.publish("Ro/I/mq55/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state6 = HIGH;
      client1.publish("Ro/I/mq55/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq56/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state7 = LOW;
      client1.publish("Ro/I/mq56/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state7 = HIGH;
      client1.publish("Ro/I/mq56/st","OFF");
   }
  }
////////////////////////////////////////////////////////////////////////////////////  
  if (strcmp(topic, "Ro/O/mq65/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state8 = HIGH;
      client1.publish("Ro/I/mq65/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state8 = LOW;
      client1.publish("Ro/I/mq65/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq66/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state9 = HIGH;
      client1.publish("Ro/I/mq66/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state9 = LOW;
      client1.publish("Ro/I/mq66/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq67/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state10 = HIGH;
      client1.publish("Ro/I/mq67/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state10 = LOW;
      client1.publish("Ro/I/mq67/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq68/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state11 = HIGH;
      client1.publish("Ro/I/mq68/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state11 = LOW;
      client1.publish("Ro/I/mq68/st","OFF");
   }
  }
///////////////////////////////////////////////////////////////////////////////////////////
  if (strcmp(topic, "Ro/O/mq57/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state12 = LOW;
      client1.publish("Ro/I/mq57/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state12 = HIGH;
      client1.publish("Ro/I/mq57/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq58/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state13 = LOW;
      client1.publish("Ro/I/mq58/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state13 = HIGH;
      client1.publish("Ro/I/mq58/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq59/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state14 = LOW;
      client1.publish("Ro/I/mq59/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state14 = HIGH;
      client1.publish("Ro/I/mq59/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq60/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state15 = LOW;
      client1.publish("Ro/I/mq60/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state15 = HIGH;
      client1.publish("Ro/I/mq60/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq61/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state16 = LOW;
      client1.publish("Ro/I/mq61/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state16 = HIGH;
      client1.publish("Ro/I/mq61/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq62/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state17 = LOW;
      client1.publish("Ro/I/mq62/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state17 = HIGH;
      client1.publish("Ro/I/mq62/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq63/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state18 = LOW;
      client1.publish("Ro/I/mq63/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state18 = HIGH;
      client1.publish("Ro/I/mq63/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq64/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state19 = LOW;
      client1.publish("Ro/I/mq64/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state19 = HIGH;
      client1.publish("Ro/I/mq64/st","OFF");
   }
  }  
  if (strcmp(topic, "Ro/O/mq69/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state20 = HIGH;
      client1.publish("Ro/I/mq69/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state20 = LOW;
      client1.publish("Ro/I/mq69/st","OFF");
   }
  }
  if (strcmp(topic, "Ro/O/mq70/cm") == 0) {
     if (strcmp(cstring,"ON")==0) {
        state21 = HIGH;
        //#out6 = HIGH;
      client1.publish("Ro/I/mq70/st","ON");
    }
   if (strcmp(cstring,"OFF")==0) {
      state21 = LOW;
      //out6 = LOW;
      client1.publish("Ro/I/mq70/st","OFF");
   }
  }    
  Serial.println("");
  //char msgBuffer[30];  
  // check topic to identify type of content
  if (String(topic) == "Ro/O/RRGB/cm") {// convert payload to String
    String value = String((char*)payload);
   // Serial.print (value);
    // split string at every "," and store in proper variable
    // convert final result to integer
    DataR = value.substring(0, value.indexOf(',')).toInt();
    DataG = value.substring(value.indexOf(',') + 1, value.lastIndexOf(',')).toInt();
    DataB = value.substring(value.lastIndexOf(',') + 1).toInt();
   leds[0].setRGB( DataR, DataG, DataB);
   leds[1].setRGB( DataR, DataG, DataB);
   leds[2].setRGB( DataR, DataG, DataB);    
    FastLED.show();
    delay(30);      
  //  while (Serial.available())
   //   Serial.read();

  }
  if (String(topic) == "Ro/O/PARGB/cm") {// convert payload to String
    String value = String((char*)payload);
   // Serial.print (value);
    // split string at every "," and store in proper variable
    // convert final result to integer
    DataR = value.substring(0, value.indexOf(',')).toInt();
    DataG = value.substring(value.indexOf(',') + 1, value.lastIndexOf(',')).toInt();
    DataB = value.substring(value.lastIndexOf(',') + 1).toInt();
   //leds[0].setRGB( DataR, DataG, DataB);
   //leds[1].setRGB( DataR, DataG, DataB);
   leds[2].setRGB( DataR, DataG, DataB);// rgb paal bij raam     
    FastLED.show();
    delay(30);      
 }
  if (String(topic) == "Ro/O/PLRGB/cm") {// convert payload to String
    String value = String((char*)payload);
   // Serial.print (value);
    // split string at every "," and store in proper variable
    // convert final result to integer
    DataR = value.substring(0, value.indexOf(',')).toInt();
    DataG = value.substring(value.indexOf(',') + 1, value.lastIndexOf(',')).toInt();
    DataB = value.substring(value.lastIndexOf(',') + 1).toInt();
   leds[0].setRGB( DataR, DataG, DataB);// rgb op plafond
   //leds[1].setRGB( DataR, DataG, DataB);
   //leds[2].setRGB( DataR, DataG, DataB);    
    FastLED.show();
    delay(30);      
 }
  if (String(topic) == "Ro/O/RARGB/cm") {// convert payload to String
    String value = String((char*)payload);
   // Serial.print (value);
    // split string at every "," and store in proper variable
    // convert final result to integer
    DataR = value.substring(0, value.indexOf(',')).toInt();
    DataG = value.substring(value.indexOf(',') + 1, value.lastIndexOf(',')).toInt();
    DataB = value.substring(value.lastIndexOf(',') + 1).toInt();
   //leds[0].setRGB( DataR, DataG, DataB);
   leds[1].setRGB( DataR, DataG, DataB);// rgb boven raamdeel achter 
   //leds[2].setRGB( DataR, DataG, DataB);    
    FastLED.show();
    delay(30);      
 }      
/*  Serial.println("");
  if (String(topic) == "Ro/O/RPWM/cm") {
    // convert payload to String
    String value = String((char*)payload);
    Serial.print (value);
    // split string at every "," and store in proper variable
    // convert final result to integer
    DataP = value.substring(0, value.indexOf(',')).toInt();
    // print obtained values for debugging
    analogWrite(PWM, DataP);
   // while (Serial.available())
   //   Serial.read();
  }*/
  Serial.println("");
}  
bool mqttReConnect() {
    if (!client1.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client1.connect(clientID)) {
            Serial.println("connected");
            client1.subscribe("Ro/O/#");
       } 
    }
    return client1.connected();
}
void pir(){
pir1 = digitalRead(pirPin1);
    if (pir1 == HIGH && pir1old == LOW && millis() - time > debounce){  //the input from the pir outside at the gate
      flag1 = HIGH;
      time = millis();
      }
    if (pir1 == LOW && pir1old == HIGH && millis() - time > debounce){  //the input from the pir outside at the gate
      flag1 = LOW;
      time = millis();
      }      
if (flag1 == 1 && flag1old == 0){client1.publish("Ro/I/RPir1/st","OPEN");}
if (flag1 == 0 && flag1old == 1 && currentMillis - StartMillis4 >= 15000){client1.publish("Ro/I/RPir1/st","CLOSED");}
pir1old = pir1;
flag1old = flag1;

sensor0 = digitalRead(sensorPin);// button read with state change//update 25-02-2019
if (sensor0 ==  HIGH && sensorold0 == LOW && millis() - time > debounce) {
    alarm0 =  LOW;
    time = millis();
  }
if (sensor0 ==  LOW && sensorold0 == HIGH && millis() - time > debounce) {
    alarm0 =  HIGH;
    time = millis();
  }  
if (alarm0 == 1 && alarmold0 == 0){
    client1.publish("Ro/I/RAl1/st","CLOSED");
}
if (alarm0 == 0 && alarmold0 == 1){
   client1.publish("Ro/I/RAl1/st","OPEN");
}
 sensorold0 = sensor0;//end 
 alarmold0 = alarm0;                  

}
void analoginput(){
// read voltage 12 VOLT output line
  voltInValue = analogRead(voltage);
  voltOutValue = (voltInValue / 1023.0) * 5000; // Gets you mV
  volt12 = voltOutValue * 0.00505;
// read amperage 12V output current
  ampInValue = analogRead(amperage);
  amp1 = ((((double) ampInValue * 5.0 / 1023.0) - 2.5) / 0.066);
// read voltage 5V output line
  voltInValue1 = analogRead(voltage1);
  voltOutValue1 = (voltInValue1 / 1023.0) * 5000; // Gets you mV
  volt5 = voltOutValue1 * 0.00505;
// read amperage 5V output current
   ampInValue1 = analogRead(amperage1);
   amp2 = ((((double) ampInValue1 * 5.0 / 1023.0) - 2.5) / 0.066);
/////////////////END OF ANALOG INPUT////////////////////////////////////
}
void plants(){
   if (currentMillis - StartMillis8 >= 500000){// 15000 is 15 sec. 1500000
       digitalWrite (state12, HIGH);
   if (plant == 1){
      Plant1  = analogRead(plant1);
      Plant2  = analogRead(plant2);
      Plant3  = analogRead(plant3);
      Plant4  = analogRead(plant4);
      Plant5  = analogRead(plant5);
      Plant6  = analogRead(plant6);
      Plant7  = analogRead(plant7);
      Plant8  = analogRead(plant8);
      Plant9  = analogRead(plant9);
      Plant10 = analogRead(plant10);
  }
  if (currentMillis - StartMillis9 >= 25000){
  digitalWrite (state12, LOW);
  }
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//SENSOREN INPUT Room MODULE //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void sensormain() { //PUSH BUTTONS with web buttons ////////////////////////////////////////////////
  read0 = digitalRead(inPin0);// button read with state change
  if (read0 ==  HIGH && previous0 == LOW && millis() - time >= debounce) {
    if (state0 == HIGH)state0 = LOW;
    else state0 =  HIGH;
    time = millis();
  }
  if (state0 == 1 && pre0 == 0) {client1.publish("Ro/I/mq49/st","OFF");}
  if (state0 == 0 && pre0 == 1) {client1.publish("Ro/I/mq49/st","ON"); }  
  if (state0 == HIGH) {digitalWrite(outputAddress[0], state0);}
  if (state0 ==  LOW) {digitalWrite(outputAddress[0], state0);}
  pre0 = state0;
  previous0 = read0;//end
  read1 = digitalRead(inPin1);// button read with state change
  if (read1 ==  HIGH && previous1 == LOW && millis() - time > debounce) {
    if (state1 == HIGH)state1 = LOW;
    else state1 =  HIGH;
    time = millis();
  }
  if (state1 == 1 && pre1 == 0) {
     client1.publish("Ro/I/mq50/st","OFF");
     }
  if (state1 == 0 && pre1 == 1) {
     client1.publish("Ro/I/mq50/st","ON");
     }  
  if (state1 == HIGH) {
    digitalWrite(outputAddress[1], state1);
  }
  if (state1 ==  LOW) {
    digitalWrite(outputAddress[1], state1);
  }
  pre1 = state1;
  previous1 = read1;//end
  read2 = digitalRead(inPin2);// button read with state change
  if (read2 ==  HIGH && previous2 == LOW && millis() - time > debounce) {
    if (state2 == HIGH)state2 = LOW;
    else state2 =  HIGH;
    time = millis();
  }
  if (state2 == 1 && pre2 == 0) {
     client1.publish("Ro/I/mq51/st","OFF");
     }
  if (state2 == 0 && pre2 == 1) {
     client1.publish("Ro/I/mq51/st","ON");
    }  
  if (state2 == HIGH) {
    digitalWrite(outputAddress[2], state2);
  }
  if (state2 ==  LOW) {
    digitalWrite(outputAddress[2], state2);
  }
  pre2 = state2;
  previous2 = read2;
  read3 = digitalRead(inPin3);// button read with state change
  if (read3 ==  HIGH && previous3 == LOW && millis() - time > debounce) {
    if (state3 == HIGH) state3 = LOW;
    else state3 =  HIGH;
    time = millis();
  }
  if (state3 == 1 && pre3 == 0) {
     client1.publish("Ro/I/mq52/st","OFF");
   }
  if (state3 == 0 && pre3 == 1) {
     client1.publish("Ro/I/mq52/st","ON");
   }  
  if (state3 == HIGH) {
    digitalWrite(outputAddress[3], state3);
  }
  if (state3 ==  LOW) {
    digitalWrite(outputAddress[3], state3);
  }
  pre3 = state3;
  previous3 = read3;//end
  read4 = digitalRead(inPin4);// button read with state change
  if (read4 ==  HIGH && previous4 == LOW && millis() - time > debounce) {
    if (state4 == HIGH) state4 = LOW;
    else state4 =  HIGH;
    time = millis();
  }
  if (state4 == 1 && pre4 == 0) {
     client1.publish("Ro/I/mq53/st","OFF");
     }
  if (state4 == 0 && pre4 == 1) {
     client1.publish("Ro/I/mq53/st","ON");
     }  
  if (state4 == HIGH) {
    digitalWrite(outputAddress[4], state4);
  }
  if (state4 ==  LOW) {
    digitalWrite(outputAddress[4], state4);
  }
  pre4 = state4;
  previous4 = read4;//end
  read5 = digitalRead(inPin5);// button read with state change
  if (read5 ==  HIGH && previous5 == LOW && millis() - time > debounce) {
    if (state5 == HIGH) state5 = LOW;
    else state5 =  HIGH;
    time = millis();
  }
  if (state5 == 1 && pre5 == 0) {
     client1.publish("Ro/I/mq54/st","OFF");
    }
  if (state5 == 0 && pre5 == 1) {
     client1.publish("Ro/I/mq54/st","ON");
    }  
  if (state5 == HIGH) {
    digitalWrite(outputAddress[5], state5);
  }
  if (state5 ==  LOW) {
    digitalWrite(outputAddress[5], state5);
  }
  pre5 = state5;
  previous5 = read5;//end
  read6 = digitalRead(inPin6);// button read with state change
  if (read6 ==  HIGH && previous6 == LOW && millis() - time > debounce) {
    if (state6 == HIGH) state6 = LOW;
    else state6 =  HIGH;
    time = millis();
  }
  if (state6 == 1 && pre6 == 0) {
     client1.publish("Ro/I/mq55/st","OFF");
     }
  if (state6 == 0 && pre6 == 1) {
     client1.publish("Ro/I/mq55/st","ON");
     }  
  if (state6 == HIGH) {
    digitalWrite(outputAddress[6], state6);
  }
  if (state6 ==  LOW) {
    digitalWrite(outputAddress[6], state6);
  }
  pre6 = state6;
  previous6 = read6;//end
  read7 = digitalRead(inPin7);// button read with state change
  if (read7 ==  HIGH && previous7 == LOW && millis() - time > debounce) {
    if (state7 == HIGH) state7 = LOW;
    else state7 =  HIGH;
    time = millis();
  }
  if (state7 == 1 && pre7 == 0) {
     client1.publish("Ro/I/mq56/st","OFF");
     }
  if (state7 == 0 && pre7 == 1) {
     client1.publish("Ro/I/mq56/st","ON");
     }  
  if (state7 == HIGH) {
    digitalWrite(outputAddress[7], state7);
  }
  if (state7 ==  LOW) {
    digitalWrite(outputAddress[7], state7);
  }
  pre7 = state7;
  previous7 = read7;//end
////////////////////////////////////////////////////////////////////////////////////////////////////////  
  read8 = digitalRead(inPin8);// button read with state change
  if (read8 ==  HIGH && previous8 == LOW && millis() - time > debounce) {
    if (state8 == HIGH)state8 = LOW;
    else state8 =  HIGH;
    time = millis();
  }
  if (state8 == HIGH) {
    digitalWrite(outputAddress2[0], state8);
  }
  if (state8 ==  LOW) {
    digitalWrite(outputAddress2[0], state8);
  }
//  if (plant == HIGH) {
//    digitalWrite(outputAddress2[0], plant);
//  }
//  if (plant ==  LOW) {
//    digitalWrite(outputAddress2[0], plant);
//  }  
   previous8 = read8;//end  
  if (state8 == 1 && pre8 == 0) {
    client1.publish("Ro/I/mq65/st","ON");
    }
  if (state8 == 0 && pre8 == 1) {
    client1.publish("Ro/I/mq65/st","OFF");
    }  

  pre8 = state8;
 
  read9 = digitalRead(inPin9);// button read with state change
  if (read9 ==  HIGH && previous9 == LOW && millis() - time > debounce) {
    if (state9 == HIGH)state9 = LOW;
    else state9 =  HIGH;
    time = millis();
  }
  if (state9 == HIGH) {
    digitalWrite(outputAddress2[1], state9);
  }
  if (state9 ==  LOW) {
    digitalWrite(outputAddress2[1], state9);
  }
  previous9 = read9;//end
  if (state9 == 1 && pre9 == 0) {
    client1.publish("Ro/I/mq66/st","ON");
    }
  if (state9 == 0 && pre9 == 1) {
    client1.publish("Ro/I/mq66/st","OFF");
    }  

  pre9 = state9;

 read10 = digitalRead(inPin10);// button read with state change
  if (read10 ==  HIGH && previous10 == LOW && millis() - time > debounce) {
    if (state10 == HIGH)state10 = LOW;
    else state10 =  HIGH;
    time = millis();
  }
  if (state10 == 1 && pre10 == 0) {
    client1.publish("Ro/I/mq67/st","ON");
    }
  if (state10 == 0 && pre10 == 1) {
    client1.publish("Ro/I/mq67/st","OFF");
    }  
  if (state10 == HIGH) {
    digitalWrite(outputAddress2[2], state10);
  }
  if (state10 ==  LOW) {
    digitalWrite(outputAddress2[2], state10);
  }
  pre10 = state10;
  previous10 = read10;//end
 read11 = digitalRead(inPin11);// button read with state change
  if (read11 ==  HIGH && previous11 == LOW && millis() - time > debounce) {
    if (state11 == HIGH)state11 = LOW;
    else state11 =  HIGH;
    time = millis();
  }
  if (state11 == 1 && pre11 == 0) {
    client1.publish("Ro/I/mq68/st","ON");
    }
  if (state11 == 0 && pre11 == 1) {
    client1.publish("Ro/I/mq68/st","OFF");
     }  
  if (state11 == HIGH) {
    digitalWrite(outputAddress2[3], state11);
  }
  if (state11 ==  LOW) {
    digitalWrite(outputAddress2[3], state11);
  }
  pre11 = state11;
  previous11 = read11;//end
  /////////////////////////////////////////////////////////////////////////////////////////////
  if (state12 == 1 && pre12 == 0) {//relay 9 
    client1.publish("Ro/I/mq57/st","OFF");
   }
  if (state12 == 0 && pre12 == 1) {
    client1.publish("Ro/I/mq57/st","ON");
   }
  if (state12 == HIGH) {
    digitalWrite(outputAddress1[0], state12);
  }
  if (state12 ==  LOW) {
    digitalWrite(outputAddress1[0], state12);
  }
  pre12 = state12;
  if (state13 == 1 && pre13 == 0) {
    client1.publish("Ro/I/mq58/st","OFF");
    }
  if (state13 == 0 && pre13 == 1) {
    client1.publish("Ro/I/mq58/st","ON");
   }
  if (state13 == HIGH) {
    digitalWrite(outputAddress1[1], state13);
  }
  if (state13 ==  LOW) {
    digitalWrite(outputAddress1[1], state13);
  }
  pre13 = state13;
  if (state14 == 1 && pre14 == 0) {
    client1.publish("Ro/I/mq59/st","OFF");
   }
  if (state14 == 0&& pre14 == 1) {
    client1.publish("Ro/I/mq59/st","ON");
   }
  if (state14 == HIGH) {
    digitalWrite(outputAddress1[2], state14);
  }
  if (state14 ==  LOW) {
    digitalWrite(outputAddress1[2], state14);
  }
  pre14 = state14;
  if (state15 == 1 && pre15 == 0) {
    client1.publish("Ro/I/mq60/st", "OFF");
   }
  if (state15 == 0 && pre15 == 1) {
    client1.publish("Ro/I/mq60/st","ON");
    }
  if (state15 == HIGH) {
    digitalWrite(outputAddress1[3], state15);
  }
  if (state15 ==  LOW) {
    digitalWrite(outputAddress1[3], state15);
  }
  pre15 = state15;
  if (state16 == 1 && pre16 == 0) {
    client1.publish("Ro/I/mq61/st","OFF");
   }
  if (state16 == 0 && pre16 == 1) {
    client1.publish("Ro/I/mq61/st","ON");
   }
  if (state16 == HIGH) {
    digitalWrite(outputAddress1[4], state16);
  }
  if (state16 == LOW) {
    digitalWrite(outputAddress1[4], state16);
  }
  pre16 = state16;
  if (state17 == 1 && pre17 == 0) {
    client1.publish("Ro/I/mq62/st","OFF");
    }
  if (state17 == 0 && pre17 == 1) {
    client1.publish("Ro/I/mq62/st","ON");
   }
  if (state17 == HIGH) {
    digitalWrite(outputAddress1[5], state17);
  }
  if (state17 ==  LOW) {
    digitalWrite(outputAddress1[5], state17);
  }
  pre17 = state17;
  if (state18 == 1 && pre18 == 0) {
    client1.publish("Ro/I/mq63/st","OFF");
    }
  if (state18 == 0 && pre18 == 1) {
    client1.publish("Ro/I/mq63/st","ON");
    }
  if (state18 == HIGH) {
    digitalWrite(outputAddress1[6], state18);
  }
  if (state18 ==  LOW) {
    digitalWrite(outputAddress1[6], state18);
  }
  pre18 = state18;
  if (state19 == 1 && pre19 == 0) {
    client1.publish("Ro/I/mq64/st","OFF");
   }
  if (state19 == 0 && pre19 == 1) {
    client1.publish("Ro/I/mq64/st","ON");
  }
  if (state19 == HIGH) {
    digitalWrite(outputAddress1[7], state19);
  }
  if (state19 ==  LOW) {
    digitalWrite(outputAddress1[7], state19);
  }
  pre19 = state19;    
  if (state20 == 1 && pre24 == 0) {
    client1.publish("Ro/I/mq69/st","ON");
    }
  if (state20 == 0 && pre24 == 1) {
    client1.publish("Ro/I/mq69/st","OFF");
    }
  if (state20 == HIGH) {
    digitalWrite(out5, HIGH);
  }
  if (state20 ==  LOW) {
    digitalWrite(out5, LOW);
  }
  pre24 = state20;
  if (state21 == 1 && pre25 == 0) {
    client1.publish("Ro/I/mq70/st","ON");
   }
  if (state21 == 0 && pre25 == 1) {
    client1.publish("Ro/I/mq70/st","OFF");
  }
  if (state21 == HIGH) {
    digitalWrite(out6, HIGH);
  }
  if (state21 ==  LOW) {
    digitalWrite(out6, LOW);
  }
  pre25 = state21;      
}  
// END OF SWITCH PART
/////////////////////////////////////////////////////////////////////////////////////////////////////
// PIR MOTION 2 STATE SENSOR MODULE TRAP ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*void motion(){ // read the state of the PIR sensors:
  topSensorState = digitalRead(TOP_SENSOR);// PIR ON TOP
  bottomSensorState = digitalRead(BOTTOM_SENSOR);// PIR ON BOTTOM
  // check if either of the PIR sensors are active.
  if ((topSensorState == ACTIVE) or (bottomSensorState == ACTIVE)){
  // digitalWrite(outputAddress[7], HIGH);// aanpassen
  }
  else{
  // digitalWrite(outputAddress[7], LOW);// aanpassen
  }
  }*/

void motor() {// MOTOR MAIN 2 MOTOR CONTROL !!!
  motor0 = digitalRead(motorPin1);// read input value and store it in val
  delay(10);                      // 10 milliseconds is a good amount of time
  motor1 = digitalRead(motorPin1);// read the input again to check for bounces
  if (motor0 == motor1) {         // make sure we got 2 consistant readings!
    if (motor0 != motorState) {   // the button for motor state has changed!
      if (motor0 == LOW) {        // check if the button for motor is pressed
        if (motorMode == 0) {     // motor off
          motorMode = 1;        // motor to go up
        }
        else {
          if (motorMode == 1) {         // motor to go up
            motorMode = 2;            // motor off
          }
          else {
            if (motorMode == 2) {         // motor off
              motorMode = 3;            // motor to go down
            }
            else {
              if (motorMode == 3) {         // motor to go down
                motorMode = 0;            // motor off
              }
            }
          }
        }
      }
    }////////////////////////////////////////////////
    motorState = motor0;            // save the new state in our variable
  }         // Now do whatever the MotorMode indicates
  if (motorMode == 0) {           // motor stop
    digitalWrite(out1, LOW);
    digitalWrite(out2, LOW);
  }
  if (motorMode == 1) {           // motor up
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
  }
  if (motorMode == 2) {           // motor stop
    digitalWrite(out1, LOW);
    digitalWrite(out2, LOW);
  }
  if (motorMode == 3)  {          // motor down
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);
  } // END OF MOTOR ///////////////////////////////////////////////////////////////////
  motor2 = digitalRead(motorPin2);// read input value and store it in val
  delay(10);                      // 10 milliseconds is a good amount of time
  motor3 = digitalRead(motorPin2);// read the input again to check for bounces
  if (motor2 == motor3) {         // make sure we got 2 consistant readings!
    if (motor2 != motorState1) {  // the button for motor state has changed!
      if (motor2 == LOW) {        // check if the button for motor is pressed
        if (motorMode1 == 0) {    // motor off
          motorMode1 = 1;       // motor to go up
        } else {
          if (motorMode1 == 1) {        // motor to go up
            motorMode1 = 2;           // motor off
          } else {
            if (motorMode1 == 2) {        // motor off
              motorMode1 = 3;           // motor to go down
            } else {
              if (motorMode1 == 3) {        // motor to go down
                motorMode1 = 0;           // motor off
              }
            }
          }
        }
      }
    }// /////////////////////////////////////////////////
    motorState1 = motor2;// save the new state in our variable
  }         // Now do whatever the motorMode indicates
  if (motorMode1 == 0) {          // motor stop
    digitalWrite(out3, LOW);
    digitalWrite(out4, LOW);
  }
  if (motorMode1 == 1) {          // motor up
    digitalWrite(out3, HIGH);
    digitalWrite(out4, LOW);
  }
  if (motorMode1 == 2) {          // motor stop
    digitalWrite(out3, LOW);
    digitalWrite(out4, LOW);
  }
  if (motorMode1 == 3)  {         // motor down
    digitalWrite(out3, LOW);
    digitalWrite(out4, HIGH);
  }
} 
void checkForClient() {
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;
     while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '*') {
          printHtmlHeader(client); //call for html header and css
          printHtmlFooter(client);
          break;
        }
        if (!sentHeader) {
          printHtmlHeader(client); //call for html header and css
          printHtmlButtonTitle(client); //print the button title
          sentHeader = true;
        }
        if (reading && c == ' ') {
          reading = false;
        }
        if (c == '?') {
          reading = true; //found the ?, begin reading the info
        }
        if (reading) {
          //if user input is H set output to 1
          if (c == 'H') {
            outp = 1;
          }
          if (c == 'L') {
            outp = 0;
          }
          //---------------------------------------------------------------------------------------------
          // ? H 1 0
          // ^ ^ ^ ^
          // | | | |____________read 4 ( 10,11,12,13....)
          // | | |______________read 3 ( 1....9)
          // | |________________read 2 if user input is H set output to L
          // |__________________read 1
          //---------------------------------------------------------------------------------------------

          if ( c == 'x') {
            char c = client.read();
            switch (c) {
              case '0':
                triggerPin(outputAddress[0], client, outp);
                state0 = outp;
                break;
              case '1':
                triggerPin(outputAddress[1], client, outp);
                state1 = outp;
                break;
              case '2':
                triggerPin(outputAddress[2], client, outp);
               state2 = outp;
                break;
              case '3':
                triggerPin(outputAddress[3], client, outp);
                state3 = outp;
                break;
              case '4':
                triggerPin(outputAddress[4], client, outp);
                state4 = outp;
                break;
              case '5':
                triggerPin(outputAddress[5], client, outp);
                state5 = outp;
                break;
              case '6':
                triggerPin(outputAddress[6], client, outp);
                state6 = outp;
                break;
              case '7':
                triggerPin(outputAddress[7], client, outp);
                state7 = outp;
                break;
            }
          }          
          if ( c == 'y') {
            char c = client.read();
            switch (c) {
              case '0':
                triggerPin(outputAddress1[0], client, outp);
                state12 = outp;
                break;
              case '1':
                triggerPin(outputAddress1[1], client, outp);
                state13 = outp;
                break;
              case '2':
                triggerPin(outputAddress1[2], client, outp);
                state14 = outp;
                break;
              case '3':
                triggerPin(outputAddress1[3], client, outp);
                state15 = outp;
                break;
              case '4':
                triggerPin(outputAddress1[4], client, outp);
                state16 = outp;
                break;
              case '5':
                triggerPin(outputAddress1[5], client, outp);
               state17 = outp;
                break;
              case '6':
                triggerPin(outputAddress1[6], client, outp);
                state18 = outp;
                break;
              case '7':
                triggerPin(outputAddress1[7], client, outp);
                state19 = outp;
                break;
            }
          }
           if ( c == 'z') {
            char c = client.read();
            switch (c) {
              case '0':
                triggerPin(outputAddress2[0], client, outp);
                state8 = outp;
                break;
              case '1':
                triggerPin(outputAddress2[1], client, outp);
                state9 = outp;
                break;
              case '2':
                triggerPin(outputAddress2[2], client, outp);
                state10 = outp;
                break;
              case '3':
                triggerPin(outputAddress2[3], client, outp);
                state11 = outp;
                break;
            }
          }         
          else {
            switch (c) {
              // extra case for motor control /////////////////////////////////////////////////////////////
              case 'A':// MOTOR 1 UP
                motorMode = 1;
                break;
              case 'B':// MOTOR 1 STOP
                motorMode = 0;
                break;
              case 'C':// MOTOR 1 DOWN
                motorMode = 3;
                break;
              case 'D'://MOTOR 2 UP
                motorMode1 = 1;
                break;
              case 'E':// MOTOR 2 STOP
                motorMode1 = 0;
                break;
              case 'F'://MOTOR 2 DOWN
                motorMode1 = 3;
                break;
                //////              end of control

            } //end of switch case
          }
        }//end of switch switch the relevant output
        //if user input was blank
        if (c == '\n' && currentLineIsBlank) {
          printLastCommandOnce = true;
          printButtonMenuOnce = true;
          //Call to read input and print menu. 777 is used not to update any outputs
          triggerPin(777, client, outp);
          break;
        }
      }
    }
    printHtmlFooter(client); //Prints the html footer
  }
  else {
    if (millis() > (timeConnectedAt + 60000)) {
      if (writeTo == true) {
        Serial.println("No Clients for more then a minute.");
        writeTo = false;
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////
//triggerPin Function Room MODULE /////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void triggerPin(int pin, EthernetClient client, int outp) {
  if (pin != 777) {
    if (outp == 1) {
      if (outputInverted == false) {
        digitalWrite(pin, HIGH);
      }
      else {
        digitalWrite(pin, LOW);
      }
    }
    if (outp == 0) {
      if (outputInverted == false) {
        digitalWrite(pin, LOW);
      }
      else {
        digitalWrite(pin, HIGH);
      }
    }
     if (outp == 1) {
      if (outputInverted1 == false) {
        digitalWrite(pin, HIGH);
      }
      else {
        digitalWrite(pin, LOW);
      }
    }
    if (outp == 0) {
      if (outputInverted1 == false) {
        digitalWrite(pin, LOW);
      }
      else {
        digitalWrite(pin, HIGH);
      }
    } 
     if (outp == 1) {
      if (outputInverted2 == false) {
        digitalWrite(pin, HIGH);
      }
      else {
        digitalWrite(pin, LOW);
      }
    }
    if (outp == 0) {
      if (outputInverted2 == false) {
        digitalWrite(pin, LOW);
      }
      else {
        digitalWrite(pin, HIGH);
      }
    }     
  }
  //Refresh the reading of outputs
  readOutputStatuses();
  //Prints the buttons
  if (printButtonMenuOnce == true) {
    printHtmlButtons(client);
    printButtonMenuOnce = false;
  }
}
////////////////////////////////////////////////////////////////////////
//printHtmlButtons Function Room MODULE ///////////////////////////////
////////////////////////////////////////////////////////////////////////
//print the html buttons to switch on/off channels
void printHtmlButtons(EthernetClient client) {
  //Start to create the html table
  client.println("");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">");

  //Start printing button by button
  for (int var = 0; var < outputQuantity; var++) {
    //Print begining of row
    client.print("<tr>\n");
    //Prints the button Text
    client.print("<td>");
    client.print(buttonText[var]);
    client.print("</td>\n");
    //Prints the ON
    client.print("<td>");
    client.print("<INPUT TYPE=\"button\" VALUE=\"ON ");
    client.print("\" onClick=\"parent.location='/?Lx");
    client.print(var);
    client.print("'\"></td>\n");
    //Prints the OFF
    client.print(" <td><INPUT TYPE=\"button\" VALUE=\"OFF");
    client.print("\" onClick=\"parent.location='/?Hx");
    client.print(var);
    client.print("'\"></td>\n");
    //Invert the LED display if output is inverted.
    if (outputStatus[var] == false ) { //If Output is ON
        client.print(" <td><div class='green-circle'></div></td>\n"); //Print html for ON LED
      }
    client.print("</tr>\n");
  }
//Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
//////////////////////////////////////////////////////////////////////////////////////////////////  
   client.println("");
  //client.println("<p>");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">"); 
  //Start printing button by button
  for (int var1 = 0; var1 < outputQuantity1; var1++) {
    //Print begining of row
    client.print("<tr>\n");
    //Prints the button Text
    client.print("<td>");
    client.print(buttonText1[var1]);
    client.print("</td>\n");
    //Prints the ON Buttons
    client.print("<td>");
    client.print("<INPUT TYPE=\"button\" VALUE=\"ON ");
    client.print("\" onClick=\"parent.location='/?Ly");
    client.print(var1);
    client.print("'\"></td>\n");
    //Prints the OFF Buttons
    client.print(" <td><INPUT TYPE=\"button\" VALUE=\"OFF");
    client.print("\" onClick=\"parent.location='/?Hy");
    client.print(var1);
    client.print("'\"></td>\n");
    //Invert the LED display if output is inverted.
  if (outputStatus1[var1] == false ) { //If Output is ON
        client.print(" <td><div class='green-circle'></div></td>\n"); //Print html for ON LED
      }   
    client.print("</tr>\n");
  }
//Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
/////////////////////////////////////////////////////////////////////////////////////////////////
  client.println("");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">"); 
  //Start printing button by button
  for (int var2 = 0; var2 < outputQuantity2; var2++) {
    //Print begining of row
    client.print("<tr>\n");
    //Prints the button Text
    client.print("<td>");
    client.print(buttonText2[var2]);
    client.print("</td>\n");
    //Prints the ON Buttons
    client.print("<td>");
    client.print("<INPUT TYPE=\"button\" VALUE=\"ON ");
    client.print("\" onClick=\"parent.location='/?Hz");
    client.print(var2);
    client.print("'\"></td>\n");
    //Prints the OFF Buttons
    client.print(" <td><INPUT TYPE=\"button\" VALUE=\"OFF");
    client.print("\" onClick=\"parent.location='/?Lz");
    client.print(var2);
    client.print("'\"></td>\n");
    //Invert the LED display if output is inverted.
  if (outputStatus2[var2] == true ) { //If Output is ON
        client.print(" <td><div class='green-circle'></div></td>\n"); //Print html for ON LED
      }
    client.print("</tr>\n");
  }
//Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
//**********************************************************************
  client.println("");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">");
  client.print("</tr>\n"); 
  // web button for motor control 1 /////////////////////////////////////////////
  client.print("<tr><td>");
  client.print("motor 1");// name to display on web page ////////////////////////
  client.print("</td>\n");
  //Prints the ON Button motor 1 ////////////////////////////////////////////////
  client.print("<td>");
  client.print("<INPUT TYPE=\"button\" VALUE=\"UP ");// prints the UP button ////
  client.print("\" onClick=\"parent.location='/?MA");// sends the code /?MA
  client.print("'\"></td>\n");
  //Prints the ON Button motor 1 /////////////////////////////////////////////////
  client.print("<td>");
  client.print("<INPUT TYPE=\"button\" VALUE=\"STOP ");// prints the STOP button
  client.print("\" onClick=\"parent.location='/?MB");// sends the code /?MB
  client.print("'\"></td>\n");
  //Prints the OFF Button motor 1 ///////////////////////////////////////////////
  client.print("<td>");
  client.print("<INPUT TYPE=\"button\" VALUE=\"DOWN");// prints the DOWN button
  client.print("\" onClick=\"parent.location='/?MC");// sends the code /?MC
  client.print("'\"></td>\n");
  // web button motor 2 /////////////////////////////////////////////////////////
  client.print("<tr><td>");
  client.print("motor 2");// name to display on web page ////////////////////////
  client.print("</td>\n");
  //Prints the ON Button motor 2 /////////////////////////////////////////////////
  client.print("<td>");
  client.print("<INPUT TYPE=\"button\" VALUE=\"UP ");// prints the UP button
  client.print("\" onClick=\"parent.location='/?MD");// sends the code /?MD
  client.print("'\"></td>\n");
  //Prints the ON Button motor 2 /////////////////////////////////////////////////
  client.print("<td>");
  client.print("<INPUT TYPE=\"button\" VALUE=\"STOP ");// prints the STOP button
  client.print("\" onClick=\"parent.location='/?ME");// sends the code /?ME
  client.print("'\"></td>\n");
  //Prints the OFF Button motor 2 ///////////////////////////////////////////////
  client.print("<td><INPUT TYPE=\"button\" VALUE=\"DOWN");// prints the DOWN button
  client.print("\" onClick=\"parent.location='/?MF");// sends the code /?MF
  client.print("'\"></td>\n");
//******************************************************************************
  client.println("");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">");
  //Printing the Temperature/////////////////////////////////////////////////////
  client.print("<tr>\n");
  client.print("<td>");
  client.print("TEMP. KAMER");
  client.print("</td>\n");
  client.print("<td>");
  client.print(t1);
  client.print("C</td>\n");
  client.print("<td>");
  client.print(h1);
  client.print("%</td>\n");
  client.print("<td></td>");
  client.print("<tr>\n");
  client.print("<td>");
  client.print("TEMP. Hal");
  client.print("</td>\n");
  client.print("<td>");
  client.print(t2);
  client.print("C</td>\n");
  client.print("<td>");
  client.print(h2);
  client.print("%</td>\n");
  client.print("<td></td>");
  client.print("</tr>\n");
  //Printing the voltage/////////////////////////////////////////////////////
  client.print("<tr>\n");
  client.print("<td>");
  client.print("volt12");
  client.print("</td>\n");
  client.print("<td>");
  client.print(volt12);
  client.print("V</td>\n");
  client.print("<td>");
  client.print(amp1);
  client.print("A</td>\n");
  client.print("<td></td>");
  client.print("<tr>\n");
  client.print("<td>");
  client.print("volt5");
  client.print("</td>\n");
  client.print("<td>");
  client.print(volt5);
  client.print("V</td>\n");
  client.print("<td>");
  client.print(amp2);
  client.print("A</td>\n");
  client.print("<td></td>");
  client.print("</tr>\n");
//Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
///*****************************************************************
  client.println("");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"left\">");
  
  client.print("<tr>\n");
  client.print("<td>");
  client.print("Planten");
  client.print("</td>\n");
  client.print("<td>");
  client.print(Plant1);
  client.print("%</td>\n");
  client.print("<td>");
  client.print(Plant2);
  client.print("%</td>\n");
  client.print("<tr>\n");
  client.print("<td>");
  client.print("Planten");
  client.print("</td>\n");
  client.print("<td>");
  client.print(Plant3);
  client.print("%</td>\n");
  client.print("<td>");
  client.print(Plant4);
  client.print("%</td>\n");  
  client.print("<td></td>");
  client.print("<tr>\n");
  client.print("<td>");
  client.print("Planten");
  client.print("</td>\n");
  client.print("<td>");
  client.print(Plant5);
  client.print("%</td>\n");
  client.print("<td>");
  client.print(Plant6);
  client.print("%</td>\n");
  client.print("<tr>\n");
  client.print("<td>");
  client.print("Planten");
  client.print("</td>\n");  
  client.print("<td>");
  client.print(Plant7);
  client.print("%</td>\n");
  client.print("<td>");
  client.print(Plant8);
  client.print("%</td>\n");  
  client.print("<td></td>");
  client.print("</tr>\n");
  client.print("<td>");
  client.print("Planten");
  client.print("</td>\n");
  client.print("<td>");
  client.print(Plant9);
  client.print("%</td>\n");
  client.print("<td>");
  client.print(Plant10);
  client.print("%</td>\n");  
  client.print("<td></td>");
  client.print("</tr>\n");
//Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
}
////////////////////////////////////////////////////////////////////////
//readOutputStatuses Function Room MODULE /////////////////////////////
////////////////////////////////////////////////////////////////////////
//Reading the Output Statuses
void readOutputStatuses() {
  for (int var = 0; var < outputQuantity; var++) {
    outputStatus[var] = digitalRead(outputAddress[var]);
  }
   for (int var1 = 0; var1 < outputQuantity1; var1++) {
     outputStatus1[var1] = digitalRead(outputAddress1[var1]);
  //Serial.print(outputStatus[var]);
    }
    for (int var2 = 0; var2 < outputQuantity2; var2++) {
      outputStatus2[var2] = digitalRead(outputAddress2[var2]);
  //Serial.print(outputStatus[var]);
   }
}

////////////////////////////////////////////////////////////////////////
//htmlHeader Function Room MODULE /////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Prints html header
void printHtmlHeader(EthernetClient client) {
  // Serial.print("Serving html Headers at ms -");
  timeConnectedAt = millis(); //Record the time when last page was served.
  writeTo = true;
  // Serial.print(timeConnectedAt); // Print time for debbugging purposes
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<head>");
  // add page title
  client.println("<title>Room Switching</title>");
  client.println("<meta name=\"description\" content=\"Room Switching\"/>");
  // add a meta refresh tag, so the browser pulls again every x seconds:
  client.print("<meta http-equiv=\"refresh\" content=\"");
  client.print(refreshPage);
  client.println("; url=/\">");
  // add other browser configuration
  client.println("<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">");
  client.println("<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">");
  //inserting the styles data, usually found in CSS files.
  client.println("<style type=\"text/css\">");
  client.println("");
  //This will set how the page will look graphically
  client.println("html { height:100%; }");
  client.println(" body { height: 100%; margin: 0; font-family: helvetica, sans-serif;");
  client.println(" -webkit-text-size-adjust: none;}");
  client.println("");
  client.println("body {");
  client.println(" -webkit-background-size: 100% 21px;");
  client.println(" background-color: #00FFFF;");//achtergrond kleur web pagina veranderen
  client.println(" background-image:");
  client.println(" -webkit-gradient(linear, left top, right top,");
  client.println(" color-stop(.75, transparent),");
  client.println(" color-stop(.75, rgba(255,255,255,.1)) );");
  client.println(" -webkit-background-size: 7px; }");
  client.println("");
  client.println(".view { min-height: 100%; overflow: auto;}");
  client.println("");
  client.println(".header-wrapper {");
  client.println(" height: 44px;");
  client.println(" font-weight: bold;");
  client.println(" text-shadow: rgba(0,0,0,0.7) 0 -1px 0;");
  client.println(" border-top: solid 1px rgba(255,255,255,0.6);");
  client.println(" border-bottom: solid 1px rgba(0,0,0,0.6);");
  client.println(" color: #fff;");//wit kleurcode
  client.println(" background-color: #8195af;");//achtergrondkleur web titelbalk veranderen
  client.println(" background-image:");
  client.println(" -webkit-gradient(linear, left top, left bottom,");
  client.println(" from(rgba(255,255,255,.4)),");
  client.println(" to(rgba(255,255,255,.05)) ),");
  client.println(" -webkit-gradient(linear, left top, left bottom,");
  client.println(" from(transparent),");
  client.println(" to(rgba(0,0,64,.1)) );");
  client.println(" background-repeat: no-repeat;");
  client.println(" background-position: top left, bottom left; }");
  client.println("");
  client.println(".header-wrapper h1 { text-align: center; font-size: 20px; line-height: 44px; margin: 0; }");
  client.println("");
  client.println(".group-wrapper { margin: 9px; }");
  client.println("");

  client.println(".group-wrapper table { background-color: #fff; -webkit-border-radius: 10px;-khtml-border-radius: 10px;");
  client.println(" font-size: 17px; line-height: 20px; margin: 9px 0 20px; border: solid 1px #a9abae;");
  client.println(" padding: 11px 3px 12px 3px; margin-left:auto; margin-right:auto; }");
  client.println("");
    //how the green (ON) LED will look
  client.println(".green-circle { display: block; height: 23px;width: 23px;background-color: #0f0;border-radius: 11px;}");
  client.println("");
  //and finally this is the end of the style data and header
  client.println("</style></head>");
  //now printing the page itself
  client.println("<body><div class=\"view\"> <div class=\"header-wrapper\"><h1>ROOM Switching</h1></div>");

} //end of htmlHeader
////////////////////////////////////////////////////////////////////////
//htmlFooter Function  Room MODULE ////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Prints html footer
void printHtmlFooter(EthernetClient client) {
  //Set Variables Before Exiting
  printLastCommandOnce = false;
  printButtonMenuOnce = false;//printing last part of the html
  client.println("</div>\n</div>\n</body>\n</html>");
  delay(1); // give the web browser time to receive the data
  client.stop(); // close the connection:
  Serial.println(" - Done, Closing Connection.");
  delay (2); //delay so that it will give time for client buffer to clear and does not repeat multiple pages.
} //end of htmlFooter
////////////////////////////////////////////////////////////////////////
//printHtmlButtonTitle Function Room MODULE ///////////////////////////
////////////////////////////////////////////////////////////////////////
//Prints html button title
void printHtmlButtonTitle(EthernetClient client) {
  client.println("<div class=\"group-wrapper\"> Room MODULE.");
  client.println();
}
