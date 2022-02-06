#include "ZKBW2.h"
#include "config.h"

#include <cstddef>
#include <MFRC522.h>
#include <require_cpp11.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <SPI.h>
#include <Keyboard.h>
#include <Adafruit_DotStar.h>

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

// Create DotStar instance
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);


void setup() {

  // Initialiase the SPI bus
  SPI.begin();

  // Initialise the MFRC522 Card Reader
  mfrc522.PCD_Init();
  mfrc522.PCD_WriteRegister(MFRC522::RFCfgReg, MFRC522::RxGain_avg);

  // Initialise the dotstar
  strip.begin();
  strip.setBrightness(80);
  strip.setPixelColor(0, 255, 255, 255);
  strip.show();

}

void loop() {

  // Have the dotstar set to white
  strip.setPixelColor(0, 255, 255, 255);
  strip.show();

  currentTime = millis();
  if (currentTime - previousTime >= (keyPressDelay * 60000))
  {
    previousTime = currentTime;
    Keyboard.begin(KBD_Layout);
    Keyboard.write(keyPress);
    Keyboard.end();
  }


  //Define some variables;
  byte blocks;
  byte blocklength;
  MFRC522::PICC_Type cardName;

  // Look for new cards, and select one if present
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {
    // Get the card type
    cardName = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Select the Card info
    switch (cardName)
    {
      case MFRC522::PICC_TYPE_ISO_14443_4:
        // Not yet defined
        return;
        break;
      case MFRC522::PICC_TYPE_ISO_18092:
        // Not yet defined
        return;
        break;
      case MFRC522::PICC_TYPE_MIFARE_MINI:
        // The Classic Mifare Mini has 20 blocks of 16 bytes each.
                blocks=19;
        blocklength=16;
        break;
      case MFRC522::PICC_TYPE_MIFARE_UL:
        // The Mifare Ultralight has 16 blocks of 4 bytes each.
        blocks=15;
        blocklength=4;
        // If this is an NTAG21[356] it will show up here, the UID will be 7 characters
        // and the first UID byte will be 04h
        if (mfrc522.uid.uidByte[0] == 04 && mfrc522.uid.size == 7)
        {

          // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
          for (uint8_t i = 0; i < 6; i++) {
            key.keyByte[i] = 0xFF;
          }
          uint8_t bufferSize = 18;
          uint8_t buffer[bufferSize];
          uint8_t block=3;
          MFRC522::StatusCode status;

          // Read block
          status = mfrc522.MIFARE_Read(block, buffer, &bufferSize);
          if (status != MFRC522::STATUS_OK)
          {
            return;
          }

          switch (buffer[2])
          {
            case 0x12:
              // NTAG213
              blocks=43;
              break;
            case 0x3E:
              // NTAG215
              blocks=133;
              break;
            case 0x6D:
              // NTAG216
              blocks=229;
              break;
                          default:
              //Unknown NXP tag
              return;
          }
        }
        break;
//      case MFRC522::PICC_TYPE_MIFARE_UL_C:
//        // The Mifare Ultralight C has 48 blocks of 4 bytes each.
//        blocks=47;
//        blocklength=4;
//        break;
      case MFRC522::PICC_TYPE_MIFARE_PLUS:
        // No information
        return;
        break;
      case MFRC522::PICC_TYPE_MIFARE_1K:
        // The Classic Mifare 1K has 64 blocks of 16 bytes each.
        blocks=63;
        blocklength=16;
        break;
      case MFRC522::PICC_TYPE_MIFARE_4K:
        // The Classic Mifare 4K has 256 blocks of 16 bytes each.
        blocks= 255;
        blocklength=16;
        break;
      default:
        // We don't know how to handle this card type yet.
        return;
    }
    // Check the chip for a password wrapped in the header/footer...
    if ( ! getPassword(blocks, blocklength))
    {
      // Then check to see if any of the encrypted passwords can be decrypted with the UID
      if ( ! getEncryptedPassword(mfrc522.uid))
      {
        // Then check to see if the password is in the plain text array
        if ( ! getUIDPassword(mfrc522.uid))
        {
          // Finally just output the UID
          getUID(mfrc522.uid);
        }
      }
    }
  }

  // Close the card reader
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

int getUIDPassword(MFRC522::Uid uid) {
  uint8_t entry = 0;
  uint8_t pwLength;

  char hexDigits[] = "0123456789ABCDEF";
  // Convert the uid into a Hex String
  char UIDString[(uid.size * 2) + 1 ];
  for (uint8_t ch ; ch < uid.size ; ch ++)
  {
    UIDString[(ch * 2)] = hexDigits[uid.uidByte[ch]/16];
    UIDString[(ch *2) + 1] = hexDigits[uid.uidByte[ch]%16];
  }
  UIDString[uid.size * 2] = '\0';

  while ( UIDPassword[entry][0] != NULL )
  {
    if (strcmp(UIDString, UIDPassword[entry][0]) == 0)
    {
      // Set the Dotstar to Blue
      strip.setPixelColor(0, 0, 0, 255);
      strip.show();
      pwLength = strlen(UIDPassword[entry][1]);
      Keyboard.begin(KBD_Layout);
      for (uint8_t i = 0; i < pwLength ; i++)
      {
        Keyboard.write(UIDPassword[entry][1][i]);
      }
      Keyboard.write(KEY_RETURN);
      Keyboard.end();
      delay(1000);
      return(true);
    }
    entry++;
  }
  return(false);
}

int getEncryptedPassword(MFRC522::Uid uid) {
  uint8_t entry = 0;
  uint8_t pwLength;
  uint8_t headerLength = strlen(header);

  // Xor the header with the UID
  char encryptedHeader[headerLength];
  for (uint8_t h=0; h < headerLength; h++)
  {
    encryptedHeader[h] = header[h]^uid.uidByte[h % uid.size];
  }
  // Walk through the encrypted password table comparing the encrypted header to the start of the entry
  while ( encryptedPassword[entry] != NULL )
  {
    // It matches, XOR the entire string.
    if (strncmp(encryptedHeader, encryptedPassword[entry], headerLength) == 0)
    {
      // Now get the decrypted password
      pwLength = strlen(encryptedPassword[entry]) - headerLength;
      char password[pwLength];
      for (uint8_t c = 0; c < pwLength; c++)
      {
        password[c] = (encryptedPassword[entry][c + headerLength] ^ uid.uidByte[(c + headerLength) % uid.size]);
      }
      // Set the Dotstar to Orange
      strip.setPixelColor(0, 255, 128, 0);
      strip.show();
      Keyboard.begin(KBD_Layout);
      for (uint8_t i=0; i < pwLength ; i++)
      {
        Keyboard.write(password[i]);
      }
      Keyboard.write(KEY_RETURN);
      Keyboard.end();
      delay(1000);
      return(true);
    }
    // Otherwise try the next one.
    entry++;
  }

  return(false);
}

int getPassword(uint8_t blocks, uint8_t blockLength) {

  //some variables we need
  uint8_t block;
  byte bufferSize;
  bool password = false;
  MFRC522::StatusCode status;

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (uint8_t i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }


  bufferSize = blockLength + 2;
  byte buffer[bufferSize];

  for (block = 1; block <= blocks; block++)
  {

    // Authenticate
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 0f MFRC522.cpp file
    if (status != MFRC522::STATUS_OK)
    {
      return(false);
    }

    // Read block
    status = mfrc522.MIFARE_Read(block, buffer, &bufferSize);
    if (status != MFRC522::STATUS_OK)
    {
      return(false);
    }

    // Check data and output the password
    for (int i = 0; i < bufferSize; i++)
    {
      if (buffer[i] == header[0])
      {
        for (int j = i; j < strlen(header); j++)
        {
          if (buffer[i+j] != header[j])
          {
            break;
          } else if ( j == (strlen(header) - 1))
          {
            password = true;
          }
        }
        if (password)
        {
          // We found the password header
          // Set the Dotstar to Green
          strip.setPixelColor(0, 0, 255, 0);
          strip.show();

          // Now get ready to start typing
          Keyboard.begin(KBD_Layout);
          password = true;
          i += strlen(header);
          while (buffer[i] != tail)
          {
            Keyboard.write(buffer[i]);
            i++;
          }
          Keyboard.write(KEY_RETURN);
          Keyboard.end();
          delay(1000);
          return(true);
        }
      }
    }
  }
  return(false);
}

int getUID(MFRC522::Uid uid) {
  char hexDigits[] = "0123456789ABCDEF";

  // Set the DotStar to Red
  strip.setPixelColor(0, 255, 0, 0);
  strip.show();

  Keyboard.begin(KBD_Layout);

  // Convert the uid into a Hex String
  for (uint8_t ch ; ch < uid.size ; ch ++)
  {
    Keyboard.write(hexDigits[uid.uidByte[ch]/16]);
    Keyboard.write(hexDigits[uid.uidByte[ch]%16]);
  }

  Keyboard.write(KEY_RETURN);
  Keyboard.end();
  delay(1000);
  return(true);
}
