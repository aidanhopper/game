#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>

static void DrawCollisionObjects(Rectangle collisionObject[],
                                 int collisionObjectLength) {
  for (int i = 0; i < collisionObjectLength; i++) {
    DrawRectangleRec(collisionObject[i], BLACK);
  }
}

#endif
