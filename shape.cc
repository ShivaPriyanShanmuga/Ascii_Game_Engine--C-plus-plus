export module shape;

import <utility>;
import <vector>;
import <optional>;

export class Shape {
    int x, y;
    int z;
    std::vector<std::pair<std::pair<int, int>, char>> charmap;

public:
    Shape(int x, int y, int z, char c): x(x), y(y), z(z) {
        charmap.emplace_back(std::make_pair(0, 0), c);
    }
    Shape(int x, int y, int z, int width, int height, char c): x(x), y(y), z(z) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                charmap.emplace_back(std::make_pair(i, j), c);
            }
        }
    }
    Shape(int x, int y, int z, std::vector<std::pair<std::pair<int, int>, char>> map): x(x), y(y), z(z), charmap(std::move(map)) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }

    std::vector<std::pair<std::pair<int, int>, char>> getCharMap() const {
        return charmap;
    }

    void setPosition(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }

    void setcharMap(const std::vector<std::pair<std::pair<int, int>, char>>& new_map) {
        charmap = new_map;
    }

    bool overlaps(const Shape& other, std::pair<int, int> nextPositions, std::pair<int, int> other_nextPositions) const {
        if (z != other.z) return false;
        for (const auto& [pos1, ch1] : charmap) {
            int abs_x1 = pos1.first + nextPositions.first;
            int abs_y1 = pos1.second + nextPositions.second;

            for (const auto& [pos2, ch2] : other.charmap) {
                int abs_x2 = pos2.first + other_nextPositions.first;
                int abs_y2 = pos2.second + other_nextPositions.second;

                if (abs_x1 == abs_x2 && abs_y1 == abs_y2) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isOutOfBounds(int min_x, int min_y, int max_x, int max_y) const {
        for (const auto& [offset, ch] : charmap) {
            int abs_x = x + offset.first;
            int abs_y = y + offset.second;
            if (abs_x <= min_x || abs_x >= max_x - 1 || abs_y <= min_y || abs_y >= max_y) {
                return true;
            }
        }
        return false;
    }

    bool goesOutOfBounds(std::pair<int, int> nextPositions, int min_x, int min_y, int max_x, int max_y) const {
        for (const auto& [offset, ch] : charmap) {
            int abs_x = offset.first + nextPositions.first + x;
            int abs_y = offset.second + nextPositions.second + y;
            if (abs_x <= min_x || abs_x >= max_x - 1 || abs_y <= min_y || abs_y >= max_y) {
                return true;
            }
        }
        return false;
    }

    bool outOfBoundsX(int min_x, int max_x, int out_x) const {
        for (const auto& [offset, ch] : charmap) {
            int abs_x = out_x + offset.first;
            if (abs_x <= min_x + 1 || abs_x >= max_x - 1) {
                return true;
            }
        }
        return false;
    }

    bool outOfBoundsY(int min_y, int max_y, int out_y) const {
        for (const auto& [offset, ch] : charmap) {
            int abs_y = out_y + offset.second;
            if (abs_y <= min_y || abs_y >= max_y) {
                return true;
            }
        }
        return false;
    }
};
