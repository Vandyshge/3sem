#include <iostream>
#include <set>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

class State {
public:
    // State() = default;
    virtual bool contains(int s) const  = 0;
};

class DiscreteState : public State {
private:
    int const state;

public:
    DiscreteState(int state_): state(state_) { }

    virtual bool contains(int s) const {
        return s == state;
    }
};

class SegmentState : public State {
private:
    int beg, end;
    
public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg_, int end_): beg(beg_), end(end_) { }

    void update(int beg_, int end_) {
        std::cout << beg_ << "  " << end_ << "\n";
        beg = beg_, end = end_;
    }
    
    virtual bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState : public State {
private:
    std::set<int> const states;
    
public:
    SetState(): states({}) { }
    SetState(std::set<int> const &src): states(src) { }
    
    virtual bool contains(int s) const {
        return states.count(s) > 0;
    }
};

// набор непрерывный состояний с исключением
class ContinuousStateSkip : public State {
private:
    SegmentState ss;
    SegmentState skip;
    
public:
    ContinuousStateSkip() { }
    ContinuousStateSkip(int beg_, int end_, std::set<int> const skip_) {
        ss.update(beg_, end_);
        skip.update(*skip_.begin(), *(--skip_.end()));
    }
    
    void update(int beg, int end, std::set<int> skip_) {
        ss.update(beg, end);
        skip.update(*skip_.begin(), *(--skip_.end()));
    }

    bool contains(int s) const {
        return ss.contains(s) && !skip.contains(s);
    }
};

// набор непрерывный состояний с дополнениями
class ContinuousStateAdd : public State {
private:
    SegmentState ss;
    SegmentState add;
    
public:
    ContinuousStateAdd() { }
    ContinuousStateAdd(int beg, int end, std::set<int> const add_) {
        ss.update(beg, end);
        std::cout << *(--add_.end()) << "\n";
        add.update(*add_.begin(), *(--add_.end()));
    }
    
    void update(int beg, int end, std::set<int> add_) {
        ss.update(beg, end);
        add.update(*add_.begin(), *(--add_.end()));
    }

    bool contains(int s) const {
        // if (add.contains(s)) std::cout << "10\n";
        // std::cout << (ss.contains(s) || add.contains(s)) << "  " << s << "\n";
        return ss.contains(s) || add.contains(s);
    }
};

// набор непрерывный состояний с дополнениями и исключениями
class ContinuousStateAddSkip : public State {
private:
    int const beg, end;
    std::set<int> const add;
    std::set<int> const skip;
    ContinuousStateAdd csa;
    ContinuousStateSkip css;
    
public:
    ContinuousStateAddSkip(): beg(0), end(-1), add({}), skip({}) {
        csa.update(beg, end, add);
        css.update(beg, end, skip);
    }
    ContinuousStateAddSkip(int beg, int end, std::set<int> const skip, std::set<int> const add): beg(beg), end(end), skip(skip), add(add) {
        csa.update(beg, end, add);
        css.update(beg, end, skip);
    }
    
    bool contains(int s) const {
        return csa.contains(s) && css.contains(s);
    }
};

// состояние как объединение 2ух состояний 
template <class t1, class t2>
class UnionState : public State {
private:
    t1 s1;
    t2 s2;    
public:
    UnionState(): s1(t1()), s2(t2()) {}
    UnionState(t1 s1, t2 s2): s1(s1), s2(s2) { }
    
    bool contains(int s) const {
        return s1.contains(s) || s2.contains(s);
    }
};

// состояние как пересечение 2ух состояний 
template <class t1, class t2>
class IntersectionState : public State {
private:
    t1 s1;
    t2 s2;    
public:
    IntersectionState(): s1(t1()), s2(t2()) {}
    IntersectionState(t1 s1, t2 s2): s1(s1), s2(s2) { }
    
    bool contains(int s) const {
        return s1.contains(s) && s2.contains(s);
    }
};



class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;
    
public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }
    
    float operator()(State &s) {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;
        
        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};




int main(int argc, const char * argv[]) {
    // DiscreteState d(1);
    // SegmentState s(0,10);
    // SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    // ContinuousStateAdd csa(0, 10, {15});
    // ContinuousStateSkip css(0, 10, {0, 99});
    // ContinuousStateAddSkip csas(0, 10, {0, 100}, {50});
    // UnionState <ContinuousStateSkip, ContinuousStateAdd> us(css, csa);
    // srand(time(NULL));
    // int r = rand() % 1000;
    // ProbabilityTest pt(r,0,100,100000);
    // std::cout << pt(d) << std::endl;
    // std::cout << pt(us) << std::endl;
    // std::cout << pt(ss) << std::endl;
    // s.update(0, 50);
    // std::cout << pt(s) << std::endl;

    std::ofstream out; 
    out.open("C:\\0.LaLaLand\\0.Phystech\\2kurs\\out.txt");
    std::cout << out.is_open() << "\n";
    // out << std::to_string(pt(us)) << "\n";
    // out.close();

    // 1 task
    // int n = 10;
    // int sk = 1e8;
    // for (auto test_count = 10; test_count <= sk; test_count *= n) {
    //     out << test_count << " ";
    // }
    // out << "\n";

    // DiscreteState d(1);
    // for (auto test_count = 10; test_count <= sk; test_count *= n) {
    //     ProbabilityTest pt(10,0,100,test_count);
    //     float tmp = 0;
    //     int n1 = 10;
    //     for (auto i = 0; i < n1; i++) {
    //         tmp += pt(d);
    //     }
    //     out << tmp/n1 << " ";
    //     std::cout << "2\n";
    // }
    // out << "\n";
    // std::cout << "1\n";

    // SegmentState s(0,10);
    // for (auto test_count = 10; test_count <= sk; test_count *= n) {
    //     ProbabilityTest pt(10,0,100,test_count);
    //     float tmp = 0;
    //     int n1 = 10;
    //     for (auto i = 0; i < n1; i++) {
    //         tmp += pt(s);
    //     }
    //     out << tmp/n1 << " ";
    //     std::cout << "2\n";
    // }
    // out << "\n";
    // std::cout << "1\n";

    // SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    // for (auto test_count = 10; test_count <= sk; test_count *= n) {
    //     ProbabilityTest pt(10,0,100,test_count);
    //     float tmp = 0;
    //     int n1 = 10;
    //     for (auto i = 0; i < n1; i++) {
    //         tmp += pt(ss);
    //     }
    //     out << tmp/n1 << " ";
    //     std::cout << "2\n";
    // }
    // out << "\n";


    // task_2
    int n = 10;
    int sk = 100;
    for (auto num = 9; num <= sk; num += n) {
        out << num << " ";
    }
    out << "\n";

    ProbabilityTest pt(10,0,100,1e7);
    for (auto num = 9; num <= sk; num += n) {
        SegmentState s(0,num);
        float tmp = 0;
        int n1 = 10;
        for (auto i = 0; i < n1; i++) {
            tmp += pt(s);
        }
        out << tmp/n1 << " ";
        std::cout << "2\n";
    }
    out << "\n";
    std::cout << "1\n";

    for (auto num = 9; num <= sk; num += n) {
        std::set <int> v = {};
        for (auto i = 0; i < num; i++) {
            int tmp = rand() % 100;
            // std::cout << tmp << " " << v.count(tmp) << "\n";
            while (v.count(tmp)) {
                // std::cout << tmp << " " << v.count(tmp) << "\n";
                tmp = rand() % 100;
            }
            v.insert(tmp);
        }
        // for (auto tmp : v) {
        //     std::cout << tmp << " ";
        // }
        // std::cout << "\n";
        SetState ss(v);
        float tmp = 0;
        int n1 = 10;
        for (auto i = 0; i < n1; i++) {
            tmp += pt(ss);
        }
        out << tmp/n1 << " ";
        std::cout << "2\n";
    }
    out << "\n";


    return 0;
}
