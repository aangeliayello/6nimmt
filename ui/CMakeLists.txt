cmake_minimum_required(VERSION 3.16)
project(SechtNimmt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

# New code to check if SFML is already fetched
FetchContent_GetProperties(SFML)
if(NOT SFML_POPULATED) # Only fetch if SFML hasn't been fetched yet
    FetchContent_Populate(SFML)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

add_executable(SechtNimmt src/main.cpp src/globals.cpp src/assets.cpp src/graphics_handler.cpp src/card_texture.cpp ../bots/src/utils.cpp ../bots/src/game_factory.cpp ../bots/src/globals.cpp ../bots/src/core/board.cpp ../bots/src/core/card.cpp ../bots/src/core/move.cpp ../bots/src/core/game.cpp ../bots/src/core/player.cpp ../bots/src/engines/human_engine.cpp ../bots/src/engines/random_engine.cpp ../bots/src/engines/mc_engine.cpp ../bots/src/engines/mcts_engine.cpp)

target_link_libraries(SechtNimmt PRIVATE sfml-graphics)
target_compile_features(SechtNimmt PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET SechtNimmt
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:SechtNimmt>
        VERBATIM)
endif()

install(TARGETS SechtNimmt)
