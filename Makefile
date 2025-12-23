CXX      := g++-14
CXXFLAGS := -std=c++20 -fmodules-ts -Wall -g
LDFLAGS  := -lncurses
BUILD_DIR := build
MODULES=chrono thread vector utility memory optional string unordered_map algorithm tuple

.PHONY: all clean clean-ofiles
all: flappy space_invaders bounce



STD_MODULE_STAMP := gcm.cache/std-modules.stamp
std-modules: $(STD_MODULE_STAMP)

$(STD_MODULE_STAMP):
	mkdir -p gcm.cache
	$(CXX) $(CXXFLAGS) -c -x c++-system-header $(MODULES)
	touch $@

clock.o: clock.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

clock_impl.o: clock_impl.cc clock.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

shape.o: shape.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

abstract_collider.o: abstract_collider.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

ncurses_win.o: ncurses_win.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

ncurses_win_impl.o: ncurses_win_impl.cc ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

ncurses_scr.o: ncurses_scr.cc ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

ncurses_scr_impl.o: ncurses_scr_impl.cc ncurses_scr.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

abstract_input.o: abstract_input.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

inputkeyboard.o: inputkeyboard.cc abstract_input.o ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

inputkeyboard_impl.o: inputkeyboard_impl.cc inputkeyboard.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

movement_strategy.o: movement_strategy.cc shape.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

linear_movement.o: linear_movement.cc movement_strategy.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

linear_movement_impl.o: linear_movement_impl.cc linear_movement.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

gravity.o: gravity.cc movement_strategy.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

gravity_impl.o: gravity_impl.cc gravity.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

cyclic_transformations.o: cyclic_transformations.cc movement_strategy.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

cyclic_transformations_impl.o: cyclic_transformations_impl.cc cyclic_transformations.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

player_movement.o: player_movement.cc movement_strategy.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

player_movement_impl.o: player_movement_impl.cc player_movement.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

view.o: view.cc ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

entity.o: entity.cc shape.o movement_strategy.o abstract_collider.o player_movement.o cyclic_transformations.o linear_movement.o ncurses_win.o gravity.o| std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

entity_impl.o: entity_impl.cc entity.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

model.o: model.cc entity.o shape.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

model_impl.o: model_impl.cc model.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_view.o: game_view.cc view.o entity.o ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_view_impl.o: game_view_impl.cc game_view.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

status_view.o: status_view.cc view.o ncurses_win.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

status_view_impl.o: status_view_impl.cc status_view.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

collider.o: collider.cc entity.o abstract_collider.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

abstract_game.o: abstract_game.cc | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_engine.o: game_engine.cc abstract_game.o model.o ncurses_scr.o game_view.o status_view.o clock.o abstract_input.o ncurses_win.o entity.o inputkeyboard.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_engine_impl.o: game_engine_impl.cc game_engine.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

flappy.o: flappy.cc game_engine.o abstract_game.o model.o collider.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

space_invaders.o: space_invaders.cc game_engine.o abstract_game.o model.o collider.o entity.o shape.o movement_strategy.o player_movement.o cyclic_transformations.o linear_movement.o gravity.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

bounce.o: bounce.cc game_engine.o abstract_game.o model.o collider.o entity.o shape.o movement_strategy.o player_movement.o cyclic_transformations.o linear_movement.o gravity.o | std-modules
	$(CXX) $(CXXFLAGS) -c $< -o $@

flappy: flappy.o game_engine_impl.o game_engine.o abstract_game.o model_impl.o model.o entity_impl.o entity.o shape.o movement_strategy.o player_movement_impl.o player_movement.o cyclic_transformations_impl.o cyclic_transformations.o linear_movement_impl.o linear_movement.o gravity_impl.o gravity.o clock_impl.o clock.o ncurses_scr_impl.o ncurses_scr.o ncurses_win_impl.o ncurses_win.o game_view_impl.o game_view.o status_view_impl.o status_view.o view.o collider.o abstract_collider.o inputkeyboard_impl.o inputkeyboard.o abstract_input.o
	$(CXX) $^ -o $@ $(LDFLAGS)

space_invaders: space_invaders.o game_engine_impl.o game_engine.o abstract_game.o model_impl.o model.o entity_impl.o entity.o shape.o movement_strategy.o player_movement_impl.o player_movement.o cyclic_transformations_impl.o cyclic_transformations.o linear_movement_impl.o linear_movement.o gravity_impl.o gravity.o clock_impl.o clock.o ncurses_scr_impl.o ncurses_scr.o ncurses_win_impl.o ncurses_win.o game_view_impl.o game_view.o status_view_impl.o status_view.o view.o collider.o abstract_collider.o inputkeyboard_impl.o inputkeyboard.o abstract_input.o
	$(CXX) $^ -o $@ $(LDFLAGS)

bounce: bounce.o game_engine_impl.o game_engine.o abstract_game.o model_impl.o model.o entity_impl.o entity.o shape.o movement_strategy.o player_movement_impl.o player_movement.o cyclic_transformations_impl.o cyclic_transformations.o linear_movement_impl.o linear_movement.o gravity_impl.o gravity.o clock_impl.o clock.o ncurses_scr_impl.o ncurses_scr.o ncurses_win_impl.o ncurses_win.o game_view_impl.o game_view.o status_view_impl.o status_view.o view.o collider.o abstract_collider.o inputkeyboard_impl.o inputkeyboard.o abstract_input.o
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o fin
	rm -rf gcm.cache