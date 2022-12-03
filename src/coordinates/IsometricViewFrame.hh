#ifndef    ISOMETRICVIEW_FRAME_HH
# define   ISOMETRICVIEW_FRAME_HH

# include <eigen3/Eigen/Eigen>
# include "Frame.hh"

namespace pge::coordinates {

  class IsometricViewFrame: public Frame {
    public:

      IsometricViewFrame(const IViewport& tiles,
                         const IViewport& pixels);

      virtual ~IsometricViewFrame() = default;

      /// @brief - Implementation of the interface method.
      olc::vf2d
      tileCoordsToPixels(float cx,
                         float cy,
                         const TileLocation& location = TileLocation::TopLeft) const noexcept override;

      /// @brief - Implementation of the interface method.
      olc::vi2d
      pixelCoordsToTiles(float px,
                         float py,
                         olc::vf2d* intraTile = nullptr) const noexcept override;

    private:

      /// @brief - Convenience Eigen defines.
      using Mat2f = Eigen::Matrix2f;
      using Vec2f = Eigen::Vector2f;

      void
      generateMatrices();

      olc::vf2d
      coordinateFrameChange(const float x,
                            bool invertX,
                            const float y,
                            bool invertY,
                            const IViewport& source,
                            const Mat2f& transform,
                            const IViewport& dest) const noexcept;

    private:

      /// @brief - The transformation matrix allowing to move from the
      /// pixel coordinate space to the world coordinate space.
      Mat2f m_pixToWorldMat;

      /// @brief - The transformation matrix allowing to move from the
      /// world coordinate space to the pixel coordinate space. This is
      /// the inverse matrix of the previous one.
      Mat2f m_worldToPixMat;
  };

}

# include "IsometricViewFrame.hxx"

#endif    /* ISOMETRICVIEW_FRAME_HH */
