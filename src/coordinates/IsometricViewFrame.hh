#ifndef    ISOMETRICVIEW_FRAME_HH
# define   ISOMETRICVIEW_FRAME_HH

# include "Frame.hh"

namespace pge::coordinates {

  class IsometricViewFrame: public Frame {
    public:

      IsometricViewFrame(const IViewport& tiles,
                         const IViewport& pixels);

      virtual ~IsometricViewFrame() = default;

      /// @brief - Implementation of the interface method.
      olc::vf2d
      tileCoordsToPixels(float cx, float cy) const noexcept override;

      /// @brief - Implementation of the interface method.
      olc::vi2d
      pixelCoordsToTiles(float px,
                         float py,
                         olc::vf2d* intraTile = nullptr) const noexcept override;

    private:

      olc::vf2d
      coordinateFrameChange(const float x,
                            bool invertX,
                            const float y,
                            bool invertY,
                            const IViewport& source,
                            const IViewport& dest) const noexcept;
  };

}

# include "IsometricViewFrame.hxx"

#endif    /* ISOMETRICVIEW_FRAME_HH */
