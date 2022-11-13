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

    log("tl: " + tl.str() + ", br: " + br.str());

    const auto out = IViewport(
      olc::vf2d(br.x, tl.y),
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
    m_cachedPOrigin = m_pixels.bottomLeft();
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
    // What we know is that the top left cell of the viewport is at the
    // top left corner of the pixels viewport.
    // We can compute the distance from the `pos` to the pixels viewport
    // origin: it should be applied the *inverse* of the `factor`
    // and we should be good to go: indeed if a viewport has its span
    // reduced, distances are lengthened (and conversely).
    olc::vf2d d = m_pixels.bottomLeft() - pos;
    d /= factor;

    m_pixels.move(pos + d);

    // Also update the dimensions of the tiles viewport by `factor`.
    m_tiles.scale(factor, factor);

    updateScale();
  }

}

#endif    /* COORDINATE_FRAME_HXX */
