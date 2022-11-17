#ifndef    VIEWPORT_HXX
# define   VIEWPORT_HXX

# include "Viewport.hh"

namespace pge::coordinates {

  template <typename Coordinate>
  inline
  Viewport<Coordinate>::Viewport(const Vector& bl,
                                 const Vector& dims) noexcept:
    Viewport<Coordinate>::Viewport(bl, dims, ViewportMode::BOTTOM_LEFT_BASED)
  {}

  template <typename Coordinate>
  inline
  Viewport<Coordinate>::Viewport(const Vector& corner,
                                 const Vector& dims,
                                 const ViewportMode& mode) noexcept:
    m_mode(mode),

    m_corner(corner),
    m_dims(dims)
  {}

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::primaryCorner() const noexcept {
    return m_corner;
  }

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::bottomLeft() const noexcept {
    return m_mode == ViewportMode::BOTTOM_LEFT_BASED ?
      m_corner :
      Vector(m_corner.x, m_corner.y + m_dims.y)
    ;
  }

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::bottomRight() const noexcept {
    return m_mode == ViewportMode::TOP_LEFT_BASED ?
      m_corner + m_dims :
      Vector(m_corner.x + m_dims.x, m_corner.y)
    ;
  }

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::topRight() const noexcept {
    return m_mode == ViewportMode::BOTTOM_LEFT_BASED ?
      m_corner + m_dims :
      Vector(m_corner.x + m_dims.x, m_corner.y)
    ;
  }

  template <typename Coordinate>
  inline
  typename Viewport<Coordinate>::Vector
  Viewport<Coordinate>::topLeft() const noexcept {
    return m_mode == ViewportMode::TOP_LEFT_BASED ?
      m_corner :
      Vector(m_corner.x, m_corner.y + m_dims.y)
    ;
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
  Viewport<Coordinate>::move(const Vector& corner) noexcept {
    m_corner = corner;
  }

  template <typename Coordinate>
  inline
  void
  Viewport<Coordinate>::scale(const Coordinate sx, const Coordinate sy) noexcept {
    m_dims.x *= sx;
    m_dims.y *= sy;
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
    if (x + sx < m_corner.x || x - sx > m_corner.x + m_dims.x) {
      return false;
    }

    if (y + sy < m_corner.y || y - sy > m_corner.y + m_dims.y) {
      return false;
    }

    return true;
  }

}

#endif    /* VIEWPORT_HXX */
