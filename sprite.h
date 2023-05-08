#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <stdlib.h>
#include "vector2int.h"
#include "data.h"

typedef struct {
  Rectangle source;
  Texture2D *texture;
} Sprite;

static Sprite CreateSprite(Rectangle source, Texture2D *texture) {
  Sprite sprite;
  sprite.texture = texture;
  sprite.source = source;
  return sprite;
}

static void DrawSprite(Sprite sprite) {
  if (sprite.texture == NULL)
    DrawRectangleRec(sprite.source, RED);
  else
   DrawTextureRec(*sprite.texture, sprite.source, (Vector2) {0,0}, RAYWHITE);
}

static void SetSpritePosition(Sprite *sprite, double x, double y) {
  sprite->source.x = x;
  sprite->source.y = y;
}

static void MoveSprite(Sprite *sprite, double dx, double dy) {
  SetSpritePosition(sprite, sprite->source.x + dx, sprite->source.y + dy);
}

static void HandleSpriteInput(Sprite *sprite, double dx, double dy) {
  if (IsKeyDown(KEY_W)) MoveSprite(sprite, 0, -dy);
  if (IsKeyDown(KEY_S)) MoveSprite(sprite, 0, dy);
  if (IsKeyDown(KEY_A)) MoveSprite(sprite, -dx, 0);
  if (IsKeyDown(KEY_D)) MoveSprite(sprite, dx, 0);
}

static void FollowSprite(Sprite sprite, Camera2D *camera, Data data) {
  camera->target.x = sprite.source.x;
  camera->target.x = sprite.source.y;
  camera->offset.x = sprite.source.x + 1;
  camera->offset.y = sprite.source.y + 1;
}

#endif
