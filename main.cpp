#include "raylib.h"
#include <box2d/box2d.h>

// Pixels per meter
const float PPM = 100.f;

int main()
{
  // Init raylib
  int screenWidth = 1024;
  int screenHeight = 640;
  float scale = 100.f;
  InitWindow(screenWidth, screenHeight, "test-ray-physics");
  SetTargetFPS(60);

  Rectangle rGroundBody, rDynamicBody;

  // Init physics
  b2Vec2 gravity(0.0f, 10.0f);
  b2World world(gravity);

  // Ground body
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set((screenWidth / 2.f) / PPM, ((screenHeight / 2.f) + 200.f) / PPM);
  rGroundBody.x = groundBodyDef.position.x * PPM;
  rGroundBody.y = groundBodyDef.position.y * PPM;
  b2Body* groundBody = world.CreateBody(&groundBodyDef);

  float groundBoxWidth = 500.f;
  float groundBoxHeight = 100.f;
  b2PolygonShape groundBox;
  groundBox.SetAsBox((groundBoxWidth / 2.f) / PPM, (groundBoxHeight / 2.f) / PPM);
  rGroundBody.width = groundBoxWidth;
  rGroundBody.height = groundBoxHeight;

  groundBody->CreateFixture(&groundBox, 0.0f);

  // Dynamic body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  float dropHeight = screenHeight / 4.f;
  bodyDef.position.Set((screenWidth / 2.f) / PPM, ((screenHeight / 2.f) - dropHeight) / PPM);
  rDynamicBody.x = bodyDef.position.x * PPM;
  rDynamicBody.y = bodyDef.position.y * PPM;
  b2Body* body = world.CreateBody(&bodyDef);

  float dynamicBodyWidth = 50.0f;
  float dynamicBodyHeight = 50.0f;
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox((dynamicBodyWidth / 2.0f) / PPM, (dynamicBodyHeight / 2.0f) / PPM);
  rDynamicBody.width = dynamicBodyWidth;
  rDynamicBody.height = dynamicBodyHeight;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  b2Fixture* fixture = body->CreateFixture(&fixtureDef);

  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  Vector2 origin = { 0 };
  float physicsToWorldScale = 100.f;
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
//    TraceLog(LOG_ERROR, "Mouse(%d, %d)", GetMouseX(), GetMouseY());

     // Process input
    if (IsKeyReleased(KEY_R))
    {
      body->DestroyFixture(fixture);
      world.DestroyBody(body);

      body = world.CreateBody(&bodyDef);
      fixture = body->CreateFixture(&fixtureDef);
    }

    // Physics update
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    rDynamicBody.x = position.x * PPM;
    rDynamicBody.y = position.y * PPM;

    // Render update
    origin = { rGroundBody.width / 2.f, rGroundBody.height / 2.f };
    DrawRectanglePro(rGroundBody, origin, 0.f, BLUE);
    origin = { rDynamicBody.width / 2.f, rDynamicBody.height / 2.f };
    DrawRectanglePro(rDynamicBody, origin, 0.f, BLACK);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
