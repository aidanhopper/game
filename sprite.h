#ifndef SPRITE_H
#define SPRITE_H

#include "math.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TARGET_FPS 60

typedef enum State { IDLE = 0, MOVING, DASHING, JUMPING } State;

typedef struct {
  Rectangle destination;
  Rectangle source;
  Rectangle collision; // collision rect relative to the destination rect
  Texture2D *texture;
  Vector2 maxVelocity;
  Vector2 velocity;
  Vector2 acceleration;
  Vector2 deceleration;
  double jump_speed;
  int dash_speed;
  int collisionType[4];
  int facingRight;
  State state;
} Sprite;

static Sprite CreateSprite(Rectangle destination, Rectangle source,
                           Rectangle collision, Vector2 maxVelocity,
                           Vector2 acceleration, Vector2 deceleration,
                           double jump_speed, Texture2D *texture) {
  Sprite sprite;
  for (int i = 0; i < 4; i++)
    sprite.collisionType[i] = 0;
  sprite.texture = texture;
  sprite.destination = destination;
  sprite.source = source;
  sprite.collision = collision;
  sprite.dash_speed = 8;
  sprite.maxVelocity = maxVelocity;
  sprite.velocity = (Vector2){0, 0};
  sprite.acceleration = acceleration;
  sprite.deceleration = deceleration;
  sprite.jump_speed = jump_speed;
  return sprite;
}

static void DrawSprite(Sprite sprite) {
  if (sprite.texture == NULL)
    DrawRectangleRec(sprite.destination, RED);
  else {
    // DrawTextureRec(*sprite.texture, sprite.destination, sprite.destination,
    // RAYWHITE);
    if (sprite.facingRight) {
      DrawTexturePro(*sprite.texture, sprite.source, sprite.destination,
                     (Vector2){0, 0}, 0, WHITE);
    } else {
      sprite.source.width = -sprite.source.width;
      DrawTexturePro(*sprite.texture, sprite.source, sprite.destination,
                     (Vector2){0, 0}, 0, WHITE);
    }
  }
}

static void Accelerate(Sprite *sprite, Vector2 direction) {
  Vector2 maxVelocity = sprite->maxVelocity;
  Vector2 acceleration = sprite->acceleration;

  if (abs(sprite->velocity.x + acceleration.x * direction.x) <= maxVelocity.x)
    sprite->velocity.x += acceleration.x * direction.x;

  // deals with y direction
  sprite->velocity.y += acceleration.y * direction.y;
  if (sprite->velocity.y > maxVelocity.y)
    sprite->velocity.y = maxVelocity.y * direction.y;
}

static double Dash(Sprite *sprite) {
  sprite->state = DASHING;
  sprite->velocity.y = 0;
  if (sprite->facingRight)
    sprite->velocity.x = sprite->dash_speed;
  else
    sprite->velocity.x = -sprite->dash_speed;
  if (sprite->collisionType[0] || sprite->collisionType[1])
    sprite->state = MOVING;
  return GetTime();
}

// TODO: need to get collisions with collision rectangle working
static void MoveSprite(Sprite *sprite, Rectangle collisionObject[],
                       int collisionObjectLength) {

  // handle state here
  //printf("STATE: %d\n", sprite->state);

  if (sprite->state == MOVING && sprite->velocity.x > sprite->maxVelocity.x)
    sprite->velocity.x = sprite->maxVelocity.x;

  // handle collisions here
  for (int i = 0; i < 4; i++)
    sprite->collisionType[i] = 0;
  // first move x axis
  sprite->destination.x += sprite->velocity.x;
  if (sprite->velocity.x > 0)
    sprite->facingRight = 1;
  else if (sprite->velocity.x < 0)
    sprite->facingRight = 0;
  for (int i = 0; i < collisionObjectLength; i++) {
    Rectangle collisonRect = {sprite->destination.x + sprite->collision.x,
                              sprite->destination.y + sprite->collision.y,
                              sprite->collision.width,
                              sprite->collision.height};
    if (CheckCollisionRecs(collisionObject[i], collisonRect)) {
      if (sprite->velocity.x > 0) {
        sprite->destination.x = collisionObject[i].x -
                                sprite->destination.width + sprite->collision.x;
        sprite->collisionType[1] = 1;
      } else if (sprite->velocity.x < 0) {
        sprite->destination.x = collisionObject[i].x +
                                collisionObject[i].width - sprite->collision.x;
        sprite->collisionType[0] = 1;
      }
    }
  }
  // then y axis
  sprite->destination.y += sprite->velocity.y;
  if (sprite->collisionType[0] || sprite->collisionType[1]) {
    sprite->velocity.x = 0;
    if (!sprite->collisionType[2] &&
        sprite->velocity.y > sprite->maxVelocity.y / 100)
      sprite->velocity.y = sprite->maxVelocity.y / 100;
  }
  for (int i = 0; i < collisionObjectLength; i++) {
    Rectangle collisonRect = {sprite->destination.x + sprite->collision.x,
                              sprite->destination.y + sprite->collision.y,
                              sprite->collision.width,
                              sprite->collision.height};
    if (CheckCollisionRecs(collisionObject[i], collisonRect)) {
      if (sprite->velocity.y > 0) {
        sprite->destination.y = collisionObject[i].y -
                                sprite->destination.height +
                                sprite->collision.y;
        sprite->collisionType[3] = 1;
      } else if (sprite->velocity.y < 0) {
        sprite->destination.y = collisionObject[i].y +
                                collisionObject[i].height - sprite->collision.y;
        sprite->collisionType[2] = 1;
      }
    }
  }
}

static void Decelerate(Sprite *sprite, Vector2 axis) {
  if (sprite->velocity.x >= sprite->deceleration.x)
    sprite->velocity.x -= sprite->deceleration.x * axis.x;
  else if (sprite->velocity.x <= -sprite->deceleration.x)
    sprite->velocity.x += sprite->deceleration.x * axis.x;
  else
    sprite->velocity.x = 0;

  /*
  if (sprite->velocity.y >= sprite->deceleration.y)
    sprite->velocity.y -= sprite->deceleration.y * axis.y * -1;
  else if (sprite->velocity.y <= -sprite->deceleration.y)
    sprite->velocity.y += sprite->deceleration.y * axis.y * -1;
  else
    sprite->velocity.y = 0;
  */
}

static void Jump(Sprite *sprite) { sprite->velocity.y = -sprite->jump_speed; }


static void HandleSpriteInput(Sprite *sprite) {
  Vector2 noInput = {1, 1};
  // dash timer setup
  static double *lastDashTime = NULL;
  double timeSinceLastDash = 0;
  if (lastDashTime == NULL)
    lastDashTime = (double *)malloc(sizeof(double));
  else
    timeSinceLastDash = GetTime() - *lastDashTime;

  // jump timer setup
  static double *lastJumpTime = NULL;
  double timeSinceLastJump = 0;
  if (lastJumpTime == NULL)
    lastJumpTime = (double *)malloc(sizeof(double));
  else
    timeSinceLastJump = GetTime() - *lastJumpTime;

  // jump
  if (IsKeyDown(KEY_SPACE) && sprite->collisionType[3] &&
      timeSinceLastJump >= 1 &&
      (sprite->collisionType[3] ||
       (sprite->collisionType[0] && sprite->facingRight) ||
       (sprite->collisionType[1] && !sprite->facingRight))) {
    Jump(sprite);
    sprite->state = JUMPING;
  }
  if (IsKeyDown(KEY_S)) {
  }
  // dash
  if (IsKeyDown(KEY_LEFT_SHIFT) && timeSinceLastDash >= 2 && sprite->state != DASHING) {
    *lastDashTime = Dash(sprite);
    timeSinceLastDash = GetTime() - *lastDashTime;
  }
  // move left
  if (IsKeyDown(KEY_A)) {
    Accelerate(sprite, (Vector2){-1, 0});
    noInput.x = 0;
  }
  // move right
  if (IsKeyDown(KEY_D)) {
    Accelerate(sprite, (Vector2){1, 0});
    noInput.x = 0;
  }

  Decelerate(sprite, noInput);
  Accelerate(sprite, (Vector2){0, 1});

  if (sprite->state == DASHING && timeSinceLastDash > 0.1) {
    if (sprite->facingRight)
      sprite->velocity.x = sprite->maxVelocity.x;
    else
      sprite->velocity.x = -sprite->maxVelocity.x;
    sprite->state = MOVING;
  }
  else if (sprite->state == DASHING) {
    Dash(sprite);
  }
  

}

static void FollowSprite(Sprite sprite, Camera2D *camera, double zoom,
                         Vector2 screen, Vector2 map, double cameraSpeed) {
  if (cameraSpeed > 1)
    cameraSpeed = 1;

  // set offset
  camera->offset = (Vector2){
      (float)screen.x / 2 - (sprite.destination.width * camera->zoom / 2),
      (float)screen.y / 2 - (sprite.destination.height * camera->zoom / 2)};

  // shorten var names
  Vector2 target = {sprite.destination.x, sprite.destination.y};

  camera->zoom = zoom;
  // handle x axis
  if (camera->target.x != target.x)
    camera->target.x -= (camera->target.x - target.x) * cameraSpeed;
  if (camera->target.x < camera->offset.x / zoom)
    camera->target.x = camera->offset.x / zoom;
  if (camera->target.x >
      map.x - camera->offset.x / zoom - sprite.destination.width)
    camera->target.x =
        map.x - camera->offset.x / zoom - sprite.destination.width;

  // handle y axis
  if (camera->target.y != target.y)
    camera->target.y -= (camera->target.y - target.y) * cameraSpeed;
  if (camera->target.y < camera->offset.y / zoom)
    camera->target.y = camera->offset.y / zoom;
  if (camera->target.y >
      map.y - camera->offset.y / zoom - sprite.destination.height)
    camera->target.y =
        map.y - camera->offset.y / zoom - sprite.destination.height;
}

static void UpdatePlayerSprite(Sprite *sprite, Rectangle collisionObject[],
                               int collisionObjectLength) {
  HandleSpriteInput(sprite);
  MoveSprite(sprite, collisionObject, collisionObjectLength);
}

#endif
