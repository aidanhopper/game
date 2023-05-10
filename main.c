#include "data.h"
#include "raylib.h"
#include "sprite.h"
#include "rlgl.h"

int main() {
  Data data;
  data.screen = (Vector2) {1280, 720};
  data.zoom = 10;
  data.map = (Vector2) {200, 200};

  InitWindow(data.screen.x, data.screen.y, "asdf");

  Camera2D camera = {0};
  SetTargetFPS(60);

  Rectangle rec;
  rec.x = 60;
  rec.y = 0;
  rec.width = 10;
  rec.height = 10;
  Sprite sprite = CreateSprite(rec, (Vector2) {2, 2}, (Vector2) {0.5, 0.5}, (Vector2) {0.5, 0.5}, NULL);

  camera.target = (Vector2) {sprite.source.x, sprite.source.y};

  data.collisionObject[0].x = 0;
  data.collisionObject[0].y = 30;
  data.collisionObject[0].height = 20;
  data.collisionObject[0].width = data.map.x;

  data.collisionObject[1].x = 40;
  data.collisionObject[1].y = 10;
  data.collisionObject[1].height = 20;
  data.collisionObject[1].width = 20;
  data.collisionObjectLength = 2;

  while (!WindowShouldClose()) {

    // update here
    UpdatePlayerSprite(&sprite, data.collisionObject, data.collisionObjectLength);
    FollowSprite(sprite, &camera, data.zoom, data.screen, data.map, 0.1);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    // draw here
    DrawText("HELLO WORLD!", 10, 10, 20, LIGHTGRAY);

    DrawSprite(sprite);
    DrawRectangleRec(data.collisionObject[0], BLACK);
    DrawRectangleRec(data.collisionObject[1], BLACK);


    EndDrawing();
  }

  CloseWindow();
}
