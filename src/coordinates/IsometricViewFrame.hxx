#ifndef    ISOMETRICVIEW_FRAME_HXX
# define   ISOMETRICVIEW_FRAME_HXX

# include "IsometricViewFrame.hh"

namespace pge::coordinates {

  inline
  IsometricViewFrame::IsometricViewFrame(const IViewport& tiles,
                                         const IViewport& pixels):
    Frame(tiles, pixels)
  {}

  inline
  olc::vf2d
  IsometricViewFrame::tileCoordsToPixels(float cx, float cy) const noexcept {
    auto out = coordinateFrameChange(cx, false, cy, true, m_tiles, m_pixels);
    // The offset accounts for the fact that the bottom tile is visible.
    // If we don't do that the effective viewport is in fact:
    // [m_tiles.bottomLeft().y + 1; m_tiles.topRight()] when it should be
    // [m_tiles.bottomLeft().y; m_tiles.topRight() - 1].
    return olc::vf2d{out.x, out.y - m_tilesToPixelsScale.y};
  }

  inline
  olc::vi2d
  IsometricViewFrame::pixelCoordsToTiles(float px,
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
  IsometricViewFrame::coordinateFrameChange(const float x,
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

#endif    /* ISOMETRICVIEW_FRAME_HXX */
