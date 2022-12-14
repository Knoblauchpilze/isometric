
/// @brief - Exploration of a simple isometric app renderer.

# include <core_utils/StdLogger.hh>
# include <core_utils/PrefixedLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "AppDesc.hh"
# include "TopViewFrame.hh"
# include "IsometricViewFrame.hh"
# include "App.hh"

using namespace pge::coordinates;

int
main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger raw;
  raw.setLevel(utils::Level::Debug);
  utils::PrefixedLogger logger("pge", "main");
  utils::LoggerLocator::provide(&raw);

  try {
    logger.logMessage(utils::Level::Notice, "Starting application");

    auto tiles = ViewportF(
      //olc::vf2d(-6.0f, -5.0f),
      olc::vf2d(0.0f, 0.0f),
      olc::vf2d(20.0f, 15.0f)
    );
    auto pixels = ViewportF(
      //olc::vf2d(10.0f, 50.0f),
      olc::vf2d(0.0f, 0.0f),
      olc::vf2d(800.0f, 600.0f),
      ViewportMode::TOP_LEFT_BASED
    );

    auto cf = std::make_shared<IsometricViewFrame>(tiles, pixels);
    // auto cf = std::make_shared<TopViewFrame>(tiles, pixels);
    pge::AppDesc ad = pge::newDesc(olc::vi2d(800, 600), cf, "isometric");
    pge::App demo(ad);

    demo.Start();
  }
  catch (const utils::CoreException& e) {
    logger.logError(utils::Level::Critical, "Caught internal exception while setting up application", e.what());
  }
  catch (const std::exception& e) {
    logger.logError(utils::Level::Critical, "Caught internal exception while setting up application", e.what());
  }
  catch (...) {
    logger.logMessage(utils::Level::Critical, "Unexpected error while setting up application");
  }

  return EXIT_SUCCESS;
}
