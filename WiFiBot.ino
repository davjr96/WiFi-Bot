#include <MyTimer.h>
#include <RCSensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RIGHTSENSOR 16
#define LEFTSENSOR 14

#define PWMA 4
#define PWMB 5

#define AIN1 12
#define AIN2 13

#define BIN1 15
#define BIN2 2

#define FORWARD  1
#define BACKWARD  0
#define RIGHT  'R'
#define LEFT 'L'
#define STOP 'S'
#define UTURN 'U'
#define STRAIGHT  'F'

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "wahoo";

int pwm = 500;
int sumError = 0;
int prevError = 0;

int turnDirection;

RCSensor right(RIGHTSENSOR);
RCSensor left(LEFTSENSOR);
int rightMax = 0, leftMax = 0;

String directions = "";
int command = 0;

MyTimer printTimer;
MyTimer forwardTimer;

void setup(){
        Serial.begin(115200);
        delay(500);
        connectWiFi();
        pinMode(AIN1, OUTPUT);
        pinMode(AIN2, OUTPUT);
        pinMode(BIN1, OUTPUT);
        pinMode(BIN2, OUTPUT);
        pinMode(PWMA, OUTPUT);
        pinMode(PWMB, OUTPUT);

        printTimer.startTimer(1000);

        calibrate();
        updateDirections();
}

void connectWiFi()
{
        WiFi.mode(WIFI_STA);
        WiFi.begin(WiFiSSID);
        while (WiFi.status() != WL_CONNECTED)
        {
                delay(100);
        }
        Serial.println("");
        Serial.println("WiFi connected");  
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Netmask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());

}

void loop(){
        if(printTimer.checkExpired())
        {
                Serial.print(left.read());
                Serial.print("  ");
                Serial.print(right.read());
                Serial.print("  ");
    
                Serial.println(pwm);
                printTimer.startTimer(1000);
        }
        if (right.read() > rightMax - 300 && left.read() > leftMax - 300)
        {
                Serial.println("INTERSECTION");
                turnDirection = directions.charAt(command);
                Serial.println(directions);
                
                if (turnDirection == RIGHT)
                        turnRight();
                else if (turnDirection == LEFT)
                        turnLeft();
                else if(turnDirection == STOP)
                {
                      halt();
                      updateDirections();
                        
                }
                else if(turnDirection == STRAIGHT){
                      Serial.println("Forward");
                      leftMotor(FORWARD, pwm);
                      rightMotor(FORWARD, pwm);
                      delay(400);
                }
                                command++;

        }

        if (left.read() > leftMax - 300) //Adjust Left
        {
                rightMotor(FORWARD, pwm);
                leftMotor(FORWARD, pwm - .90*pwm);
        }
        else if (right.read() > rightMax - 300) //Adjust Right
        {
                rightMotor(FORWARD, pwm - .90*pwm);
                leftMotor(FORWARD, pwm);

        }
        else
        {
                rightMotor(FORWARD, pwm);
                leftMotor(FORWARD, pwm);
        }


}

void turnLeft()
{
        Serial.println("Turning Left");
        rightMotor(FORWARD, pwm);
        leftMotor(BACKWARD, .15 * pwm);
        while(right.read() > rightMax - 950)
        {              yield();
}
        while(right.read() < rightMax - 950)
        {              yield();
}
        while(right.read() > rightMax - 950)
        {              yield();
}
        while(right.read() < rightMax - 700)
        {              yield();
}
}
void turnRight()
{
        Serial.println("Turning Right");
        rightMotor(BACKWARD, .15 * pwm);
        leftMotor(FORWARD, pwm);
        while(left.read() > leftMax - 950)
        {              yield();
}
        while(left.read() < leftMax - 950)
        {              yield();
}
        while(left.read() > leftMax - 950)
        {              yield();
}
        while(left.read() < leftMax - 700)
        {              yield();
}
}
void halt()
{
                digitalWrite(AIN1, LOW);
                digitalWrite(AIN2, LOW);
                analogWrite(PWMA, 0);
                digitalWrite(BIN1, LOW);
                digitalWrite(BIN2, LOW);
                analogWrite(PWMB, 0);
}
void rightMotor(int rotate, int rate)
{
        if(rotate == 1)
        {
                digitalWrite(AIN1, HIGH);
                digitalWrite(AIN2, LOW);
                analogWrite(PWMA, rate);
        }
        else
        {
                digitalWrite(AIN1, LOW);
                digitalWrite(AIN2, HIGH);
                analogWrite(PWMA, rate);
        }
}
void leftMotor(int rotate, int rate)
{
        if(rotate == 1)
        {
                digitalWrite(BIN1, HIGH);
                digitalWrite(BIN2, LOW);
                analogWrite(PWMB, rate);
        }
        else
        {
                digitalWrite(BIN1, LOW);
                digitalWrite(BIN2, HIGH);
                analogWrite(PWMB, rate);
        }
}
void calibrate()
{
        MyTimer timer;
        timer.startTimer(5000);
        while(!timer.checkExpired())
        {
              yield();
                int reading = right.read();
                if (reading > rightMax)
                        rightMax = reading;
                reading = left.read();
                if (reading > leftMax)
                        leftMax = reading;
        }
        Serial.println(leftMax);
        Serial.println(rightMax);
}

void updateDirections()
{
  
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("http://216.197.78.205/id?id=1"); //HTTP
        String previousDirections = directions;
        while(previousDirections == directions)
        {
          yield();
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                directions = http.getString();
                directions = directions.substring(directions.indexOf(':') + 1);

            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        }
        http.end();
    
  command = 0;
  turnDirection = directions.charAt(command);
                
                if (turnDirection == RIGHT)
                        turnRight();
                else if (turnDirection == LEFT)
                        turnLeft();
                else if (turnDirection == UTURN){
                      leftMotor(BACKWARD, pwm);
                      rightMotor(FORWARD, pwm);
                      delay(1600);
                }
                else if(turnDirection == STRAIGHT){
                      Serial.println("Forward");
                      leftMotor(FORWARD, pwm);
                      rightMotor(FORWARD, pwm);
                      delay(400);
                }
                                command++;

                
  Serial.println(directions);
}

