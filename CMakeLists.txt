cmake_minimum_required(VERSION 3.21)
project(game-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET
  allegro-5
  allegro_acodec-5
  allegro_audio-5
  allegro_font-5
  allegro_image-5
)
find_package(nlohmann_json 3 REQUIRED)

add_executable(game-engine
  src/main.cpp
  src/config/Config.cpp
  src/display/DisplayTools.cpp
  src/engine/Game.cpp
  src/engine/Map.cpp
  src/engine/Sprite.cpp
  src/engine/Structs.cpp
  src/engine/animations/Animation.cpp
  src/engine/animations/Animator.cpp
  src/engine/animations/Film.cpp
  src/engine/animations/SystemClock.cpp
  src/engine/physics/Gravity.cpp
  src/engine/physics/MotionQuantizer.cpp
  src/game/App.cpp
  src/game/Audio.cpp
  src/handlers/AIHandler.cpp
  src/handlers/AnimationHandler.cpp
  src/handlers/CollisionsHandler.cpp
  src/handlers/InputHandler.cpp
  src/handlers/PhysicsHandler.cpp
  src/handlers/RenderHandler.cpp
  src/tools/csv_parser.cpp
)
target_include_directories(game-engine PRIVATE include)
target_link_libraries(game-engine PRIVATE PkgConfig::ALLEGRO nlohmann_json::nlohmann_json)
