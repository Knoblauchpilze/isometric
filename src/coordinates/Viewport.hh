#ifndef    VIEWPORT_HH
# define   VIEWPORT_HH

# include <maths_utils/Vector2.hh>
# include "olcEngine.hh"

namespace pge::coordinates {

  /// @brief - The definition mode for a viewport.
  enum class ViewportMode {
    BOTTOM_LEFT_BASED,
    TOP_LEFT_BASED
  };

  /// @brief - Defines a viewport from its top left corner and the
  /// associated dimensions.
  template <typename Coordinate>
  class Viewport {
    private:
      using Vector = olc::v2d_generic<Coordinate>;

    public:

      /// @brief - Create a viewport with a bottom left based mode.
      Viewport(const Vector& bl,
               const Vector& dims) noexcept;

      /// @brief - Create a viewport with the corresponding definition
      /// mode.
      Viewport(const Vector& corner,
               const Vector& dims,
               const ViewportMode& mode) noexcept;

      Vector
      primaryCorner() const noexcept;

      Vector
      bottomLeft() const noexcept;

      Vector
      bottomRight() const noexcept;

      Vector
      topRight() const noexcept;

      Vector
      topLeft() const noexcept;

      const Vector&
      dims() const noexcept;

      /// @brief - Moves the reference corner of this viewport (according
      /// to the view mode) to the new position.
      void
      move(const Vector& corner) noexcept;

      void
      scale(const Coordinate sx, const Coordinate sy) noexcept;

      /**
       * @brief - Whether or not a position with the specified radius
       *          is at least partially visible based on the viewport
       *          dimensions.
       * @param x - the abscissa of the point to check.
       * @param y - the ordinate of the point to check.
       * @param radius - the radius of the element.
       * @return - `true` if the element is at least partially visible.
       */
      bool
      visible(const Coordinate x,
              const Coordinate y,
              const Coordinate radius = Coordinate(1)) const noexcept;

      /**
       * @brief - Similar method to the above but handles position as
       *          a floating point position and the radius as a
       *          rectangular-ish shape.
       * @param p - the position to check.
       * @param sz - the dimensions of the element.
       * @return - `true` if the element is at least partially visible.
       */
      bool
      visible(const Vector& p,
              const Vector& sz = Vector(Coordinate(1), Coordinate(1))) const noexcept;

    private:

      bool
      visible(const Coordinate& x,
              const Coordinate& y,
              const Coordinate& sx,
              const Coordinate& sy) const noexcept;

    private:

      /// @brief - The mode defining how to interpret the corner used
      /// as a definition for the viewport.
      ViewportMode m_mode;

      /// @brief - Defines the principal corner of the viewport.
      Vector m_corner;

      /// @brief - Represents the dimensions of the view window along
      /// each axis.
      Vector m_dims;
  };

  using ViewportI = Viewport<int>;
  using ViewportF = Viewport<float>;
}

# include "Viewport.hxx"

#endif    /* VIEWPORT_HH */
