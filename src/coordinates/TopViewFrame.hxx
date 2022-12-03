#ifndef    TOPVIEW_FRAME_HXX
# define   TOPVIEW_FRAME_HXX

# include "TopViewFrame.hh"

namespace pge::coordinates {

  inline
  TopViewFrame::TopViewFrame(const IViewport& tiles,
                             const IViewport& pixels):
    Frame(tiles, pixels)
  {}

  inline
  olc::vf2d
  TopViewFrame::tileCoordsToPixels(float cx,
                                   float cy,
                                   const TileLocation& location) const noexcept
  {
    auto out = coordinateFrameChange(cx, false, cy, true, m_tiles, m_pixels);
    // The offset accounts for the fact that the bottom tile is visible.
    // If we don't do that the effective viewport is in fact:
    // [m_tiles.bottomLeft().y + 1; m_tiles.topRight()] when it should be
    // [m_tiles.bottomLeft().y; m_tiles.topRight() - 1].
    olc::vf2d topLeft{out.x, out.y - m_tilesToPixelsScale.y};

    olc::vf2d ttp = tilesToPixels();
    switch (location) {
      case TileLocation::TopCenter:
        return olc::vf2d(topLeft.x + ttp.x / 2.0f, topLeft.y);
      case TileLocation::TopRight:
        return olc::vf2d(topLeft.x + ttp.x, topLeft.y);
      case TileLocation::RightCenter:
        return olc::vf2d(topLeft.x + ttp.x, topLeft.y + ttp.y / 2.0f);
      case TileLocation::BottomRight:
        return topLeft + tilesToPixels();
      case TileLocation::BottomCenter:
        return olc::vf2d(topLeft.x + ttp.x / 2.0f, topLeft.y + ttp.y);
      case TileLocation::BottomLeft:
        return olc::vf2d(topLeft.x, topLeft.y + ttp.y);
      case TileLocation::LeftCenter:
        return olc::vf2d(topLeft.x, topLeft.y + ttp.y / 2.0f);
      case TileLocation::TopLeft:
      default:
        return topLeft;
    }
  }

  inline
  olc::vi2d
  TopViewFrame::pixelCoordsToTiles(float px,
                                   float py,
                                   olc::vf2d* intraTile) const noexcept
  {
    const auto tiles = coordinateFrameChange(px, false, py, true, m_pixels, m_tiles);
    const auto out = olc::vi2d{
      static_cast<int>(std::floor(tiles.x)),
      static_cast<int>(std::floor(tiles.y))
    };

    if (intraTile != nullptr) {
      intraTile->x = tiles.x - out.x;
      intraTile->y = tiles.y - out.y;
    }

    return out;
  }

  inline
  olc::vf2d
  TopViewFrame::coordinateFrameChange(const float x,
                                      bool invertX,
                                      const float y,
                                      bool invertY,
                                      const IViewport& source,
                                      const IViewport& dest) const noexcept
  {
    auto tx = x;
    if (invertX) {
      tx = source.bottomLeft().x - tx;
    }
    else {
      tx -= source.bottomLeft().x;
    }
    tx /= source.dims().x;
    tx *= dest.dims().x;
    tx += dest.bottomLeft().x;

    auto ty = y;
    if (invertY) {
      ty = source.bottomLeft().y - ty;
    }
    else {
      ty -= source.bottomLeft().y;
    }
    ty /= source.dims().y;
    ty *= dest.dims().y;
    ty += dest.bottomLeft().y;

    return olc::vf2d(tx, ty);
  }

}

#endif    /* TOPVIEW_FRAME_HXX */
