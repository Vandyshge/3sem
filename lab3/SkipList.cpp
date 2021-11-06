#include <iostream>
#include <vector>
#include <limits>
#include <random>  
#include <ctime> 

template <typename T> 
struct SkipNode
{
    T value;
    float key;
    std::vector <SkipNode*> forwardNodes;
};

template <typename T> 
class SkipList {
private:
    SkipNode<T>* NIL = new SkipNode<T>;
    SkipNode<T>* head = new SkipNode<T>;
    double probability;
    size_t size_list;
    size_t maxLevel;
    // std::mt19937 gen(time(0));
    // std::uniform_real_distribution<> p(0, 1);
public:
    SkipList() : probability(0.5), size_list(0), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->forwardNodes = {NIL};
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes = {nullptr};
    }

    SkipList(std::vector <T> const &values, std::vector <float> const &keys) : probability(0.5), size_list(keys.size()), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->forwardNodes = {NIL};
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes = {nullptr};
        for (auto i = 0; i < values.size(); i++) {
            insert(keys[i], values[i]);
        }
    }

    SkipList(bool t) : probability(0.5), size_list(5), maxLevel(3) {
        SkipNode<T>* NN1 = new SkipNode<T>;
        SkipNode<T>* NN2 = new SkipNode<T>;
        SkipNode<T>* NN3 = new SkipNode<T>;
        SkipNode<T>* NN4 = new SkipNode<T>;
        SkipNode<T>* NN5 = new SkipNode<T>;
        NN1->key = 1;
        NN2->key = 3;
        NN3->key = 5;
        NN4->key = 7;
        NN5->key = 9;
        head->key = std::numeric_limits<float>::min();
        head->forwardNodes = {NN1, NN1, NN1};
        NN1->forwardNodes = {NN2, NN3, NN4};
        NN2->forwardNodes = {NN3};
        NN3->forwardNodes = {NN4, NN4};
        NN4->forwardNodes = {NN5, NIL, NIL};
        NN5->forwardNodes = {NIL};
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes = {nullptr, nullptr, nullptr};
    }



    int maxLevel_new() const {
        // std::cout << "p: " << probability << "\n";
        // std::random_device rd;
        std::mt19937 gen(time(0));
        std::uniform_real_distribution<> p(0, 1);
        int c = 1;
        double rnd = p(gen);
        // std::cout << "rnd: " << rnd << "\n";
        while (c <= maxLevel && rnd > probability) {
            // std::cout << "rnd > p: " << (rnd > probability) << "\n"; 
            c++;
            rnd = p(gen);
            // std::cout << "rnd: " << rnd << "\n";
        }
        // std::cout << "c=" << c << "\n";
        return c;
    }

    bool empty() const {
        return size_list == 0;
    }

    size_t size() const {
        return size_list;
    }

    double p() const {
        return probability;
    }

    void new_p(double p) {
        probability = p;
    }

    SkipNode<T>* find(float key_) {
        // std::cout << "---------------------------------------"  << std::endl;
        // std::cout << "find: key=" << key_ << "\n\n";
        if (size() == 0) return nullptr;

        size_t level = maxLevel;
        // std::cout << "level: " << level << "\n";
        SkipNode<T>* tmp = head->forwardNodes[level-1];
        // std::cout << "-------------------\n";
        // std::cout << tmp->key << "\n";
        while (level > 0) {
            while (tmp->forwardNodes[level-1] != NIL && tmp->key < key_) {
                // std::cout << "1 -- " << tmp->key << "\n";
                // std::cout << "2 -- " << tmp->forwardNodes[level-1]->key << "\n";
                if (tmp->forwardNodes[level-1]->key == key_) {
                    return tmp->forwardNodes[level-1];
                } else if (tmp->forwardNodes[level-1]->key > key_) {
                    break;
                }
                tmp = tmp->forwardNodes[level-1];
            }
            if (tmp->forwardNodes[level-1] == NIL && tmp->key == key_) return tmp;
            if (tmp->key == key_) return tmp;
            level--;
        }
        // std::cout << "---------------------------------------" << std::endl;
        return nullptr;
    }

    void insert(float key_, T value_) {
        std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "insert: key=" << key_ << "\n\n";
        SkipNode<T>* NewNode = new SkipNode<T>;
        NewNode->value = value_;
        NewNode->key = key_;

        int c = maxLevel_new();
        // int c = 4;
        std::cout << "c: " << c << "\n";
        NewNode->forwardNodes.resize(c);
        int tmp_c = c;
        SkipNode<T>* prev;
        // SkipNode<T>* next;
        if (c > maxLevel) {
            head->forwardNodes.emplace_back(NewNode);
            NewNode->forwardNodes[c-1] = NIL;
            NIL->forwardNodes.emplace_back(nullptr);
            tmp_c--;
            maxLevel++;
        }
        while (tmp_c > 0) {
            prev = head;
            while (prev->key < key_) {
                if (prev->forwardNodes[tmp_c-1]->key >= key_) {
                    std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << prev->forwardNodes[tmp_c-1]->key << "\n";
                    NewNode->forwardNodes[tmp_c-1] = prev->forwardNodes[tmp_c-1];
                    prev->forwardNodes[tmp_c-1] = NewNode;
                    break;
                }
                // std::cout << " -- " << "\n";
                prev = prev->forwardNodes[tmp_c-1];
            }
            tmp_c--;
        }
        std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    void erase(float key_) {
        std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "erase: key=" << key_ << "\n\n";
        SkipNode<T>* Node = find(key_);
        int tmp_c = Node->forwardNodes.size();
        SkipNode<T>* prev;
        if (head->forwardNodes[tmp_c-1] == Node && Node->forwardNodes[tmp_c-1] == NIL) {
            std::cout << "level: " << tmp_c << ", prev: " << head->key << ", next: " << head->forwardNodes[tmp_c-1]->key << "\n";
            maxLevel--;
            head->forwardNodes.pop_back();
            Node->forwardNodes.pop_back();
            tmp_c--;
        }
        while (tmp_c > 0) {
            prev = head;
            while (prev->forwardNodes[tmp_c-1] != Node) {
                prev = prev->forwardNodes[tmp_c-1];
            }
            std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << Node->forwardNodes[tmp_c-1]->key << "\n";
            prev->forwardNodes[tmp_c-1] = Node->forwardNodes[tmp_c-1];
            tmp_c--;
        }
        delete Node;
        size_list--;
        std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    int count(float key_) {
        SkipNode<T>* Node = find(key_);
        if (Node == nullptr) return 0;
        int s = 1;
        while (Node->forwardNodes[0]->key == key_) {
            Node = Node->forwardNodes[0];
            s++;
        }
        return s;
    }

    void clear() {
        SkipNode<T>* tmp = head->forwardNodes[0];
        while (tmp->forwardNodes[0] != NIL) {
            tmp = tmp->forwardNodes[0];
            delete tmp;
        }
        delete tmp;

        head->forwardNodes = {NIL};
        size_list = 0;
        maxLevel = 1;
    }

    template <typename T1> 
    friend std::ostream &operator<<(std::ostream &os, SkipList<T1> &s);
    

    ~SkipList() {
        SkipNode<T>* tmp = head->forwardNodes[0];
        delete head;
        // tmp = tmp->forwardNodes[0];
        while (tmp->forwardNodes[0] != nullptr) {
            tmp = tmp->forwardNodes[0];
            delete tmp;
        }
        delete tmp;
    }
};


template <typename T> 
std::ostream &operator<<(std::ostream &os, SkipList<T> &s) {
    os << "-------------------------------------------------------------------------------------------" << std::endl;
    os << "print:\n\n";
    SkipNode<T>* tmp;
    for (auto i = s.maxLevel; i > 0; i--) {
        os << "level: " << i << " : " << s.head->key;
        tmp = s.head->forwardNodes[i-1];
        while (tmp->forwardNodes[i-1] != nullptr) {
            os << " -- " << tmp->key;
            tmp = tmp->forwardNodes[i-1];
        }
        os << " -- " << tmp->key << "\n";
    }
    os << "-------------------------------------------------------------------------------------------" << std::endl;
    return os;
}


int main() {
    // SkipList<int> s(true);
    // std::cout << s.empty() << "\n";
    // std::cout << s.size() << "\n";
    // std::cout << s;
    // std::cout << s.find(6) << "\n";
    // s.clear();
    // std::cout << s.empty() << "\n";
    // std::cout << s.size() << "\n";
    // std::cout << s.find(6) << "\n";
    // s.insert(6, 10);
    // std::cout << s;
    // s.insert(6, 10);
    // std::cout << "count: " << s.count(6) << "\n";
    // std::cout << s;
    // s.erase(6);
    // std::cout << s;
    // SkipNode<int>* head;
    // head->value;
    
    SkipList<int> s({1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6});
    std::cout << s;

    return 1;
}
