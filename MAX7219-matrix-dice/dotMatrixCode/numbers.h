#pragma once
#include <avr/pgmspace.h>

/*
 * Dice face definitions for MAX7219 8×8 matrix
 * Each face: 8 rows × 8 bits (as '0'/'1' ASCII strings)
 */

const char diceBits[][8][9] PROGMEM = { // dfines: A global variable diceBits in flash
  { // Face 1
    "00000000",
    "00000000",
    "00000000",
    "00011000",
    "00011000",
    "00000000",
    "00000000",
    "00000000"
  },
  { // Face 2
    "00000000",
    "00000110",
    "00000110",
    "00000000",
    "00000000",
    "01100000",
    "01100000",
    "00000000"
  },
  { // Face 3
    "00000110",
    "00000110",
    "00000000",
    "00011000",
    "00011000",
    "00000000",
    "01100000",
    "01100000"
  },
  { // Face 4
    "00000000",
    "01100110",
    "01100110",
    "00000000",
    "00000000",
    "01100110",
    "01100110",
    "00000000"
  },
  { // Face 5
    "01100110",
    "01100110",
    "00000000",
    "00011000",
    "00011000",
    "00000000",
    "01100110",
    "01100110"
  },
  { // Face 6
    "01100110",
    "01100110",
    "00000000",
    "01100110",
    "01100110",
    "00000000",
    "01100110",
    "01100110"
  }
};

/* Auto-calculate number of faces (always correct) */
const byte numFaces = sizeof(diceBits) / sizeof(diceBits[0]); 
// Defines: A global constant numFaces in RAM (holds the number of dice faces)


