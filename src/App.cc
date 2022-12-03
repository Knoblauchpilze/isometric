
# include "App.hh"
# include "TopViewFrame.hh"
# include "IsometricViewFrame.hh"

// # define SQUARES

namespace {
# ifdef SQUARES
  olc::Pixel
  colorFromCoord(const int x, const int y) {
    constexpr auto size = 4;
    static const auto colors = std::vector<olc::Pixel>{
      olc::VERY_DARK_RED,
      olc::VERY_DARK_BLUE,
      olc::VERY_DARK_GREEN,
      olc::VERY_DARK_YELLOW,
      olc::VERY_DARK_MAGENTA,
      olc::VERY_DARK_CYAN,
      olc::VERY_DARK_GREY,
      olc::WHITE,
      olc::VERY_DARK_ORANGE,
      olc::VERY_DARK_APPLE_GREEN,
      olc::VERY_DARK_COBALT_BLUE,
      olc::VERY_DARK_PURPLE,
      olc::VERY_DARK_PINK,
      olc::VERY_DARK_BROWN,
      olc::VERY_DARK_CORNFLOWER_BLUE,
      olc::VERY_DARK_BIDOOF
    };

    if (x < -10 || x > 15) {
      return olc::BLACK;
    }
    if (y < -20 || y > 3) {
      return olc::VERY_DARK_GREY;
    }

    return colors[((y % size) * 4 + (x % size)) % colors.size()];
  }
# endif
}

namespace pge {

  App::App(const AppDesc& desc):
    PGEApp(desc),

    m_game(nullptr),
    m_state(nullptr),
    m_menus(),

    m_packs(std::make_shared<TexturePack>()),
    m_planetPackID(),

    m_isometric(true)
  {}

  bool
  App::onFrame(float fElapsed) {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return false;
    }

    if (!m_game->step(fElapsed)) {
      info("This is game over");
    }

    return m_game->terminated();
  }

  void
  App::onInputs(const controls::State& c,
                const coordinates::Frame& cf)
  {
    // Handle case where no game is defined.
    if (m_game == nullptr) {
      return;
    }

    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    if (m_state != nullptr) {
      menu::InputHandle ih = m_state->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(c.mPosX, c.mPosY, &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[controls::keys::P]) {
      m_game->togglePause();
    }

    if (c.buttons[controls::mouse::Middle] == controls::ButtonState::Released) {
      auto tiles = coordinates::ViewportF(olc::vf2d(-6.0f, -5.0f), olc::vf2d(20.0f, 15.0f));
      auto pixels = coordinates::ViewportF(olc::vf2d(10.0f, 50.0f), olc::vf2d(800.0f, 600.0f), coordinates::ViewportMode::TOP_LEFT_BASED);

      coordinates::FrameShPtr cf;
      if (m_isometric) {
        cf = std::make_shared<coordinates::TopViewFrame>(tiles, pixels);
      }
      else {
        cf = std::make_shared<coordinates::IsometricViewFrame>(tiles, pixels);
      }

      setCoordinateFrame(cf);
      m_isometric = !m_isometric;
    }
  }

  void
  App::loadData() {
    // Create the game and its state.
    m_game = std::make_shared<Game>();
  }

  void
  App::loadResources() {
    // Assign a specific tint to the regular drawing layer so that we
    // have a built in transparency.
    // We can't do it directly when drawing in the rendering function
    // because as the whole layer will be drawn as one quad in opengl
    // with an opaque alpha, we will lose this info.
    // This means that everything is indeed transparent but that's the
    // only way for now to achieve it.
    setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));

    pge::sprites::Pack pack;
    pack.file = "data/img/planet.png";
    constexpr auto TILE_SIZE = 1000;
    pack.sSize = olc::vi2d(TILE_SIZE, TILE_SIZE);
    pack.layout = olc::vi2d(1, 1);

    m_planetPackID = m_packs->registerPack(pack);

    info("Load app resources in the 'm_packs' attribute");
  }

  void
  App::loadMenuResources() {
    // Generate the game state.
    m_state = std::make_shared<GameState>(
      olc::vi2d(ScreenWidth(), ScreenHeight()),
      Screen::Game
    );

    m_menus = m_game->generateMenus(ScreenWidth(), ScreenHeight());
  }

  void
  App::cleanResources() {
    if (m_packs != nullptr) {
      m_packs.reset();
    }
  }

  void
  App::cleanMenuResources() {
    m_menus.clear();
  }

  void
# ifdef SQUARES
  App::drawDecal(const RenderDesc& res) {
# else
  App::drawDecal(const RenderDesc& /*res*/) {
# endif
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::VERY_DARK_GREY);

    // In case we're not in the game screen, do nothing.
    if (m_state->getScreen() != Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

# ifdef SQUARES
    const auto viewport = res.cf.cellsViewport();
    const auto bl = viewport.bottomLeft();
    const auto tr = viewport.topRight();

    for (int y = bl.y ; y < tr.y ; ++y) {
      for (int x = bl.x ; x < tr.x ; ++x) {
        const auto c = colorFromCoord(x, y);
        const auto pos = res.cf.tileCoordsToPixels(x, y);
        const auto scale = res.cf.tilesToPixels();

        FillRectDecal(pos, scale, c);
      }
    }
# endif

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::draw(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawUI(const RenderDesc& /*res*/) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    // Render the game menus.
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id]->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  App::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in game mode, just render
    // the state.
    if (m_state->getScreen() != Screen::Game) {
      m_state->render(this);
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    // Draw cursor's position.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp.x, mp.y, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    // const auto pos = res.cf.tileCoordsToPixels(mtp.x, mtp.y);
    // FillRectDecal(pos, res.cf.tilesToPixels(), olc::Pixel(255, 255, 0, alpha::SemiOpaque));

    constexpr auto PLANET_SCALE = 0.05f;
    DrawDecal(
      mp,
      m_packs->getDecalForPack(m_planetPackID),
      olc::vf2d(PLANET_SCALE, PLANET_SCALE),
      olc::ORANGE
    );

# ifdef CANONICAL_SPRITE_EXAMPLE
    sd.x = x;
    sd.y = y;

    sd.sprite.pack = m_planetPackID;
    sd.sprite.id = 0;
    sd.sprite.tint = olc::WHITE;
    sd.sprite.sprite = olc::vi2d(0, 0);

    drawSprite(sd, res.cf);
# endif

    SetPixelMode(olc::Pixel::NORMAL);
  }

}
