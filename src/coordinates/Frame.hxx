#ifndef    FRAME_HXX
# define   FRAME_HXX

# include "Frame.hh"
# include <cmath>

namespace pge::coordinates {

  inline
  Frame::Frame(const IViewport& tiles,
               const IViewport& pixels):
    utils::CoreObject("frame"),

    m_tiles(tiles),
    m_pixels(pixels),

    m_tilesToPixelsScale(1.0f, 1.0f),

    m_translationOrigin(),
    m_cachedPOrigin()
  {
    setService("coordinate");
    updateScale();
  }

  inline
  olc::vf2d
  Frame::tilesToPixels() const noexcept {
    return m_tilesToPixelsScale;
  }

  inline
  Frame::IViewport
  Frame::cellsViewport() const noexcept {
    olc::vi2d tl = pixelCoordsToTiles(0, 0);
    const auto dims = m_pixels.dims();
    olc::vi2d br = pixelCoordsToTiles(dims.x, dims.y);

    const auto out = IViewport(
      olc::vf2d(tl.x, br.y),
      // The `+1` comes from the fact that the `br.x - tl.x` accounts for
      // the number of tiles in difference but does not count the actual
      // `tl.x` tile.
      olc::vf2d(
        br.x - tl.x + 1.0f,
        tl.y - br.y + 1.0f
      )
    );

    return out;
  }

  inline
  olc::vf2d
  Frame::tileCoordsToPixels(const olc::vf2d pos, const TileLocation& location) const noexcept {
    return tileCoordsToPixels(pos.x, pos.y, location);
  }

  inline
  olc::vi2d
  Frame::pixelCoordsToTiles(const olc::vf2d pos) const noexcept {
    return pixelCoordsToTiles(pos);
  }

  inline
  void
  Frame::zoomIn(const olc::vf2d& pos,
                const float factor)
  {
    zoom(pos, factor);
  }

  inline
  void
  Frame::zoomOut(const olc::vf2d& pos,
                 const float factor)
  {
    zoom(pos, factor);
  }

  inline
  void
  Frame::beginTranslation(const olc::vf2d& origin) {
    m_translationOrigin = origin;
    m_cachedPOrigin = m_pixels.primaryCorner();
  }

  inline
  void
  Frame::translate(const olc::vf2d& pos) {
    // We need to deduce the translation added by the input `pos`
    // assuming that this will be the final position of the viewport.
    olc::vf2d translation = pos - m_translationOrigin;
    m_pixels.move(m_cachedPOrigin + translation);
  }

  inline
  void
  Frame::updateScale() {
    m_tilesToPixelsScale = m_pixels.dims() / m_tiles.dims();

    log(
      "1 tile = " + m_tilesToPixelsScale.str() + " pixel(s)",
      utils::Level::Debug
    );
  }

  inline
  void
  Frame::zoom(const olc::vf2d& pos, const float factor) {
    // We want to zoom on the `pos` position by a factor in input.
    // The invariant is that each corner of the pixels' viewport
    // corresponds to the respective corner of the tiles' viewport.
    // In order to zoom, we only have to zoom in one of the viewport.
    // The mapping between both will automatically make the smaller
    // or larger other viewport match the same space.
    // As the pixels' viewport doesn't change, there's no reason for
    // it to be modified, so we focus on the tiles' viewport.
    // The idea is to compute the distance between the input position
    // and one corner, and then scale this value by the `factor`: it
    // will guarantee that we get the same position for the initial
    // position as it is propostionnally at the same position from
    // the corner of the viewport.
    // As we get a position in pixels, we have to convert in tiles
    // before performing the computation.

    olc::vf2d intra;
    auto tiles = pixelCoordsToTiles(pos.x, pos.y, &intra);
    olc::vf2d tPos(tiles.x + intra.x, tiles.y + intra.y);
    olc::vf2d d = m_tiles.bottomLeft() - tPos;

    d *= factor;
    m_tiles.move(tPos + d);
    m_tiles.scale(factor, factor);

    updateScale();
  }

}

#endif    /* COORDINATE_FRAME_HXX */
