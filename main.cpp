// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "precompile.h"
#include "App.h"
#include "Game.h"

#include "json.h"
#undef main

int main(int argc, const char * argv[])
{
    App::initialize();

    Game* game = Game::getInstance();
    game->initialize();

    try {
        game->launch();
        delete game;
	}
    catch (std::exception e) {
        std::cout << "Crashed LOL DAMN!";
        return -1;
    }

    delete App::getInstance();

	return 0;
}