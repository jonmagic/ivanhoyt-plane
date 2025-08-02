#include <Arduino.h>
#include <MAVLink.h>
#include <CANSAME5x.h>

CANSAME5x CAN;

#define MAVLINK_SERIAL Serial1
#define MAVLINK_SYSTEM_ID 1
#define MAVLINK_COMPONENT_ID 1

bool OUTPUT_SERIAL  = true;
bool OUTPUT_aEFIS   = false;
bool OUTPUT_CAN     = true;

// CAN-FIX Parameter IDs
const uint16_t FIX_PITCH = 384;              // 0x180
const uint16_t FIX_ROLL = 385;               // 0x181
const uint16_t FIX_AOA = 386;                // 0x182 (optional)
const uint16_t FIX_INDICATED_AIRSPEED = 387; // 0x183
const uint16_t FIX_INDICATED_ALTITUDE = 388; // 0x184
const uint16_t FIX_HEADING = 389;            // 0x185 (magnetic heading)
const uint16_t FIX_VERTICAL_SPEED = 390;     // 0x186
const uint16_t FIX_LATITUDE  = 451;          // 0x1C3
const uint16_t FIX_LONGITUDE = 452;          // 0x1C4
const uint16_t FIX_GROUND_TRACK = 454;       // 0x1C6 - True Ground Track
const uint16_t FIX_COURSE = 457;             // 0x1C9 - Selected Course
const uint16_t FIX_LATERAL_ACCEL = 395;      // 0x18B
const uint16_t FIX_TURN_RATE = 1027;         // 0x403

const uint16_t nodeId = 0x82;

// AHRS and GPS variables
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;
float ground_speed = 0.0;
float gps_altitude = 0.0;
float vertical_speed = 0.0;
float turn_rate = 0.0;
float lateral_accel = 0.0;
float gps_track = 0.0;
float mav_lat = 0.0;
float mav_lon = 0.0;

// Scaled integer values for CAN-FIX
int32_t lngPitch = 0;
int32_t lngRoll = 0;
int32_t lngGlobalHeading = 0;
int32_t lngGroundSpeed = 0;
int32_t lngGpsAltitude = 0;
int32_t lngVerticalSpeed = 0;
int32_t lngTurnRate = 0;
int32_t lngLateralAccel = 0;
int32_t lngTrack = 0;

void handleMavlinkMessage() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while (MAVLINK_SERIAL.available()) {
    uint8_t c = MAVLINK_SERIAL.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_ATTITUDE: {
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          roll = attitude.roll * RAD_TO_DEG;
          pitch = attitude.pitch * RAD_TO_DEG;
          yaw = attitude.yaw * RAD_TO_DEG;
          // Calculate turn rate from yawspeed (rad/s to deg/s)
          turn_rate = attitude.yawspeed * RAD_TO_DEG;
          if (OUTPUT_SERIAL) {
            Serial.print("DBG ATT: pitch="); Serial.print(pitch);
            Serial.print(" roll="); Serial.print(roll);
            Serial.print(" yaw="); Serial.print(yaw);
            Serial.print(" turn_rate="); Serial.println(turn_rate);
          }
          break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
          mavlink_global_position_int_t gp;
          mavlink_msg_global_position_int_decode(&msg, &gp);
          gps_altitude = (gp.alt / 1000.0) * 3.28084; // mm to feet
          // Vertical speed in feet/min (vz is cm/s, down is positive)
          vertical_speed = (gp.vz / 10.0) * 3.28084 * 60.0;
          // Convert MAVLink lat/lon from int32 (degrees * 1e7) to float degrees
          mav_lat = gp.lat / 10000000.0;
          mav_lon = gp.lon / 10000000.0;
          // GPS heading/track (cdeg to degrees)
          gps_track = gp.hdg / 100.0;
          if (OUTPUT_SERIAL) {
            Serial.print("DBG GP_INT: lat="); Serial.print(mav_lat, 7);
            Serial.print(" lon="); Serial.print(mav_lon, 7);
            Serial.print(" alt_ft="); Serial.print(gps_altitude);
            Serial.print(" vs_fpm="); Serial.print(vertical_speed);
            Serial.print(" track="); Serial.println(gps_track);
          }
          break;
        }
        case MAVLINK_MSG_ID_GPS_RAW_INT: {
          mavlink_gps_raw_int_t gr;
          mavlink_msg_gps_raw_int_decode(&msg, &gr);
          ground_speed = (gr.vel / 100.0) * 1.94384; // cm/s to knots
          if (OUTPUT_SERIAL) {
            Serial.print("DBG RAW_INT: gs_kts="); Serial.println(ground_speed);
          }
          break;
        }
        case MAVLINK_MSG_ID_RAW_IMU: {
          mavlink_raw_imu_t imu;
          mavlink_msg_raw_imu_decode(&msg, &imu);
          // Lateral acceleration in G (assuming yacc is lateral)
          lateral_accel = imu.yacc / 1000.0;
          if (OUTPUT_SERIAL) {
            Serial.print("DBG IMU: lat_accel_g="); Serial.println(lateral_accel);
          }
          break;
        }
        default:
          break;
      }

      // Scale values for CAN-FIX transmission
      lngPitch = pitch * 100;                    // degrees * 100
      lngRoll = roll * 100;                      // degrees * 100
      lngGlobalHeading = normalizeHeading(yaw);  // degrees * 10
      lngGpsAltitude = gps_altitude + 1000;             // feet
      lngGroundSpeed = ground_speed * 10;        // knots * 10
      lngVerticalSpeed = vertical_speed;         // feet/min
      lngTurnRate = turn_rate * 10;              // deg/sec * 10
      lngLateralAccel = lateral_accel * 100;     // G * 100
      lngTrack = normalizeHeading(gps_track);    // degrees * 10

      if (OUTPUT_CAN) {
        sendCanMessage(FIX_PITCH, lngPitch);
        sendCanMessage(FIX_ROLL, lngRoll);
        sendCanMessage(FIX_HEADING, lngGlobalHeading);
        sendCanMessage(FIX_INDICATED_ALTITUDE, lngGpsAltitude);
        sendCanMessage(FIX_INDICATED_AIRSPEED, lngGroundSpeed);
        sendCanMessage(FIX_VERTICAL_SPEED, lngVerticalSpeed);
        sendCanMessage(FIX_TURN_RATE, lngTurnRate);
        sendCanMessage(FIX_LATERAL_ACCEL, lngLateralAccel);
        // Send latitude and longitude as float values
        sendCanMessage(FIX_LATITUDE, mav_lat);
        sendCanMessage(FIX_LONGITUDE, mav_lon);
        // Send ground track and course
        sendCanMessage(FIX_GROUND_TRACK, lngTrack);
        sendCanMessage(FIX_COURSE, lngTrack);
      }

      if (OUTPUT_SERIAL) {
        Serial.print("CAN_WR: pitch:"); Serial.print(lngPitch);
        Serial.print(", roll:"); Serial.print(lngRoll);
        Serial.print(", heading:"); Serial.print(lngGlobalHeading);
        Serial.print(", alt:"); Serial.print(lngGpsAltitude);
        Serial.print(", speed:"); Serial.print(lngGroundSpeed);
        Serial.print(", vs:"); Serial.print(lngVerticalSpeed);
        Serial.print(", tr:"); Serial.print(lngTurnRate);
        Serial.print(", la:"); Serial.print(lngLateralAccel);
        Serial.print(", track:"); Serial.print(lngTrack);
        Serial.print(", lat:"); Serial.print(mav_lat, 7);
        Serial.print(", lon:"); Serial.println(mav_lon, 7);
      }

      if (OUTPUT_aEFIS) {
        send_canfix_frame_to_aefis(FIX_PITCH, lngPitch);
        send_canfix_frame_to_aefis(FIX_ROLL, lngRoll);
        send_canfix_frame_to_aefis(FIX_HEADING, lngGlobalHeading);
        send_canfix_frame_to_aefis(FIX_INDICATED_ALTITUDE, lngGpsAltitude);
        send_canfix_frame_to_aefis(FIX_INDICATED_AIRSPEED, lngGroundSpeed);
        // Optionally send lat/lon to aEFIS if you want
      }
    }
  }
}

// Normalize heading to 0-360 degrees and scale by 10 for CAN-FIX
int32_t normalizeHeading(float heading_deg) {
  while (heading_deg < 0) heading_deg += 360;
  while (heading_deg >= 360) heading_deg -= 360;
  return (int32_t)(heading_deg * 10);
}

void sendCanMessage(int messageType, int32_t data) {
  byte byteData[4] = {
    static_cast<byte>(data),
    static_cast<byte>(data >> 8),
    static_cast<byte>(data >> 16),
    static_cast<byte>(data >> 24)
  };

  CAN.beginPacket(messageType);
  CAN.write(nodeId);
  CAN.write(0x00);
  CAN.write(0x00);
  for (int i = 0; i < 4; i++) {
    CAN.write(byteData[i]);
  }
  CAN.endPacket();

  if (OUTPUT_SERIAL) {
    Serial.print("CAN_PKT: ID="); Serial.print(messageType, HEX);
    Serial.print(" data=");
    for (int i = 0; i < 4; i++) {
      Serial.print(byteData[i], HEX); Serial.print(" ");
    }
    Serial.println();
  }
}

void sendCanMessage(int messageType, float data) {
  // Convert float to bytes using union for proper bit representation
  union {
    float f;
    byte bytes[4];
  } floatData;

  floatData.f = data;

  CAN.beginPacket(messageType);
  CAN.write(nodeId);
  CAN.write(0x00);
  CAN.write(0x00);
  for (int i = 0; i < 4; i++) {
    CAN.write(floatData.bytes[i]);
  }
  CAN.endPacket();

  if (OUTPUT_SERIAL) {
    Serial.print("CAN_PKT: ID="); Serial.print(messageType, HEX);
    Serial.print(" float_data="); Serial.print(data, 7);
    Serial.print(" bytes=");
    for (int i = 0; i < 4; i++) {
      Serial.print(floatData.bytes[i], HEX); Serial.print(" ");
    }
    Serial.println();
  }
}

void send_canfix_frame_to_aefis(int messageType, int32_t data) {
  byte message[] = {
    messageType,
    messageType >> 8,
    messageType >> 16,
    0x78, 0x00, 0x00,
    static_cast<byte>(data),
    static_cast<byte>(data >> 8),
    static_cast<byte>(data >> 16),
    static_cast<byte>(data >> 24)
  };
  Serial.write(message, 11);
}

int32_t getHeadingReciprocal(int32_t heading) {
  return (heading + 3600) % 3600;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (OUTPUT_SERIAL && Serial) {
    Serial.println("Serial Started ...");
  }

  MAVLINK_SERIAL.begin(115200);
  delay(100);
  if (OUTPUT_SERIAL && MAVLINK_SERIAL) {
    Serial.println("MAVLink Attitude Listener Started");
  }

  if (OUTPUT_CAN) {
    pinMode(PIN_CAN_STANDBY, OUTPUT);
    digitalWrite(PIN_CAN_STANDBY, false);
    pinMode(PIN_CAN_BOOSTEN, OUTPUT);
    digitalWrite(PIN_CAN_BOOSTEN, true);

    while (!CAN.begin(250000)) {
      if (OUTPUT_SERIAL) {
        Serial.println("CAN failed to initialize! Will try again in 1 second ...");
      }
      delay(1000);
    }
    if (OUTPUT_SERIAL) {
      Serial.println("CAN started!");
    }
  }
  delay(500);
}

void loop() {
  handleMavlinkMessage();
}
