#ifndef DATA_H
#define DATA_H

#include <raylib.h>
#include <time.h>

typedef struct {
  Vector2 screen; 
  double zoom;
  Vector2 map;
  Rectangle collisionObject[100];
  int collisionObjectLength;
  float dt;
} Data;

#endif
