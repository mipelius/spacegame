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

#ifndef __Background_H_
#define __Background_H_

class Texture;

class Background {
private:
    Texture* texture;
    int x;
    int y;
    int w;
    int h;
    void init(Texture *texture, int x, int y, int w, int h);
public:
    /**
     * Constructor that initializes the width and the height using the size of the texture
     *
     * @param texture   the texture object used for rendering
     * @param x         the x-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     * @param y         the y-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     */
    Background(Texture *texture, int x, int y);

    /**
     * Constructor
     *
     * @param texture   the texture object used for rendering
     * @param x         the x-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     * @param y         the y-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     * @param w         the width of the background
     * @param h         the height of the background
     */
    Background(Texture *texture, int x, int y, int w, int h);

    /**
     * Renders the background using texture area from point (offsetX, offsetY) to point (offsetX + w, offsetY + h)
     *
     * @param offsetX   the x-coordinate of the offset
     * @param offsetY   the y-coordinate of the offset
     * @param w         the width of the rectangle
     * @param h         the height of the rectangle
     */
    void render(int offsetX, int offsetY, int w, int h);

    /**
     * @return          the x-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     */
    int getX();

    /**
     * @return          the y-coordinate of the northwest corner
     *                  of the destination rectangle in pixels
     */
    int getY();

    /**
     * @return          the width of the background
     */
    int getW();

    /**
     * @return          the height of the background
     */
    int getH();
};


#endif //__Background_H_
