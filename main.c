#include "data.h"
#include "debug.h"
#include "raylib.h"
#include "rlgl.h"
#include "sprite.h"

int main() {
  Data data;
  data.screen = (Vector2){1280, 720};
  data.zoom = 8;
  data.map = (Vector2){500, 70};
  data.dt = 0;

  InitWindow(data.screen.x, data.screen.y, "asdf");

  Texture2D texture = LoadTexture("images/player.png");

  Camera2D camera = {0};
  SetTargetFPS(60);

  Sprite sprite =
      CreateSprite((Rectangle){70, 60, 8, 10}, (Rectangle){0, 0, 8, 10},
                   (Rectangle){1, 0, 6, 10}, (Vector2){2, 5},
                   (Vector2){0.5, 0.5}, (Vector2){0.5, 0.5}, 7, &texture);

  camera.target = (Vector2){sprite.destination.x, sprite.destination.y};

  data.collisionObject[0].x = 0;
  data.collisionObject[0].y = 60;
  data.collisionObject[0].height = 20;
  data.collisionObject[0].width = data.map.x;

  data.collisionObject[1].x = -20;
  data.collisionObject[1].y = 0;
  data.collisionObject[1].height = 60;
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

  Color bg;
  bg.a = 1;
  bg.r = 39;
  bg.g = 39;
  bg.b = 68;

  Texture2D bg0 = LoadTexture("./images/bg0.png");

  int state = 0;

  while (!WindowShouldClose()) {
    switch (state) {
    case 0: // menu
      BeginDrawing();
      ClearBackground(bg);
      DrawText("Press Enter to Play", 100, data.screen.y / 2 - 100, 100,
               RAYWHITE);
      EndDrawing();

      if (IsKeyDown(KEY_ENTER))
        state = 1;

      break;
    case 1: // game
      // update here
      UpdatePlayerSprite(&sprite, data.collisionObject,
                         data.collisionObjectLength);
      FollowSprite(sprite, &camera, data.zoom, data.screen, data.map, 0.2);

      BeginDrawing();
      ClearBackground(bg);
      BeginMode2D(camera);

      // draw here
      DrawText("HELLO WORLD!", 10, 10, 20, RAYWHITE);

      DrawCollisionObjects(data.collisionObject, data.collisionObjectLength);

      BeginBlendMode(BLEND_ADDITIVE);
      DrawSprite(sprite);
      EndBlendMode();

      // DrawSprite(sprite);
      EndDrawing();
      break;
    }
  }

  CloseWindow();
}
