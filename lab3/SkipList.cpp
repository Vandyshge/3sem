#include <iostream>
#include <vector>
#include <limits>
#include <random>  
#include <ctime> 

std::random_device rd;
std::mt19937 gen(rd());

template <typename T> 
struct SkipNode
{
    T value;
    float key;
    std::vector <SkipNode*> forwardNodes;
    SkipNode* prev;
};

template <typename T>
class BidirectionIterator;

template <typename T>
class ForwardIterator;

template <typename T>
class ReverseIterator;



template <typename T, typename Comparator = std::less<T>> 
class SkipList {
private:
    SkipNode<T>* NIL = new SkipNode<T>;
    SkipNode<T>* head = new SkipNode<T>;
    double probability;
    size_t size_list;
    size_t maxLevel;
    Comparator comparator;
    // std::random_device rd;
    // std::mt19937 gen; 
public:
    SkipList() : probability(0.5), size_list(0), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->prev = nullptr;
        head->forwardNodes = {NIL};
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes = {nullptr};
        NIL->prev = head;
    }

    SkipList(std::vector <T> const &values, std::vector <float> const &keys) : probability(0.5), size_list(keys.size()), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->forwardNodes = {NIL};
        head->prev;
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes = {nullptr};
        NIL->prev = head;
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

    SkipList(SkipList <T> const &s) : maxLevel(1), probability(s.p()), size_list(0) {
        head->key = std::numeric_limits<float>::min();
        head->prev = nullptr;
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes.push_back(nullptr);
        NIL->prev = head;
        head->forwardNodes.push_back(NIL);
        SkipNode<T>* tmp = s.get_head()->forwardNodes[0];
        for (auto i = 0; i < s.size(); i++) {
            std::cout << "insert\n";
            insert(tmp->key, tmp->value, tmp->forwardNodes.size());
            tmp = tmp->forwardNodes[0];
        }
    }

    SkipList(BidirectionIterator<T> &it1, BidirectionIterator<T> &it2) : probability(0.5), size_list(0), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->prev = nullptr;
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes.push_back(nullptr);
        NIL->prev = head;
        head->forwardNodes.push_back(NIL);
        insert(it1, it2);
    }

    SkipList(SkipList&& s) : probability(std::move(s.p())), size_list(0), maxLevel(1) {
        head->key = std::numeric_limits<float>::min();
        head->prev = nullptr;
        NIL->key = std::numeric_limits<float>::max();
        NIL->forwardNodes.push_back(nullptr);
        NIL->prev = head;
        head->forwardNodes.push_back(NIL);
        SkipNode<T>* tmp = std::move(s.get_head()->forwardNodes[0]);
        for (auto i = 0; i < s.size(); i++) {
            std::cout << "insert\n";
            insert(tmp->key, tmp->value, tmp->forwardNodes.size());
            tmp = tmp->forwardNodes[0];
        }
    }

    SkipList& operator=(SkipList<T> const& s) {
        clear();
        // std::cout << "1234\n";
        probability = s.p();
        // std::cout << "size=" << s.size() << "\n";
        SkipNode<T>* tmp = s.get_head()->forwardNodes[0];
        for (auto i = 0; i < s.size(); i++) {
            // std::cout << "insert2\n";
            insert(tmp->key, tmp->value, tmp->forwardNodes.size());
            tmp = tmp->forwardNodes[0];
        }
        return *this;
    }

    SkipList& operator=(SkipList<T> const&& s) {
        clear();
        // std::cout << "1234\n";
        probability = std::move(s.p());
        // std::cout << "size=" << s.size() << "\n";
        SkipNode<T>* tmp = std::move(s.get_head()->forwardNodes[0]);
        for (auto i = 0; i < s.size(); i++) {
            // std::cout << "insert2\n";
            insert(tmp->key, tmp->value, tmp->forwardNodes.size());
            tmp = tmp->forwardNodes[0];
        }
        return *this;
    }

    // std::vector <int> get_level() const {
    //     std::vector <> lev;
    //     SkipNode<T>* tmp = head;
    //     lev.push_back(maxLevel);
    //     while (head) {

    //     }
    // }

    int maxLevel_new() const {
        // std::cout << "p: " << probability << "\n";
        // std::random_device rd;
        // std::mt19937 gen(rd());
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

    int get_maxLevel() const {
        return maxLevel;
    }

    SkipNode<T>* get_head() const {
        return head;
    }

    SkipNode<T>* find(float key_) {
        // std::cout << "---------------------------------------"  << std::endl;
        // std::cout << "find: key=" << key_ << "\n\n";
        if (size() == 0) return bend();

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
                    return BidirectionIterator<T>(tmp->forwardNodes[level-1]);
                } else if (tmp->forwardNodes[level-1]->key > key_) {
                    break;
                }
                tmp = tmp->forwardNodes[level-1];
            }
            if (tmp->forwardNodes[level-1] == NIL && tmp->key == key_) return BidirectionIterator<T>(tmp);
            if (tmp->key == key_) return BidirectionIterator<T>(tmp);
            level--;
        }
        // std::cout << "---------------------------------------" << std::endl;
        return bend();
    }

    void insert(float key_, T value_) {
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        // std::cout << "insert: key=" << key_ << "\n\n";
        SkipNode<T>* NewNode = new SkipNode<T>;
        NewNode->value = value_;
        NewNode->key = key_;

        // size_list++;
        int c = maxLevel_new();
        // int c = 4;
        // std::cout << "c: " << c << "\n";
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
                    // std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << prev->forwardNodes[tmp_c-1]->key << "\n";
                    NewNode->forwardNodes[tmp_c-1] = prev->forwardNodes[tmp_c-1];
                    prev->forwardNodes[tmp_c-1] = NewNode;
                    break;
                }
                // std::cout << " -- " << "\n";
                prev = prev->forwardNodes[tmp_c-1];
            }
            if (tmp_c == 1) NewNode->prev = prev;
            tmp_c--;
        }
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    void insert(float key_, T value_, int c_) {
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        // std::cout << "insert: key=" << key_ << " ,c=" << c_ << "\n";
        SkipNode<T>* NewNode = new SkipNode<T>;
        NewNode->value = value_;
        NewNode->key = key_;
        size_list++;
        // int c = maxLevel_new();
        // int c = 4;
        // std::cout << "c: " << c << "\n";
        NewNode->forwardNodes.resize(c_);
        int tmp_c = c_;
        SkipNode<T>* prev;
        // SkipNode<T>* next;
        if (c_ > maxLevel) {
            // std::cout << "insert: key=" << key_ << " ,c=" << c_ << "\n";
            head->forwardNodes.emplace_back(NewNode);
            NewNode->forwardNodes[c_-1] = NIL;
            NIL->forwardNodes.emplace_back(nullptr);
            tmp_c--;
            maxLevel++;
            // std::cout << "new maxLev " << maxLevel << "\n";
        }
        while (tmp_c > 0) {
            prev = head;
            while (prev->key < key_) {
                if (prev->forwardNodes[tmp_c-1]->key >= key_) {
                    // std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << prev->forwardNodes[tmp_c-1]->key << "\n";
                    NewNode->forwardNodes[tmp_c-1] = prev->forwardNodes[tmp_c-1];
                    prev->forwardNodes[tmp_c-1] = NewNode;
                    break;
                }
                // std::cout << " -- " << "\n";
                prev = prev->forwardNodes[tmp_c-1];
            }
            if (tmp_c == 1) NewNode->prev = prev;
            tmp_c--;
        }
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    void insert(BidirectionIterator<T> &it1, BidirectionIterator<T> &it2) {
        for (auto it = it1; it1 != it2; ++it) {
            SkipNode<T>* tmp = it.get_node();
            insert(tmp->key, tmp->value, tmp->forwardNodes.size());
        }
    }

    void erase(float key_) {
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        // std::cout << "erase: key=" << key_ << "\n\n";
        SkipNode<T>* Node = find(key_).get_node();
        int tmp_c = Node->forwardNodes.size();
        SkipNode<T>* prev;
        if (head->forwardNodes[tmp_c-1] == Node && Node->forwardNodes[tmp_c-1] == NIL) {
            // std::cout << "level: " << tmp_c << ", prev: " << head->key << ", next: " << head->forwardNodes[tmp_c-1]->key << "\n";
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
            // std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << Node->forwardNodes[tmp_c-1]->key << "\n";
            prev->forwardNodes[tmp_c-1] = Node->forwardNodes[tmp_c-1];
            if (tmp_c == 1) Node->forwardNodes[0]->prev = prev;
            tmp_c--;
        }
        delete Node;
        size_list--;
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    void erase(BidirectionIterator<T> &it) {
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
        // std::cout << "erase: key=" << key_ << "\n\n";
        SkipNode<T>* Node = it.get_node();
        int tmp_c = Node->forwardNodes.size();
        SkipNode<T>* prev;
        if (head->forwardNodes[tmp_c-1] == Node && Node->forwardNodes[tmp_c-1] == NIL) {
            // std::cout << "level: " << tmp_c << ", prev: " << head->key << ", next: " << head->forwardNodes[tmp_c-1]->key << "\n";
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
            // std::cout << "level: " << tmp_c << ", prev: " << prev->key << ", next: " << Node->forwardNodes[tmp_c-1]->key << "\n";
            prev->forwardNodes[tmp_c-1] = Node->forwardNodes[tmp_c-1];
            if (tmp_c == 1) Node->forwardNodes[0]->prev = prev;
            tmp_c--;
        }
        delete Node;
        size_list--;
        // std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    }

    void erase(BidirectionIterator<T> &it1, BidirectionIterator<T> &it2) {
        for (auto it = it1; it1 != it2; ++it) {
            erase(it);
        }        
    }

    int count(float key_) {
        SkipNode<T>* Node = find(key_).get_node();
        if (Node == nullptr) return 0;
        int s = 1;
        while (Node->forwardNodes[0]->key == key_) {
            Node = Node->forwardNodes[0];
            s++;
        }
        return s;
    }

    BidirectionIterator<T> lower_bound(float key_) {
        BidirectionIterator<T> res = find(key_);
        --res;
        return res;
    }

    BidirectionIterator<T> upper_bound(float key_) {
        BidirectionIterator<T> res = find(key_);
        while (res.get_node()->forwardNodes[0]->key == key_) ++res;
        return ++res;
    }

    std::pair<BidirectionIterator<T>, BidirectionIterator<T>> equal_range(float key_) {
        BidirectionIterator<T> it1 = find(key_);
        int n = count(key_);
        BidirectionIterator<T> it2 = it1 + n;
        std::pair<BidirectionIterator<T>, BidirectionIterator<T>> res = {it1, it2};
        return res;
    }

    void clear() {
        SkipNode<T>* tmp = head->forwardNodes[0];
        SkipNode<T>* tmp1;
        // std::cout << "insert11\n";
        while (tmp->forwardNodes[0] != nullptr) {
            tmp1 = tmp->forwardNodes[0];
            delete tmp;
            tmp = tmp1;
        }
        // if (size_list != 0) delete tmp;
        // std::cout << "insert\n";

        head->forwardNodes = {NIL};
        NIL->forwardNodes = {nullptr};
        size_list = 0;
        maxLevel = 1;
    }

    bool compare(float key1, float key2) {
        return comparator(key1, key2);
    };

    template <typename T1> 
    friend std::ostream &operator<<(std::ostream &os, SkipList<T1> &s);
    

    ~SkipList() {
        SkipNode<T>* tmp = head->forwardNodes[0];
        SkipNode<T>* tmp1;
        delete head;
        // tmp = tmp->forwardNodes[0];
        while (tmp->forwardNodes[0] != nullptr) {
            // std::cout << "key: " << tmp->key << "\n";
            tmp1 = tmp->forwardNodes[0];
            delete tmp;
            tmp = tmp1;
        }
        delete tmp;
    }

    // class ForwardIterator;
    // class ReverseIterator;
    // BidirectionIterator<T>;

    
    BidirectionIterator<T> bbegin() {
        return BidirectionIterator<T>(head->forwardNodes[0]);
    }

    BidirectionIterator<T> bend() {
        return BidirectionIterator<T>(NIL);
    }

    ForwardIterator<T> fbegin() {
        return ForwardIterator<T>(head->forwardNodes[0]);
    }

    ForwardIterator<T> fend() {
        return ForwardIterator<T>(NIL);
    }

    ReverseIterator<T> rbegin() {
        return ReverseIterator<T>(NIL);
    }

    ReverseIterator<T> rend() {
        return ReverseIterator<T>(head);
    }
};

template <typename T> 
class BidirectionIterator {
    private:
        SkipNode<T>* it;
    public:
        BidirectionIterator(SkipNode<T>* it_) : it(it_) {};

        BidirectionIterator& operator=(BidirectionIterator<T> const &tmp) = default;

        T operator*() const {
            return it->key;
        }

        BidirectionIterator& operator++() {
            // std::cout << "icbrjvf\n";
            it = it->forwardNodes[0];
            return *this;
        }

        BidirectionIterator operator++(int) {
            BidirectionIterator<T> tmp = *this;
            ++(*this);
            return tmp;
        }

        BidirectionIterator& operator--() {
            it = it->prev;
            return *this;
        }

        BidirectionIterator operator--(int) {
            BidirectionIterator<T> tmp = *this;
            --(*this);
            return tmp;
        }

        BidirectionIterator& operator+(int n) {
            for (auto i = 0; i < n; i++) {
                // if (it->forwardNodes[0] == nullptr) return nullptr;
                it = it->forwardNodes[0];
            }
            return *this;
        }

        BidirectionIterator& operator+=(int n) {
            for (auto i = 0; i < n; i++) {
                // if (it->forwardNodes[0] == nullptr) return nullptr;
                it = it->forwardNodes[0];
            }
            return *this;
        }

        BidirectionIterator& operator-(int n) {
            for (auto i = 0; i < n; i++) {
                // if (it->forwardNodes[0] == nullptr) return nullptr;
                it = it->prev;
            }
            return *this;
        }

        BidirectionIterator& operator-=(int n) {
            for (auto i = 0; i < n; i++) {
                // if (it->forwardNodes[0] == nullptr) return nullptr;
                it = it->prev;
            }
            return *this;
        }

        SkipNode<T>* get_node() const {
            return it;
        }

        bool operator!=(BidirectionIterator<T> const &s) const {
            return it != s.get_node();
        }
};

template <typename T> 
class ForwardIterator {
    private:
        SkipNode<T>* it;
    public:
        ForwardIterator(SkipNode<T>* it_) : it(it_) {};

        ForwardIterator& operator=(ForwardIterator<T> const &tmp) = default;

        T operator*() const {
            return it->value;
        }

        ForwardIterator& operator++() {
            // std::cout << "icbrjvf\n";
            it = it->forwardNodes[0];
            return *this;
        }

        SkipNode<T>* get_node() const {
            return it;
        }

        bool operator!=(ForwardIterator<T> const &s) const {
            return it != s.get_node();
        }
};

template <typename T> 
class ReverseIterator {
    private:
        SkipNode<T>* it;
    public:
        ReverseIterator(SkipNode<T>* it_) : it(it_) {};

        ReverseIterator& operator=(ReverseIterator<T> const &tmp) = default;

        T operator*() const {
            return it->key;
        }

        ReverseIterator& operator++() {
            // std::cout << "icbrjvf\n";
            it = it->prev;
            return *this;
        }

        SkipNode<T>* get_node() const {
            return it;
        }

        bool operator!=(ReverseIterator<T> const &s) const {
            return it != s.get_node();
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
};


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
    std::cout << s << "\n";

    SkipList<int> s1 = std::move(s);
    // SkipNode<int>* tmp = s.find(3);
    // std::cout << tmp->key << "\n"; 
    // std::cout << tmp->prev->prev->key << "\n";    

    // SkipList<int> s1;
    // std::cout << s;
    // std::cout << s1;
    // std::cout << s1.size() << "\n";
    // s.clear();
    // std::cout << s;
    // std::cout << s1 << s1.get_maxLevel();
    // s1 = s;
    // std::cout << s;
    // std::cout << s1;

    // SkipList<int>::BidirectionIterator beg = s.bbegin();
    // for (auto end = s.bend(); beg != end; ++beg) {
    //     *beg = 10;
    // }
    auto beg = s.bbegin();
    beg = beg + 3;
    auto a = beg++;

    std::cout << *a << "\n";
    std::cout << *beg << "\n";

    // for (auto it = s.bbegin(); it != s.bend(); ++it) {
    //     std::cout << *it << " " << std::endl;
    // }

    // BidirectionIterator<int> beg = s.bbegin();

    // std::cout << *(++++beg) << "\n";


    return 1;
}