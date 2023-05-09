#ifndef DATA_H
#define DATA_H

#include <raylib.h>

typedef struct {
  Vector2 screen; 
  double zoom;
  Vector2 map;
  Rectangle collisionObject[100];
  int collisionObjectLength;
} Data;

#endif
