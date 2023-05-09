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
  rec.x = 0;
  rec.y = 0;
  rec.width = 10;
  rec.height = 10;
  Sprite sprite = CreateSprite(rec, (Vector2) {1, 1}, (Vector2) {0.5, 0.5}, (Vector2) {0.5, 0.5}, NULL);

  camera.target = (Vector2) {sprite.source.x, sprite.source.y};

  Rectangle ground;
  ground.x = 0;
  ground.y = 30;
  ground.height = 1;
  ground.width = data.map.x;

  while (!WindowShouldClose()) {

    // update here
    UpdatePlayerSprite(&sprite);
    FollowSprite(sprite, &camera, data.zoom, data.screen, data.map, 0.1);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    // draw here
    DrawText("HELLO WORLD!", 10, 10, 20, LIGHTGRAY);

    DrawSprite(sprite);
    DrawRectangleRec(ground, BLACK);


    EndDrawing();
  }

  CloseWindow();
}
