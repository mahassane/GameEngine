#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: Done (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

// The idea of a checkerboard is that the color changes every tile. 
// This means that, if the top_left most tile has an index of one and the tile to its right is 2 and so on, all the tiles on that row with odd indices will be a color and 
// the one with even indices will be another. The next row they switch and then the next and so on.
// To do this, we imagine that every pixel is located in a specific tile and to get the index of the color of this tile we divide both coordinates by the size.
// Then we sum these values and divide them by 2. If remainder is 0, we use color[0] and if remainder is 1, we use color[1].
// This way every tile has a different color from all its neighbours.

void main(){
    float tile_x_coord = gl_FragCoord.x/size;
    float tile_y_coord = gl_FragCoord.y/size;
    int tile = int(tile_x_coord) + int(tile_y_coord);
    int color_index = tile % 2;
    frag_color = vec4(colors[color_index], 1.0);
}