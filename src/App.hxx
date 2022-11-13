#ifndef    APP_HXX
# define   APP_HXX

# include "App.hh"
# include "Action.hh"

namespace pge {

  inline
  void
  App::drawSprite(const SpriteDesc& t, const coordinates::Frame& cf) {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y);

    m_packs->draw(this, t.sprite, p, t.radius);
  }

  inline
  void
  App::drawRect(const SpriteDesc& t,
                const coordinates::Frame& cf)
  {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y);
    FillRectDecal(p, {t.radius, t.radius}, t.sprite.tint);
  }

}

#endif    /* APP_HXX */
