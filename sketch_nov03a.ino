
//------Global Variables-----
byte Buffer[20];
byte Buffer_len = 0;
unsigned char flag = false;

int carSpeed=0;
int batteryVoltage=0;
int parkingSensor=0;
int carLock=0;

// -----TYPED Defines-----
typedef enum {
  // inputs for dashboard
  DASHBOARD_CAR_SPEED_VP = 0x1000,
  DASHBOARD_BATTERY_VOLTAGE_VP = 0x1100,
  DASHBOARD_DOOR_LOCK_VP = 0x1200,
  DASHBOARD_PARKING_VP = 0x1300,

  // outputs from dashboard
  DASHBOARD_FRONT_LIGHTS_VP = 0x1400,
  DASHBOARD_TURNING_FLASHERS_R_VP = 0x1500,
  DASHBOARD_TURNING_FLASHERS_L_VP = 0x1600,
  DASHBOARD_CAR_HORN_VP = 0x1700

} VP_Location;


// PINS

#define EVER_FRONT_LIGHT 3
#define EVER_HORN 4
#define EVER_RIGHT_FLASHER 5
#define EVER_LEFT_FLASHER 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(EVER_FRONT_LIGHT, OUTPUT);
  pinMode(EVER_HORN, OUTPUT);
  pinMode(EVER_RIGHT_FLASHER, OUTPUT);
  pinMode(EVER_LEFT_FLASHER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Send speed of the car to the dashboard
  DISPLAY_sendInt(carSpeed,DASHBOARD_CAR_SPEED_VP);
  //Send battery voltage to the dashboard
  DISPLAY_sendInt(batteryVoltage,DASHBOARD_BATTERY_VOLTAGE_VP);
  //Send door lock status to the dashboard
  DISPLAY_sendInt(carLock,DASHBOARD_DOOR_LOCK_VP);
  //Send parking sensor status to the dashboard
  DISPLAY_sendInt(parkingSensor,DASHBOARD_PARKING_VP);

  DISPLAY_processInputs();
}

// ----FUNCTIONS-------


void DISPLAY_sendInt(int data, VP_Location location) {
  /*5A A5: Frame header
   05: Data length
  82: Write VPS
  1000 : RAM address (2bytes)
  0002: Data(2bytes)*/
  Serial.write(0x5A);
  Serial.write(0xA5);
  Serial.write(0x05);
  Serial.write(0xA5);
  Serial.write(highByte(location));
  Serial.write(lowByte(location));
  Serial.write(highByte(data));
  Serial.write(lowByte(data));
}

void DISPLAY_processInputs() {
  // check that there is available communication and store the recieved command
  if (Serial.available()) {
    Buffer[Buffer_len] = Serial.read();
    Buffer_len++;
    flag = true;
  }

  else {
    // check for the recieved frame
    /*
    5A A5: Frame header
    04: Data length
    83: Read VPS
    1001: RAM address(2 bytes)
    01: Data length is 1 word
    00 02: Key value 0002*/
     if (flag) {
      if (Buffer[0] == 0x5A && Buffer[1] == 0xA5) {
        // gets the data from vp location and process it

        // turning on/off front light
        if (Buffer[4] == highByte(DASHBOARD_FRONT_LIGHTS_VP) && Buffer[5] == lowByte(DASHBOARD_FRONT_LIGHTS_VP)) {
          digitalWrite(EVER_FRONT_LIGHT, bitRead(Buffer[8], 0));
        }

        // turning on/off right flasher
        if (Buffer[4] == highByte(DASHBOARD_TURNING_FLASHERS_R_VP) && Buffer[5] == lowByte(DASHBOARD_TURNING_FLASHERS_R_VP)) {
          digitalWrite(EVER_RIGHT_FLASHER, bitRead(Buffer[8], 0));
        }

        // turning on/off left flasher
        if (Buffer[4] == highByte(DASHBOARD_TURNING_FLASHERS_L_VP) && Buffer[5] == lowByte(DASHBOARD_TURNING_FLASHERS_L_VP)) {
          digitalWrite(EVER_LEFT_FLASHER, bitRead(Buffer[8], 0));
        }

        // turning on/off Car Horn
        if (Buffer[4] == highByte(DASHBOARD_CAR_HORN_VP) && Buffer[5] == lowByte(DASHBOARD_CAR_HORN_VP)) {
          digitalWrite(EVER_HORN, bitRead(Buffer[8], 0));
        }
      }
    }
    Buffer_len = 0;
    flag = false;
  }
}
