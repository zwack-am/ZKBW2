#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

// Valid settings are:
//   KeyboardLayout_de_DE
//   KeyboardLayout_en_US
//   KeyboardLayout_es_ES
//   KeyboardLayout_fr_FR
//   KeyboardLayout_it_IT

#define KBD_Layout KeyboardLayout_en_US

// Create the UIDPassword Array
char * UIDPassword[][2] = {
  { "DEADBEEF", "Password1"},
  { "UID2", "Password2"},
  { "UID3", "Password3"},
  { NULL, NULL }
};



// Create the encrypted UIDPassword Array
// Encrypted password array
char * encryptedPassword[] = {
  "\x84\xfd\xf5\xb8\x8e\xcc\xcd\x9c\xa9\xc2\xcc\x8b\xef",
  NULL
};

// Set the header string
char header[] = "ZPKW";

// Set the tail character
char tail = 0;

// Time between sent key presses in minutes.
unsigned long keyPressDelay = 10;

// The key to send
//char keyPress = 0x41; //A
char keyPress = 0xF2; //F15

#endif // CONFIG_H_INCLUDED
