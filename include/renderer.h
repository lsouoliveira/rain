#pragma once

#include <algorithm>
#include <array>
#include <earcut.hpp>
#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include <vector>

namespace Rain {
using Coord = double;
using N = uint32_t;
using Point = std::array<Coord, 2>;

static void DrawTexturePoly(Texture2D texture, Vector2 center, Vector2 *points,
                            Vector2 *texcoords, int pointCount, Color tint) {
  rlSetTexture(texture.id);

  // Texturing is only supported on RL_QUADS
  rlBegin(RL_QUADS);

  rlColor4ub(tint.r, tint.g, tint.b, tint.a);

  for (int i = 1; i < pointCount; i++) {
    rlTexCoord2f(0.5f, 0.5f);
    rlVertex2f(points[i - 1].x + center.x, points[i - 1].y + center.y);

    rlTexCoord2f(texcoords[i].x, texcoords[i].y);
    rlVertex2f(points[i].x + center.x, points[i].y + center.y);

    rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
    rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);

    rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
    rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);
  }
  rlEnd();

  rlSetTexture(0);
}

static void DrawPolygon(Texture2D texture, Vector2 center, Vector2 *points,
                        Vector2 *texcoords, int pointCount, Color tint) {
  std::vector<std::vector<Point>> polygon;
  std::vector<Point> outer;

  for (int i = 0; i < pointCount; i++) {
    outer.push_back({points[i].x, points[i].y});
  }

  polygon.push_back(outer);

  std::vector<N> indices = mapbox::earcut<N>(polygon);

  rlSetTexture(texture.id);

  rlBegin(RL_TRIANGLES);

  rlColor4ub(tint.r, tint.g, tint.b, tint.a);

  std::reverse(indices.begin(), indices.end());

  for (int i = 0; i < indices.size(); i += 3) {
    rlTexCoord2f(texcoords[indices[i]].x, texcoords[indices[i]].y);
    rlVertex2f(points[indices[i]].x + center.x,
               points[indices[i]].y + center.y);

    rlTexCoord2f(texcoords[indices[i + 1]].x, texcoords[indices[i + 1]].y);
    rlVertex2f(points[indices[i + 1]].x + center.x,
               points[indices[i + 1]].y + center.y);

    rlTexCoord2f(texcoords[indices[i + 2]].x, texcoords[indices[i + 2]].y);
    rlVertex2f(points[indices[i + 2]].x + center.x,
               points[indices[i + 2]].y + center.y);
  }

  rlEnd();
}
}; // namespace Rain