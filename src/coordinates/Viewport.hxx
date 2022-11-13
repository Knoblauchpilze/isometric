#ifndef    VIEWPORT_HXX
# define   VIEWPORT_HXX

# include "Viewport.hh"

namespace pge::coordinates {

  template <typename Coordinate>
  inline
  Viewport<Coordinate>::Viewport(const Vector& bl,
                                 const Vector& dims) noexcept:
    m_bl(bl),
    m_dims(dims),

    m_tr(m_bl + dims)
  {}

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::bottomLeft() const noexcept {
    return m_bl;
  }

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::topRight() const noexcept {
    return m_tr;
  }

  template <typename Coordinate>
  inline
  const typename Viewport<Coordinate>::Vector&
  Viewport<Coordinate>::dims() const noexcept {
    return m_dims;
  }

  template <typename Coordinate>
  inline
  void
  Viewport<Coordinate>::move(const Vector& bottomLeft) noexcept {
    m_bl = bottomLeft;

    m_tr = m_bl + m_dims;
  }

  template <typename Coordinate>
  inline
  void
  Viewport<Coordinate>::scale(const Coordinate sx, const Coordinate sy) noexcept {
    m_dims.x *= sx;
    m_dims.y *= sy;

    m_tr = m_bl + m_dims;
  }

  template <typename Coordinate>
  inline
  bool
  Viewport<Coordinate>::visible(const Coordinate x,
                                const Coordinate y,
                                const Coordinate radius) const noexcept
  {
    return visible(x, y, radius, radius);
  }

  template <typename Coordinate>
  inline
  bool
  Viewport<Coordinate>::visible(const Vector& p, const Vector& sz) const noexcept {
    return visible(p.x, p.y, sz.x, sz.y);
  }

  template <typename Coordinate>
  inline
  bool
  Viewport<Coordinate>::visible(const Coordinate& x,
                                const Coordinate& y,
                                const Coordinate& sx,
                                const Coordinate& sy) const noexcept
  {
    if (x + sx < m_bl.x || x - sx > m_tr.x) {
      return false;
    }

    if (y + sy < m_bl.y || y - sy > m_tr.y) {
      return false;
    }

    return true;
  }

}

#endif    /* VIEWPORT_HXX */
