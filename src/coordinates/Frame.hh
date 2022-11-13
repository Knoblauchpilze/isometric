#ifndef    FRAME_HH
# define   FRAME_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "Viewport.hh"

namespace pge::coordinates {

  class Frame: public utils::CoreObject {
    public:
      using IViewport = Viewport<float>;

      Frame(const IViewport& cells,
            const IViewport& pixels/*,
            const olc::vf2d& tileSize*/);

      virtual ~Frame() = default;

      /// @brief - Used to return the current scaling factor to apply
      /// to the tile compared to their initial size.
      /// @return - the scaling factor to apply to the tiles.
      // const olc::vf2d&
      // tileScale() const noexcept;

      /// @brief - Returns the actual size of the tile by applying the
      /// current scaling factor (as returned by `tileScale`) to the
      /// initial tile size.
      /// @return - the size of the tile in pixels.
      olc::vf2d
      tilesToPixels() const noexcept;

      /// @brief - Return the current viewport expressed in cells. This
      /// interface should be specialized by inheriting classes to account
      /// for the precise layout of the coordinates system they define.
      /// @return - the viewport of this coordinate frame in cells.
      virtual IViewport
      cellsViewport() const noexcept = 0;

      /// @brief - Used to convert from tile coordinates to pixel
      /// coordinates. This method can be used when some tile is to be
      /// displayed on the screen. We make use of a global position of
      /// the viewport to be able to correctly interpret the input cell.
      /// It is also dependent on the current scaling for tiles based on
      /// the cells viewport. This method should be redefined by inheriting
      /// classes for their specific purposes.
      /// @param x - the cell coordinate along the `x` axis.
      /// @param y - the cell coordinate along the `y` axis.
      /// @return - the coordinates in pixels of the tile defined by the
      /// input coords.
      virtual olc::vf2d
      tileCoordsToPixels(float x, float y) const noexcept = 0;

      /// @brief - Convert from pixels coordinates to tile coords. Some
      /// extra logic is added in order to account for the tiles that do
      /// not align with the grid so that we always get an accurate
      /// position for the tile.
      /// This method should be redefined by inheriting classes for their
      /// specific purposes.
      /// @param px - the abscissa of the pixel coordinates.
      /// @param py - the ordinate of the pixel coordinates.
      /// @param intraTile - used to provide the intra tile coords if a non
      /// `null` value is provided. The coordinates are expressed in the
      /// range `[0; 1]` (as in a percentage).
      /// @return - the corresponding tile coordinates.
      virtual olc::vi2d
      pixelCoordsToTiles(float px,
                         float py,
                         olc::vf2d* intraTile = nullptr) const noexcept = 0;

      /// @brief - Used to perform the zoom operation requested. The
      /// viewports will be updated to be smaller than the current value
      /// so as to actually simulate a zoom.
      /// @param pos - the position around which the zoom operation should
      /// be performed (typically the current position of the mouse).
      /// The final position will stay the same on screen.
      /// @param factor - the factor to use to zoom in: defines the scaling
      /// which will be applied when comparing the viewport before and after
      /// the zoom operation.
      void
      zoomIn(const olc::vf2d& pos,
             const float factor = 0.5f);

      /// @brief - Reverse operation to `zoomIn`.
      /// @param pos - the position which should stay constant after the
      /// zoom out operation.
      /// @param factor - the factor to use to zoom out: defines the scaling
      /// which will be applied when comparing the viewport before and after
      /// the zoom operation.
      void
      zoomOut(const olc::vf2d& pos,
              const float factor = 2.0f);

      /// @brief - Notify the coordinate frames to start a translation.
      /// This mainly concerns the pixel viewport's origin: the goal is
      /// to fix the current top left corner so that it can be updated
      /// at the end of the translation.
      /// @param origin - allows to keep track of the translation so that
      /// we can define the transformation to be applied to the pixel
      /// viewport.
      void
      beginTranslation(const olc::vf2d& origin);

      /// @brief - In case a translation has begun indicates that the new
      /// position to track is the input parameter. We will interpret it
      /// internally in regard to the value of the cached viewport origin
      /// and deduce the actual translation.
      /// @param pos - the current final position of the translation.
      void
      translate(const olc::vf2d& pos);

    private:

      void
      updateScale();

      /// @brief - General zoom method which takes a factor that will
      /// be applied to the viewport (a value that is larger than `1`
      /// will zoom out while a zoom in will be performed with a value
      /// smaller than `1`) and the center to fix.
      /// @param pos - the position to fix in the zoom operation.
      /// @param factor - a value to reduce/increase the size of the
      /// viewports.
      void
      zoom(const olc::vf2d& pos, const float factor);

    protected:

      /// @brief - Define the viewport for this coordinate frame. It
      /// represent the area that is visible for now given the position
      /// of the camera. The viewport is expressed in cells and defined
      /// through its top left corner and dimensions.
      IViewport m_cells;

      /// @brief - Define a similar element but for the pixels on screen.
      /// It is used to represent the pixels that can be displayed on the
      /// device.
      /// Computing the ratio between both viewport gives the size of a
      /// tile in pixels and we can deduce a scaling factor from the initial
      /// size of a tile.
      IViewport m_pixels;

      /// @brief - Define the initial size for a tile as described from
      /// the loaded sprite file. This size is used as a base to compute
      /// the scale from the values of the viewports.
      // olc::vf2d m_ts;

      /// @brief - A value to scale from tiles value to pixels value.
      olc::vf2d m_tilesToPixelsScale;

      /// @brief - The size of the tiles once scaled: convenience value
      /// to cache the result of `m_ts * m_scale` as it is used quite
      /// often.
      // olc::vf2d m_tScaled;

      /// @brief - The origin of the translation (i.e. the pixels position
      /// when it started). Allows to compute the accumulated transform to
      /// apply to the wo.
      olc::vf2d m_translationOrigin;

      /// @brief - Cached position of the top left corner of the pixels
      /// viewport when starting the translation. Once the translation is
      /// performed we are able to update the viewport accordingly.
      olc::vf2d m_cachedPOrigin;
  };

  using FrameShPtr = std::shared_ptr<Frame>;
}

# include "Frame.hxx"

#endif    /* FRAME_HH */
