#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class SegmentTree {
public:
    explicit SegmentTree(std::vector<int> borders) {
        if (borders.size() < 3) {
            number_of_levels = 0;
        } else {
            number_of_levels = static_cast<int>(log2(borders.size() - 2)) + 1;
        }
        int num_of_borders_in_segtree = static_cast<int>(pow(2, number_of_levels)) + 1;
        std::sort(borders.begin(), borders.end());
        
        for (int i = 0; i < num_of_borders_in_segtree; ++i) {
            if (i < borders.size()) {
                sorted_borders.push_back(borders[i]);
            } else {
                sorted_borders.push_back(big_constant + i);
            }
        }
        int num_of_vertices_in_segtree = num_of_borders_in_segtree * 2 - 3;
        for (int i = 0; i < num_of_vertices_in_segtree; ++i) {
            lengths.push_back(0);
            values.push_back(0);
            levels.push_back(static_cast<int>(log2(i + 1)));
            left_borders.push_back(sorted_borders[left_and_right_borders_for(i).first]);
            right_borders.push_back(sorted_borders[left_and_right_borders_for(i).second]);
        }
    }
    bool Add(int left_border, int right_border) {
        bool insert_flag = true;
        Add(left_border, right_border, 0, insert_flag);
        return insert_flag;
    }
    void Remove(int left_border, int right_border) {
        Remove(left_border, right_border, 0);
    }
private:
    const long long big_constant = 10000000000;
    int number_of_levels;
    std::vector<long long> sorted_borders;
    std::vector<int> values;
    std::vector<long long> left_borders;
    std::vector<long long> right_borders;
    std::vector<int> levels;
    std::vector<int> lengths;
    
    std::pair<int, int> left_and_right_borders_for(int element) {
        int order = element - static_cast<int>(pow(2, static_cast<int>(log2(element + 1)))) + 1;
        int length = static_cast<int>(pow(2, number_of_levels - static_cast<int>(log2(element + 1))));
        return std::make_pair(length * order, length * (order + 1));
    }
    
    void Add(long long left_border, long long right_border, int index,
             bool &insert_flag) {
        if (values[index] != 0) {
            insert_flag = false;
        }
        if (levels[index] == number_of_levels) {
            if (left_border == left_borders[index] &&
                right_border == right_borders[index]) {
                ++values[index];
            }
        } else {
            int left_son = 2 * index + 1;
            int right_son = left_son + 1;
            if (left_border == left_borders[index] &&
                right_border == right_borders[index]) {
                ++values[index];
            } else {
                long long middle_pivot = left_borders[right_son];
                if (left_border < middle_pivot) {
                    if (middle_pivot < right_border) {
                        Add(left_border, middle_pivot, left_son, insert_flag);
                        Add(middle_pivot, right_border, right_son, insert_flag);
                    } else {
                        Add(left_border, right_border, left_son, insert_flag);
                    }
                } else {
                    Add(left_border, right_border, right_son, insert_flag);
                }
            }
        }
    }
    
    void Remove(long long left_border, long long right_border, int index) {
        if (levels[index] == number_of_levels) {
            if (left_border == left_borders[index] &&
                right_border == right_borders[index]) {
                --values[index];
            }
        } else {
            int left_son = 2 * index + 1;
            int right_son = left_son + 1;
            if (left_border == left_borders[index] &&
                right_border == right_borders[index]) {
                --values[index];
            } else {
                long long middle_pivot = left_borders[right_son];
                if (left_border < middle_pivot) {
                    if (middle_pivot < right_border) {
                        Remove(left_border, middle_pivot, left_son);
                        Remove(middle_pivot, right_border, right_son);
                    } else {
                        Remove(left_border, right_border, left_son);
                    }
                } else {
                    Remove(left_border, right_border, right_son);
                }
            }
        }
    }
};
