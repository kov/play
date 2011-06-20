#include <Box2D.h>

int main()
{
  b2AABB worldAABB;
  b2Vec2 gravity(0.0f, -10.0f);

  worldAABB.lowerBound.Set(-100.0f, -100.0f);
  worldAABB.upperBound.Set(100.0f, 100.0f);

  return 0;
}
