#ifndef    TOPVIEW_FRAME_HH
# define   TOPVIEW_FRAME_HH

# include "Frame.hh"

namespace pge::coordinates {

  class TopViewFrame: public Frame {
    public:

      TopViewFrame(const IViewport& cells,
                   const IViewport& pixels/*,
                   const olc::vi2d& tileSize*/);

      virtual ~TopViewFrame() = default;

      /// @brief - Implementation of the interface method.
      IViewport
      cellsViewport() const noexcept override;

      /// @brief - Implementation of the interface method.
      olc::vf2d
      tileCoordsToPixels(float x, float y) const noexcept override;

      /// @brief - Implementation of the interface method.
      olc::vi2d
      pixelCoordsToTiles(float px,
                         float py,
                         olc::vf2d* intraTile = nullptr) const noexcept override;
  };

}

# include "TopViewFrame.hxx"

#endif    /* TOPVIEW_FRAME_HH */
