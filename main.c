#include "data.h"
#include "raylib.h"
#include "sprite.h"
#include "rlgl.h"

int main() {
  Data data;
  data.screen = (Vector2Int){1280, 720};

  InitWindow(data.screen.x, data.screen.y, "asdf");

  Camera2D camera = {0};
  camera.zoom = 10;
  
  SetTargetFPS(60);

  Rectangle rec;
  rec.x = 10;
  rec.y = 10;
  rec.width = 10;
  rec.height = 10;
  Sprite sprite = CreateSprite(rec, NULL);

  while (!WindowShouldClose()) {

    // update here
    HandleSpriteInput(&sprite, 1, 1);
    FollowSprite(sprite, &camera, data);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    // draw here
    DrawText("HELLO WORLD!", 190, 200, 20, LIGHTGRAY);
    DrawSprite(sprite);

    EndDrawing();
  }

  CloseWindow();
}
