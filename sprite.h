#ifndef SPRITE_H
#define SPRITE_H

#include "math.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Rectangle source;
  Texture2D *texture;
  Vector2 maxVelocity;
  Vector2 velocity;
  Vector2 acceleration;
  Vector2 deceleration;
} Sprite;

static Sprite CreateSprite(Rectangle source, Vector2 maxVelocity,
                           Vector2 acceleration, Vector2 deceleration,
                           Texture2D *texture) {
  Sprite sprite;
  sprite.texture = texture;
  sprite.source = source;
  sprite.maxVelocity = maxVelocity;
  sprite.velocity = (Vector2){0, 0};
  sprite.acceleration = acceleration;
  sprite.deceleration = deceleration;
  return sprite;
}

static void DrawSprite(Sprite sprite) {
  if (sprite.texture == NULL)
    DrawRectangleRec(sprite.source, RED);
  else
    DrawTextureRec(*sprite.texture, sprite.source, (Vector2){0, 0}, RAYWHITE);
}

static void Accelerate(Sprite *sprite, Vector2 direction) {
  Vector2 maxVelocity = sprite->maxVelocity;
  Vector2 acceleration = sprite->acceleration;

  sprite->velocity.x += acceleration.x * direction.x;
  if (abs(sprite->velocity.x) > abs(maxVelocity.x))
    sprite->velocity.x = maxVelocity.x * direction.x;

  // deals with y direction
  sprite->velocity.y += acceleration.y * direction.y;
  if (abs(sprite->velocity.y) > abs(maxVelocity.y))
    sprite->velocity.y = maxVelocity.y * direction.y;
}

static void MoveSprite(Sprite *sprite, Rectangle collisionObject[],
                       int collisionObjectLength) {
  // first move x axis
  sprite->source.x += sprite->velocity.x;
  for (int i = 0; i < collisionObjectLength; i++) {
    if (CheckCollisionRecs(collisionObject[i], sprite->source)) {
      if (sprite->velocity.x > 0)
        sprite->source.x = collisionObject[i].x - sprite->source.width;
      else if (sprite->velocity.x < 0)
        sprite->source.x = collisionObject[i].x + collisionObject[i].width;
    }
  }

  // then y axis
  sprite->source.y += sprite->velocity.y;
  for (int i = 0; i < collisionObjectLength; i++) {
    if (CheckCollisionRecs(collisionObject[i], sprite->source)) {
      if (sprite->velocity.y > 0)
        sprite->source.y = collisionObject[i].y - sprite->source.height;
      else if (sprite->velocity.y < 0)
        sprite->source.y = collisionObject[i].y + collisionObject[i].height;
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

  if (sprite->velocity.y >= sprite->deceleration.y)
    sprite->velocity.y -= sprite->deceleration.y * axis.y;
  else if (sprite->velocity.y <= -sprite->deceleration.y)
    sprite->velocity.y += sprite->deceleration.y * axis.y;
  else
    sprite->velocity.y = 0;
}

static void HandleSpriteInput(Sprite *sprite) {
  Vector2 noInput = {1, 1};
  if (IsKeyDown(KEY_W)) {
    Accelerate(sprite, (Vector2){0, -1});
    noInput.y = 0;
  }
  if (IsKeyDown(KEY_S)) {
    Accelerate(sprite, (Vector2){0, 1});
    noInput.y = 0;
  }
  if (IsKeyDown(KEY_A)) {
    Accelerate(sprite, (Vector2){-1, 0});
    noInput.x = 0;
  }
  if (IsKeyDown(KEY_D)) {
    Accelerate(sprite, (Vector2){1, 0});
    noInput.x = 0;
  }
  Decelerate(sprite, noInput);
}

static void FollowSprite(Sprite sprite, Camera2D *camera, double zoom,
                         Vector2 screen, Vector2 map, double cameraSpeed) {
  if (cameraSpeed > 1)
    cameraSpeed = 1;

  // set offset
  camera->offset = (Vector2){
      (float)screen.x / 2 - (sprite.source.width * camera->zoom / 2),
      (float)screen.y / 2 - (sprite.source.height * camera->zoom / 2)};

  // shorten var names
  Vector2 target = {sprite.source.x, sprite.source.y};

  camera->zoom = zoom;
  // handle x axis
  if (camera->target.x != target.x)
    camera->target.x -= (camera->target.x - target.x) * cameraSpeed;
  if (camera->target.x < camera->offset.x / zoom)
    camera->target.x = camera->offset.x / zoom;
  if (camera->target.x > map.x - camera->offset.x / zoom - sprite.source.width)
    camera->target.x = map.x - camera->offset.x / zoom - sprite.source.width;

  // handle y axis
  if (camera->target.y != target.y)
    camera->target.y -= (camera->target.y - target.y) * cameraSpeed;
  if (camera->target.y < camera->offset.y / zoom)
    camera->target.y = camera->offset.y / zoom;
  if (camera->target.y > map.y - camera->offset.y / zoom - sprite.source.height)
    camera->target.y = map.y - camera->offset.y / zoom - sprite.source.height;
}

static void UpdatePlayerSprite(Sprite *sprite, Rectangle collisionObject[],
                               int collisionObjectLength) {
  HandleSpriteInput(sprite);
  MoveSprite(sprite, collisionObject, collisionObjectLength);
}

#endif
