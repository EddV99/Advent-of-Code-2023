#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * declarations
 */
std::vector<std::string> tokenize(std::string line);
std::shared_ptr<std::ifstream> openFile(std::string filePath);

void part1(std::string inputFile);

void part2(std::string inputFile);

/**
 * util
 */
std::shared_ptr<std::ifstream> openFile(std::string filePath) {
    std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>(
            filePath);
    if (!file->is_open()) {
        std::cout << "Coudn't find input file\n";
        exit(1);
    }
    return file;
}

std::vector<std::string> tokenize(std::string line) {
    std::vector<std::string> tokens;

    std::stringstream stream(line);
    std::string save;

    while (std::getline(stream, save, ' ')) {
        if (!save.empty()) tokens.push_back(save);
    }
    return tokens;
}

/**
 * main function
 */
int main(int argc, char** argv) {
    std::string inputFile;
    if (argc > 1) inputFile = std::string(argv[1]);
    else {
        std::cout << "No input file given\n";
        return 1;
    }

    part1(inputFile);
    part2(inputFile);

    return 0;
};

/**
 * Part 1
 */

struct position {
    int row;
    int col;
    char pipe;
    bool visited;
    position(int row, int col, char pipe)
        : row(row), col(col), pipe(pipe), visited(false) {}
    position() : row(-1), col(-1) {}

    bool operator==(const position& other) {
        return (row == other.row) && (col == other.col);
    }
};

enum pipeShape { NS, EW, NE, NW, SW, SE, START, NONE };
enum direction { N, E, S, W };

pipeShape getPipeShape(char ch) {
    if (ch == '|') return NS;
    if (ch == '-') return EW;
    if (ch == 'L') return NE;
    if (ch == 'J') return NW;
    if (ch == '7') return SW;
    if (ch == 'F') return SE;
    if (ch == 'S') return START;
    return NONE;
}
using grid = std::vector<std::vector<position>>;

struct pipeMaze {
    grid maze;
    position start;
    int width;
    int height;
};

void getWidthAndHeightOfPipeMaze(int& width, int& height,
                                 std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);
    height = 0;
    std::string line;
    std::getline(*file, line);
    width = line.length();
    height++;

    while (std::getline(*file, line)) { height++; }
}

void printGrid(const grid maze) {
    int height = maze.size();
    int width = maze[0].size();

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) { std::cout << maze[r][c].pipe; }
        std::cout << std::endl;
    }
}

void setupGrid(grid& m, int width, int height) {
    for (int r = 0; r < height; r++) {
        std::vector<position> tmp;
        for (int c = 0; c < width; c++) { tmp.push_back(position(r, c, '.')); }
        m.push_back(tmp);
    }
}

/**
 * Check if in range of maze
 */
bool inRange(const pipeMaze& maze, int row, int col) {
    return (row < maze.height && row >= 0) && (col < maze.width && col >= 0);
}

/**
 * Check if two pipes connect from a direction
 *  Assume we are looking from pipe 1 to pipe 2
 */
bool pipesConnect(char p1, char p2, direction looking) {
    pipeShape pipe1 = getPipeShape(p1);
    pipeShape pipe2 = getPipeShape(p2);

    if (pipe1 == NONE || pipe2 == NONE) return false;

    bool southEntrance = pipe2 == SE || pipe2 == SW || pipe2 == NS;
    bool northEntrance = pipe2 == NS || pipe2 == NE || pipe2 == NW;
    bool eastEntrance = pipe2 == SE || pipe2 == EW || pipe2 == NE;
    bool westEntrance = pipe2 == SW || pipe2 == EW || pipe2 == NW;

    if (pipe1 == START) {
        if (looking == N) return southEntrance;
        if (looking == S) return northEntrance;
        if (looking == W) return eastEntrance;
        if (looking == E) return westEntrance;
    } else if (pipe1 == NS) {
        if (looking == N) return southEntrance;
        if (looking == S) return northEntrance;
    } else if (pipe1 == EW) {
        if (looking == E) return westEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == NE) {
        if (looking == N) { return southEntrance; }
        if (looking == E) return westEntrance;
    } else if (pipe1 == NW) {
        if (looking == N) return southEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == SW) {
        if (looking == S) return northEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == SE) {
        if (looking == S) return northEntrance;
        if (looking == E) return westEntrance;
    }
    return false;
}
/**
 * Get position of connected pipe next to it.
 *
 * The problem guarantees a loop, so each pipe has one unvisited pipe it can
 * traverse. The start should have two, so just find the first one.
 */
position getNext(const pipeMaze& maze, position pipe) {
    int row = pipe.row;
    int col = pipe.col;
    char pipeCh = maze.maze[row][col].pipe;

    // look up
    if (inRange(maze, row - 1, col)
        && pipesConnect(pipeCh, maze.maze[row - 1][col].pipe, N)
        && !maze.maze[row - 1][col].visited) {
        return maze.maze[row - 1][col];
    }
    // look down
    if (inRange(maze, row + 1, col)
        && pipesConnect(pipeCh, maze.maze[row + 1][col].pipe, S)
        && !maze.maze[row + 1][col].visited) {
        return maze.maze[row + 1][col];
    }
    // look right
    if (inRange(maze, row, col + 1)
        && pipesConnect(pipeCh, maze.maze[row][col + 1].pipe, E)
        && !maze.maze[row][col + 1].visited) {
        return maze.maze[row][col + 1];
    }
    // look left
    if (inRange(maze, row, col - 1)
        && pipesConnect(pipeCh, maze.maze[row][col - 1].pipe, W)
        && !maze.maze[row][col - 1].visited) {
        return maze.maze[row][col - 1];
    }

    return position();
}

int getFarthestPath(pipeMaze maze) {
    position left = getNext(maze, maze.start);
    maze.maze[left.row][left.col].visited = true;
    position right = getNext(maze, maze.start);
    maze.maze[right.row][right.col].visited = true;

    int count = 1;
    while (true) {
        count++;

        left = getNext(maze, left);
        if (left.row == -1) break;
        maze.maze[left.row][left.col].visited = true;


        right = getNext(maze, right);
        if (right.row == -1) break;
        maze.maze[right.row][right.col].visited = true;
    }

    return count;
}

void part1(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    int height;
    int width;
    getWidthAndHeightOfPipeMaze(width, height, inputFile);
    pipeMaze maze;
    setupGrid(maze.maze, width, height);
    maze.width = width;
    maze.height = height;


    std::string line;

    // fill grid with pipes
    int row = 0;
    while (std::getline(*file, line)) {
        for (int col = 0; col < width; col++) {
            char ch = line[col];
            if (ch == 'S') {
                maze.start.row = row;
                maze.start.col = col;
                maze.start.pipe = 'S';
                maze.start.visited = false;
            }
            maze.maze[row][col].pipe = ch;
        }
        row++;
    }

    int result = getFarthestPath(maze);
    std::cout << "Part 1: Steps farthest from start is " << result << "\n";
}
/**
 * Mark the loop in the maze
 */
void findLoop(pipeMaze& maze) {
    // find the loop
    position next = getNext(maze, maze.start);
    maze.maze[next.row][next.col].visited = true;
    position prev = maze.start;
    maze.start.visited = true;


    while (true) {
        prev = next;
        next = getNext(maze, prev);

        if (next.row == -1) break;
        maze.maze[next.row][next.col].visited = true;
    }
}

/**
 * An edge case for ray casting
 * Returns count of characters to skip
 */
int isPointEdge(const pipeMaze& maze, int row, int col) {
    bool isPoint = false;
    int count = 1;

    char p1 = maze.maze[row][col].pipe;
    int i = col + 1;
    while (inRange(maze, row, i) && maze.maze[row][i].pipe == '-') {
        count++;
        i++;
    }
    char p2 = maze.maze[row][i].pipe;
    count++;

    if ((p1 == 'F' && p2 == '7') || (p1 == 'L' && p2 == 'J')) isPoint = true;

    if (isPoint) return count;

    return -1;
}
/**
 * An edge case for ray casting
 * Returns count of characters to skip
 */
int isEdge(const pipeMaze& maze, int row, int col) {
    bool isPoint = false;
    int count = 1;

    char p1 = maze.maze[row][col].pipe;
    int i = col + 1;
    while (inRange(maze, row, i) && maze.maze[row][i].pipe == '-') {
        count++;
        i++;
    }
    char p2 = maze.maze[row][i].pipe;
    count++;

    if ((p1 == 'F' && p2 == 'J') || (p1 == 'L' && p2 == '7')) isPoint = true;

    if (isPoint) return count;

    return -1;
}
/**
 * Check if given coords is inside the loop using a "ray cast"
 * If we throw a ray in any direction and count the number of times it crosses
 * a shape (loop). If it is even then it's outside the shape, inside if odd.
 */
bool inLoop(const pipeMaze& maze, int row, int col) {
    if (maze.maze[row][col].visited) return false;

    int count = 0;
    for (int i = col + 1; i < maze.width;) {
        if (maze.maze[row][i].visited) {
            if (isEdge(maze, row, i) != -1) {
                count++;
                i += isEdge(maze, row, i);
                continue;
            } else if (isPointEdge(maze, row, i) != -1) {
                count += 2;
                i += isPointEdge(maze, row, i);
                continue;
            } else {
                count++;
                i++;
                continue;
            }
        }

        i++;
    }
    return !(count % 2 == 0);
}

/**
 * Part 2
 */
void part2(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    int height;
    int width;
    getWidthAndHeightOfPipeMaze(width, height, inputFile);
    pipeMaze maze;
    setupGrid(maze.maze, width, height);
    maze.width = width;
    maze.height = height;
    std::string line;

    // fill grid with pipes
    int row = 0;
    while (std::getline(*file, line)) {
        for (int col = 0; col < width; col++) {
            char ch = line[col];
            if (ch == 'S') {
                maze.start.row = row;
                maze.start.col = col;
                maze.start.pipe = 'S';
                maze.start.visited = false;
            }
            maze.maze[row][col].pipe = ch;
        }
        row++;
    }

    // explore the grid and mark the loop visited
    findLoop(maze);

    int count = 0;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            if (inLoop(maze, r, c)) { count++; }
        }
    }

    std::cout << "Part 2: Number of pipes inside the loop is " << count << "\n";
}


/**
 *                      --- Day 10: Pipe Maze ---
 *
 * You use the hang glider to ride the hot air from Desert Island all the
 * way up to the floating metal island. This island is surprisingly cold and
 * there definitely aren't any thermals to glide on, so you leave your hang
 * glider behind.
 *
 * You wander around for a while, but you don't find any people or animals.
 * However, you do occasionally find signposts labeled "Hot Springs"
 * pointing in a seemingly consistent direction; maybe you can find someone
 * at the hot springs and ask them where the desert-machine parts are made.
 *
 * The landscape here is alien; even the flowers and trees are made of
 * metal. As you stop to admire some metal grass, you notice something
 * metallic scurry away in your peripheral vision and jump into a big pipe!
 * It didn't look like any animal you've ever seen; if you want a better
 * look, you'll need to get ahead of it.
 *
 * Scanning the area, you discover that the entire field you're standing on
 * is densely packed with pipes; it was hard to tell at first because
 * they're the same metallic silver color as the "ground". You make a quick
 * sketch of all of the surface pipes you can see (your puzzle input).
 *
 * The pipes are arranged in a two-dimensional grid of tiles:
 *
 *     | is a vertical pipe connecting north and south.
 *     - is a horizontal pipe connecting east and west.
 *     L is a 90-degree bend connecting north and east.
 *     J is a 90-degree bend connecting north and west.
 *     7 is a 90-degree bend connecting south and west.
 *     F is a 90-degree bend connecting south and east.
 *     . is ground; there is no pipe in this tile.
 *     S is the starting position of the animal; there is a pipe on this
 * tile, but your sketch doesn't show what shape the pipe has.
 *
 * Based on the acoustics of the animal's scurrying, you're confident the
 * pipe that contains the animal is one large, continuous loop.
 *
 * For example, here is a square loop of pipe:
 *
 * .....
 * .F-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * If the animal had entered this loop in the northwest corner, the sketch
 * would instead look like this:
 *
 * .....
 * .S-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * In the above diagram, the S tile is still a 90-degree F bend: you can
 * tell because of how the adjacent pipes connect to it.
 *
 * Unfortunately, there are also many pipes that aren't connected to the
 * loop! This sketch shows the same loop as above:
 *
 * -L|F7
 * 7S-7|
 * L|7||
 * -L-J|
 * L|-JF
 *
 * In the above diagram, you can still figure out which pipes form the main
 * loop: they're the ones connected to S, pipes those pipes connect to,
 * pipes those pipes connect to, and so on. Every pipe in the main loop
 * connects to its two neighbors (including S, which will have exactly two
 * pipes connecting to it, and which is assumed to connect back to those two
 * pipes).
 *
 * Here is a sketch that contains a slightly more complex main loop:
 *
 * ..F7.
 * .FJ|.
 * SJ.L7
 * |F--J
 * LJ...
 *
 * Here's the same example sketch with the extra, non-main-loop pipe tiles
 * also shown:
 *
 * 7-F7-
 * .FJ|7
 * SJLL7
 * |F--J
 * LJ.LJ
 *
 * If you want to get out ahead of the animal, you should find the tile in
 * the loop that is farthest from the starting position. Because the animal
 * is in the pipe, it doesn't make sense to measure this by direct distance.
 * Instead, you need to find the tile that would take the longest number of
 * steps along the loop to reach from the starting point - regardless of
 * which way around the loop the animal went.
 *
 * In the first example with the square loop:
 *
 * .....
 * .S-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * You can count the distance each tile in the loop is from the starting
 * point like this:
 *
 * .....
 * .012.
 * .1.3.
 * .234.
 * .....
 *
 * In this example, the farthest point from the start is 4 steps away.
 *
 * Here's the more complex loop again:
 *
 * ..F7.
 * .FJ|.
 * SJ.L7
 * |F--J
 * LJ...
 *
 * Here are the distances for each tile on that loop:
 *
 * ..45.
 * .236.
 * 01.78
 * 14567
 * 23...
 *
 * Find the single giant loop starting at S. How many steps along the loop
 * does it take to get from the starting position to the point farthest from
 * the starting position?
 *
 *                            --- Part Two ---
 * You quickly reach the farthest point of the loop, but the animal never
 * emerges. Maybe its nest is within the area enclosed by the loop?
 *
 * To determine whether it's even worth taking the time to search for such a
 * nest, you should calculate how many tiles are contained within the loop.
 * For example:
 *
 * ...........
 * .S-------7.
 * .|F-----7|.
 * .||.....||.
 * .||.....||.
 * .|L-7.F-J|.
 * .|..|.|..|.
 * .L--J.L--J.
 * ...........
 *
 * The above loop encloses merely four tiles - the two pairs of . in the
 * southwest and southeast (marked I below). The middle . tiles (marked O
 * below) are not in the loop. Here is the same loop again with those
 * regions marked:
 *
 * ...........
 * .S-------7.
 * .|F-----7|.
 * .||OOOOO||.
 * .||OOOOO||.
 * .|L-7OF-J|.
 * .|II|O|II|.
 * .L--JOL--J.
 * .....O.....
 *
 * In fact, there doesn't even need to be a full tile path to the outside
 * for tiles to count as outside the loop - squeezing between pipes is also
 * allowed! Here, I is still within the loop and O is still outside the
 * loop:
 *
 * ..........
 * .S------7.
 * .|F----7|.
 * .||OOOO||.
 * .||OOOO||.
 * .|L-7F-J|.
 * .|II||II|.
 * .L--JL--J.
 * ..........
 *
 * In both of the above examples, 4 tiles are enclosed by the loop.
 *
 * Here's a larger example:
 *
 * .F----7F7F7F7F-7....
 * .|F--7||||||||FJ....
 * .||.FJ||||||||L7....
 * FJL7L7LJLJ||LJ.L-7..
 * L--J.L7...LJS7F-7L7.
 * ....F-J..F7FJ|L7L7L7
 * ....L7.F7||L7|.L7L7|
 * .....|FJLJ|FJ|F7|.LJ
 * ....FJL-7.||.||||...
 * ....L---J.LJ.LJLJ...
 *
 * The above sketch has many random bits of ground, some of which are in the
 * loop (I) and some of which are outside it (O):
 *
 * OF----7F7F7F7F-7OOOO
 * O|F--7||||||||FJOOOO
 * O||OFJ||||||||L7OOOO
 * FJL7L7LJLJ||LJIL-7OO
 * L--JOL7IIILJS7F-7L7O
 * OOOOF-JIIF7FJ|L7L7L7
 * OOOOL7IF7||L7|IL7L7|
 * OOOOO|FJLJ|FJ|F7|OLJ
 * OOOOFJL-7O||O||||OOO
 * OOOOL---JOLJOLJLJOOO
 *
 * In this larger example, 8 tiles are enclosed by the loop.
 *
 * Any tile that isn't part of the main loop can count as being enclosed by
 * the loop. Here's another example with many bits of junk pipe lying around
 * that aren't connected to the main loop at all:
 *
 * FF7FSF7F7F7F7F7F---7
 * L|LJ||||||||||||F--J
 * FL-7LJLJ||||||LJL-77
 * F--JF--7||LJLJ7F7FJ-
 * L---JF-JLJ.||-FJLJJ7
 * |F|F-JF---7F7-L7L|7|
 * |FFJF7L7F-JF7|JL---7
 * 7-L-JL7||F7|L7F-7F7|
 * L.L7LFJ|||||FJL7||LJ
 * L7JLJL-JLJLJL--JLJ.L
 *
 * Here are just the tiles that are enclosed by the loop marked with I:
 *
 * FF7FSF7F7F7F7F7F---7
 * L|LJ||||||||||||F--J
 * FL-7LJLJ||||||LJL-77
 * F--JF--7||LJLJIF7FJ-
 * L---JF-JLJIIIIFJLJJ7
 * |F|F-JF---7IIIL7L|7|
 * |FFJF7L7F-JF7IIL---7
 * 7-L-JL7||F7|L7F-7F7|
 * L.L7LFJ|||||FJL7||LJ
 * L7JLJL-JLJLJL--JLJ.L
 *
 * In this last example, 10 tiles are enclosed by the loop.
 *
 * Figure out whether you have time to search for the nest by calculating
 * the area within the loop. How many tiles are enclosed by the loop?
 */
