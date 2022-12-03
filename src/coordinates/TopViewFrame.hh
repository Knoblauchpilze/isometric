#ifndef    TOPVIEW_FRAME_HH
# define   TOPVIEW_FRAME_HH

# include "Frame.hh"

namespace pge::coordinates {

  class TopViewFrame: public Frame {
    public:

      TopViewFrame(const IViewport& tiles,
                   const IViewport& pixels);

      virtual ~TopViewFrame() = default;

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

      olc::vf2d
      coordinateFrameChange(const float x,
                            bool invertX,
                            const float y,
                            bool invertY,
                            const IViewport& source,
                            const IViewport& dest) const noexcept;
  };

}

# include "TopViewFrame.hxx"

#endif    /* TOPVIEW_FRAME_HH */
