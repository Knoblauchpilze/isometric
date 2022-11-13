#ifndef    FRAME_HXX
# define   FRAME_HXX

# include "Frame.hh"
# include <cmath>

namespace pge::coordinates {

  inline
  Frame::Frame(const IViewport& cells,
               const IViewport& pixels/*,
               const olc::vf2d& tileSize*/):
    utils::CoreObject("frame"),

    m_cells(cells),
    m_pixels(pixels),

    // m_ts(tileSize),
    m_tilesToPixelsScale(1.0f, 1.0f),
    // m_tScaled(m_ts),

    m_translationOrigin(),
    m_cachedPOrigin()
  {
    setService("coordinate");
    updateScale();
  }

  // template <typename Coordinate>
  // inline
  // const olc::vf2d&
  // Frame<Coordinate>::tileScale() const noexcept {
  //   return m_scale;
  // }

  inline
  olc::vf2d
  Frame::tilesToPixels() const noexcept {
    return m_tilesToPixelsScale;
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
    m_tilesToPixelsScale = m_pixels.dims() / m_cells.dims();

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

    // Also update the dimensions of the cells viewport by `factor`.
    m_cells.scale(factor, factor);

    updateScale();
  }

}

#endif    /* COORDINATE_FRAME_HXX */
