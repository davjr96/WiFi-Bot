#include <MyTimer.h>
#include <RCSensor.h>
#include <ESP8266WiFi.h>

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
#define UTUNR 'U'

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

String directions = "RLRS";
int command = 0;

MyTimer turningTimer;
MyTimer printTimer;

bool turned = false;

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
        if (turned && !turningTimer.checkExpired())
        {

                turned = false;
        }

        if (right.read() > rightMax - 550 && left.read() > leftMax - 550)
        {
                turnDirection = directions.charAt(command);
                command++;
                
                if (turnDirection == RIGHT)
                        turnRight();
                else if (turnDirection == LEFT)
                        turnLeft();
                else if(turnDirection == STOP)
                        updateDirections();
                turned = true;
                turningTimer.startTimer(1000);
                Serial.println("INTERSECTION");
        }

        if (left.read() > leftMax - 350) //Adjust Left
        {
                rightMotor(FORWARD, pwm);
                leftMotor(FORWARD, pwm - .90*pwm);
        }
        else if (right.read() > rightMax - 350) //Adjust Right
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
        while(right.read() > rightMax - 750)
        {              yield();
}
        while(right.read() < rightMax - 750)
        {              yield();
}
        while(right.read() > rightMax - 750)
        {              yield();
}
        while(right.read() < rightMax - 500)
        {              yield();
}
}
void turnRight()
{
        Serial.println("Turning Right");
        rightMotor(BACKWARD, .15 * pwm);
        leftMotor(FORWARD, pwm);
        while(left.read() > leftMax - 750)
        {              yield();
}
        while(left.read() < leftMax - 750)
        {              yield();
}
        while(left.read() > leftMax - 750)
        {              yield();
}
        while(left.read() < leftMax - 500)
        {              yield();
}
}
void rightMotor(int direction, int speed)
{

        if(direction == 1)
        {
                digitalWrite(AIN1, HIGH);
                digitalWrite(AIN2, LOW);
                analogWrite(PWMA, speed);
        }
        else
        {
                digitalWrite(AIN1, LOW);
                digitalWrite(AIN2, HIGH);
                analogWrite(PWMA, speed);
        }
}
void leftMotor(int direction, int speed)
{

        if(direction == 1)
        {
                digitalWrite(BIN1, HIGH);
                digitalWrite(BIN2, LOW);
                analogWrite(PWMB, speed);
        }
        else
        {
                digitalWrite(BIN1, LOW);
                digitalWrite(BIN2, HIGH);
                analogWrite(PWMB, speed);
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
  directions = "RLRS";
  command = 0;

}

