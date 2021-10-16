#include <iostream>

template <typename T>
T min(T a, T b) {
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

template <typename T>
T max(T a, T b) {
    if (a < b) {
        return b;
    } else {
        return a;
    }
}

template <typename T>
T abs(T a) {
    if (a < 0) {
        return -a;
    } else {
        return a;
    }
}

template <typename T>
void swap(T* i, T* j) {
    T tmp = *i;
    *i = *j;
    *j = tmp;
}



template <int N>
struct factorial {
    static const int value = N * factorial <N - 1>::value;
};

template <>
struct factorial <0> {
    static const int value = 1;
};


template <int N, int i>
struct is_prime {
    static const bool value = (N % i == 0) ? false : is_prime <N, i-1>::value;
};


template <int N>
struct is_prime <N, 1> {
    static const bool value = true;
};

template <int N>
struct Prime {
    static const bool res = is_prime <N, N/2>::value;
};



int main() {
    const int f = factorial <5>::value;
    std::cout << f << "\n";

    const bool p = Prime <10>::res;
    std::cout << p << "\n";


    int a[3] = {1, 2, 3};
    swap(&(a[0]), &(a[1]));
    for (auto u : a) {
        std::cout << u << " ";
    }
    std::cout << "\n";

    return 0;
};