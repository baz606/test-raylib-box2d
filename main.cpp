#include <iostream>
#include "raylib.h"
#include <box2d/box2d.h>

int main()
{
  std::cout << "Hello, World!" << std::endl;
  b2Vec2 gravity(0.0f, -10.0f);
  std::cout << gravity.Length() << std::endl;

  return 0;
}
