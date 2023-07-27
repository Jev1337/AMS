#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 20, 4);
int i = 0;


int id;
int test = 1;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("[Initialized]");
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  //Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  finger.verifyPassword();

  finger.getTemplateCount();

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
}

char c = 'n';

void loop()
{
  if (c == 'n') {
    while (Serial.available() == 0);
    c = Serial.read();
    if (c == 'l')
          for (int j = 0; j <= 3 ; j++) {
        digitalWrite(9, LOW);
        delay(50);
        digitalWrite(9, HIGH);
        delay(50);
      }
    else if (c == 'f'){
        lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[Fingerprint]");
    }
    else{
              lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[Waiting]");
    }
  }

  if (c == 'l') {
    if (Serial.available())
    { delay(100);
      lcd.clear();
      // for the new strings
      lcd.setCursor(0, 0);
      i = 0;
      // read all the available characters
      while (Serial.available() > 0)
      { // display each character to the LCD = #
        lcd.write(Serial.read());
        if (i >= 15)
        { // for strings with length > 16
          lcd.setCursor(0, 1);
          while (Serial.available() > 0)
          {
            lcd.write(Serial.read());
          }
        }
        i++;
      }
    }
  }
  if (c == 'f') {

  
    String receivedData; // Initialize an empty string to store the concatenated data

    if (Serial.available()) {
      while (Serial.available() > 0) {
        char receivedChar = Serial.read(); // Read a character from the Serial port
        receivedData += receivedChar; // Concatenate the character to the string
      }
      id = receivedData.toInt();
      
    if (id == 0)
      uint8_t t = getFingerprintID() ;
    if (id > 0 && id < 127)
      getFingerprintEnroll();
    }
    delay(50);            //don\"t ned to run this at full speed.
  }
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("{\"Module\":\"fingerprint\", \"Id\":null, \"Status\":\"NFG\"}");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("{\"Module\":\"fingerprint\", \"Id\":null, \"Status\":\"RET\"}");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("{\"Module\":\"fingerprint\", \"Id\":null, \"Status\":\"ERR\"}");
      return p;
    default:
      Serial.println("{\"Module\":\"fingerprint\", \"Id\":null, \"Status\":\"ERR\"}");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"ERR\"}");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"RET\"}");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"ERR\"}");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"ERR\"}");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\": \"" + String(finger.fingerID) + "\",\"Status\": \"OK\"}");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"RET\"}");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"null\",\"Status\": \"INC\"}");
    return p;
  } else {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"null\",\"Status\": \"UNK\"}");
    return p;
  }

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  return finger.fingerID;
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  //Serial.print("{\"Module\": \"fingerprint\", \"Id\":\"null\",\"Status\": \"PUT_FINGER\"}");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("{\"Module\":\"\"fingerprint\", \"Id\":\"null\",\"Status\":\"\"RET\"}");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"null\",\"Status\": \"UNK\"}");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"RET\"}");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"UNK\"}");
      return p;
  }

  Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"REMOVE_FINGER\"}");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"PUT_FINGER_AGAIN\"}");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"ERR\"}");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"UNK\"}");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"ERR\"}");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"UNK\"}");
      return p;
  }

  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"OK\"}");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"ERR\"}");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"INC\"}");
    return p;
  } else {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"UNK\"}");
    return p;
  }

  //Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"ERR\"}");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"BAD_LOC\"}");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"BAD_FLASH\"}");
    return p;
  } else {
    Serial.println("{\"Module\": \"fingerprint\", \"Id\":\"" + String(id) + "\",\"Status\": \"UNK\"}");
    return p;
  }

  return true;
}
