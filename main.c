#include "data.h"
#include "debug.h"
#include "raylib.h"
#include "rlgl.h"
#include "sprite.h"

int main() {
  Data data;
  data.screen = (Vector2){1280, 720};
  data.zoom = 10;
  data.map = (Vector2){200, 200};

  InitWindow(data.screen.x, data.screen.y, "asdf");

  Texture2D texture = LoadTexture("images/player.png");

  Camera2D camera = {0};
  SetTargetFPS(60);

  Sprite sprite =
      CreateSprite((Rectangle){60, 0, 8, 10}, (Rectangle){0, 0, 8, 10},
                   (Rectangle){1, 0, 6, 10}, (Vector2){2, 5},
                   (Vector2){0.5, 0.5}, (Vector2){0.5, 0.5}, 7, &texture);

  camera.target = (Vector2){sprite.destination.x, sprite.destination.y};

  data.collisionObject[0].x = 0;
  data.collisionObject[0].y = 60;
  data.collisionObject[0].height = 20;
  data.collisionObject[0].width = data.map.x;

  data.collisionObject[1].x = 40; data.collisionObject[1].y = 40;
  data.collisionObject[1].height = 20;
  data.collisionObject[1].width = 20;

  data.collisionObject[2].x = -50;
  data.collisionObject[2].y = 0;
  data.collisionObject[2].width = 50;
  data.collisionObject[2].height = data.map.y;

  data.collisionObject[3].x = data.map.x;
  data.collisionObject[3].y = 0;
  data.collisionObject[3].width = 50;
  data.collisionObject[3].height = data.map.y;
  data.collisionObjectLength = 4;

  Color color;
  color.a = 1;
  color.r = 39;
  color.g = 39;
  color.b = 68;

  while (!WindowShouldClose()) {

    // update here
    UpdatePlayerSprite(&sprite, data.collisionObject,
                       data.collisionObjectLength);
    FollowSprite(sprite, &camera, data.zoom, data.screen, data.map, 0.2);

    BeginDrawing();
    ClearBackground(color);
    BeginMode2D(camera);

    // draw here
    // DrawText("HELLO WORLD!", 10, 10, 20, RAYWHITE);

    DrawCollisionObjects(data.collisionObject, data.collisionObjectLength);
    DrawSprite(sprite);

    EndDrawing();
  }

  CloseWindow();
}
