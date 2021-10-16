#include <iostream>

template <typename T>
class Grid { 
private: 
    T *memory; 
    size_t x_size, y_size; 
public:
    Grid(size_t x_size_, size_t y_size_) : x_size(x_size_), y_size(y_size_) {
        memory = new T[x_size_*y_size_];
    }

    Grid() : x_size(0), y_size(0) {};

    size_t get_idx(size_t x_idx, size_t y_idx) const{
        return y_size*x_idx + y_idx;
    }
    
    T operator()(size_t x_idx, size_t y_idx) const {
        return memory[get_idx(x_idx, y_idx)];
    }

    T& operator()(size_t x_idx, size_t y_idx) {
        return memory[get_idx(x_idx, y_idx)];
    } 
    

    size_t get_xsize() const {
        return x_size;
    } 
    size_t get_ysize() const {
        return y_size;
    }
    

    Grid& operator=(T tmp) {
        for (size_t i = 0; i < x_size; i++) {
            for (size_t j = 0; j < y_size; j++) {
                if (memory[get_idx(i, j)] != tmp) {
                    memory[get_idx(i, j)] = tmp;
                }
            }
        }
        return *this;
    } 
    
    template <typename T1>
    friend std::ostream& operator<<(std::ostream&, Grid <T1> const&); 

    template <typename T1>
    friend std::istream& operator>>(std::istream&, Grid <T1> &);

    ~Grid() {
        delete[] memory;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Grid <T> const& grid) {
        for (size_t i = 0; i < grid.get_xsize(); i++) {
            for (size_t j = 0; j < grid.get_ysize()-1; j++) {
                os << grid(i, j) << " ";
            }
            os << grid(i, grid.get_ysize()-1);
            os << "\n";
        }
        return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Grid <T> &grid) {
    for (size_t i = 0; i < grid.get_xsize(); i++) {
        for (size_t j = 0; j < grid.get_ysize(); j++) {
            T tmp;
            is >> tmp;
            grid(i, j) = tmp;
        }
    }
    return is;
}

int main() {
    Grid<float> ex(2, 4);
    ex = 3;
    // std::cout << ex.get_idx(1, 1);
    std::cout << ex;

    std::cin >> ex;
    std::cout << ex;

    // std::cin >> ex;
    // std::cout << ex;

    return 0;
}
