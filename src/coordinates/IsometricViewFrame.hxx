#ifndef    ISOMETRICVIEW_FRAME_HXX
# define   ISOMETRICVIEW_FRAME_HXX

# include <numbers>
# include "IsometricViewFrame.hh"
# include <iostream>

namespace pge::coordinates {

  inline
  IsometricViewFrame::IsometricViewFrame(const IViewport& tiles,
                                         const IViewport& pixels):
    Frame(tiles, pixels),

    m_pixToWorldMat(Mat2f::Identity())
  {
    generateMatrices();
  }

  inline
  olc::vf2d
  IsometricViewFrame::tileCoordsToPixels(float cx,
                                         float cy,
                                         const TileLocation& location) const noexcept
  {
    switch (location) {
      case TileLocation::TopCenter:
        return tileCoordsToPixels(cx + 0.5f, cy);
      case TileLocation::TopRight:
        return tileCoordsToPixels(cx + 1.0f, cy);
      case TileLocation::RightCenter:
        return tileCoordsToPixels(cx + 1.0f, cy + 0.5f);
      case TileLocation::BottomRight:
        return tileCoordsToPixels(cx + 1.0f, cy + 1.0f);
      case TileLocation::BottomCenter:
        return tileCoordsToPixels(cx + 0.5f, cy + 1.0f);
      case TileLocation::BottomLeft:
        return tileCoordsToPixels(cx, cy + 1.0f);
      case TileLocation::LeftCenter:
        return tileCoordsToPixels(cx, cy + 0.5f);
      case TileLocation::TopLeft:
      default:
        break;
    }

    auto out = coordinateFrameChange(cx, false, cy, true, m_tiles, m_worldToPixMat, m_pixels);
    // The offset accounts for the fact that the bottom tile is visible.
    // If we don't do that the effective viewport is in fact:
    // [m_tiles.bottomLeft().y + 1; m_tiles.topRight()] when it should be
    // [m_tiles.bottomLeft().y; m_tiles.topRight() - 1].
    return olc::vf2d{out.x, out.y};
  }

  inline
  olc::vi2d
  IsometricViewFrame::pixelCoordsToTiles(float px,
                                         float py,
                                         olc::vf2d* intraTile) const noexcept
  {
    const auto tiles = coordinateFrameChange(px, false, py, true, m_pixels, m_pixToWorldMat, m_tiles);
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

  void
  IsometricViewFrame::generateMatrices() {
    // See this link:
    // https://gamedev.stackexchange.com/questions/31113/how-can-i-improve-my-isometric-tile-picking-algorithm
    constexpr auto alpha = -std::numbers::pi / 40.0f;

    Mat2f rotation;
    rotation << std::cos(alpha), -std::sin(alpha),
                std::sin(alpha), std::cos(alpha);

    // Mat2f scale = Eigen::Scaling(1.0f, 0.5f);
    Mat2f scale = Eigen::Scaling(1.0f, 1.0f);

    m_pixToWorldMat = rotation * scale;

    m_worldToPixMat = m_pixToWorldMat.inverse();

    std::cout << "pixToWorld: " << std::endl << m_pixToWorldMat << std::endl;
    std::cout << "worldToPix: " << std::endl << m_worldToPixMat << std::endl;
  }

  inline
  olc::vf2d
  IsometricViewFrame::coordinateFrameChange(const float x,
                                            bool invertX,
                                            const float y,
                                            bool invertY,
                                            const IViewport& source,
                                            const Mat2f& transform,
                                            const IViewport& dest) const noexcept
  {
    // Convert to normalized coordinate space from the source.
    auto tx = x;
    if (invertX) {
      tx = source.bottomLeft().x - tx;
    }
    else {
      tx -= source.bottomLeft().x;
    }
    tx /= source.dims().x;

    auto ty = y;
    if (invertY) {
      ty = source.bottomLeft().y - ty;
    }
    else {
      ty -= source.bottomLeft().y;
    }
    ty /= source.dims().y;

    // Apply the coordinate frame transform.
    Vec2f v(tx, ty);
    Vec2f tr = transform * v;
    tx = tr(0);
    ty = tr(1);

    // Convert back to the destination viewport.
    tx *= dest.dims().x;
    tx += dest.bottomLeft().x;

    ty *= dest.dims().y;
    ty += dest.bottomLeft().y;

    return olc::vf2d(tx, ty);
  }

}

#endif    /* ISOMETRICVIEW_FRAME_HXX */
