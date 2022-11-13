#ifndef    TOPVIEW_FRAME_HXX
# define   TOPVIEW_FRAME_HXX

# include "TopViewFrame.hh"

namespace pge::coordinates {

  inline
  TopViewFrame::TopViewFrame(const IViewport& cells,
                             const IViewport& pixels/*,
                             const olc::vi2d& tileSize*/):
    Frame(cells, pixels)
  {}

  inline
  TopViewFrame::IViewport
  TopViewFrame::cellsViewport() const noexcept {
    // We know the position of the origin point in pixels. We also know
    // the size of each cell in pixels.
    // From there we can convert the position `(0, 0)` in pixels to tiles.
    olc::vi2d tl = pixelCoordsToTiles(0, 0);
    const auto dims = m_pixels.dims();
    olc::vi2d br = pixelCoordsToTiles(dims.x, dims.y);

    IViewport out(
      tl,
      // The `+1` comes from the fact that the `br.x - tl.x` accounts for
      // the number of tiles in difference but does not count the actual
      // `tl.x` tile.
      olc::vf2d(br.x - tl.x + 1, br.y - tl.y + 1)
    );

    return out;
  }

  inline
  olc::vf2d
  TopViewFrame::tileCoordsToPixels(float x, float y) const noexcept {
    // Offset the input coordinates based on the current position of
    // the cell's viewport.
    auto p = olc::vf2d(x, y);

    p -= m_cells.bottomLeft();

    // Convert to top view coordinates: we just need to scale by the
    // tile size.
    return m_pixels.bottomLeft() + p * tilesToPixels();
  }

  inline
  olc::vi2d
  TopViewFrame::pixelCoordsToTiles(float px,
                                   float py,
                                   olc::vf2d* intraTile) const noexcept
  {
    const olc::vf2d p{px, py};
    const auto pOrigin = p - m_pixels.bottomLeft();

    const auto tilePos = pOrigin / tilesToPixels();

    // The viewport should be handled after we first computed coordinates
    // in the world's frame: it is what is meant by the cells' offset.
    // Note that as we allow decimal coordinates, we need to handle the
    // rounding.
    olc::vf2d offsetTilePos = tilePos + m_cells.bottomLeft();
    olc::vi2d roundedTile(
      static_cast<int>(std::floor(offsetTilePos.x)),
      static_cast<int>(std::floor(offsetTilePos.y))
    );

    log(
      "p: " + p.str() +
      ", tilePos: " + tilePos.str() +
      ", scale: " + tilesToPixels().str() +
      ", offsetTilePos: " + offsetTilePos.str() +
      ", roundedTile: " + roundedTile.str()
    );

    // Compute the intra-tile offset by converting the tile position
    // to pixels and comparing the input value.
    olc::vf2d tlp = tileCoordsToPixels(roundedTile.x, roundedTile.y);

    if (intraTile != nullptr) {
      *intraTile = p - tlp;

      // Convert the intra tile to value in the range `[0; 1]`.
      *intraTile = *intraTile / tilesToPixels();
    }

    return roundedTile;
  }

}

#endif    /* TOPVIEW_FRAME_HXX */
