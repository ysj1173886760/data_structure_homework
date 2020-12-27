//
// Created by sheep on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_RBTREE_HPP
#define DATA_STRUCTURE_HOMEWORK_RBTREE_HPP

#include<vector>

#define bro(x) (((x)->fa->ls == (x)) ? ((x)->fa->rs) : ((x)->fa->ls))
#define isls(x) ((x) != nullptr && (x)->fa->ls == (x))
#define isrs(x) ((x) != nullptr && (x)->fa->rs == (x))

template<typename Key, typename Value>
class RBTree {
private:
    struct Node;

    Node *_root;
    Node *_hot;

    int _size;

    void destroy(Node *);
//    void update(Node*);

    void connect34(Node*, Node*, Node*, Node*, Node*, Node*, Node*);

    void solveDoubleRed(Node*);

    void solveDoubleBlack(Node*);
    Node* rfind(Key);
//    Node* findKth(int, Node*);
//    int findRank(Value, Node*);

    Node* getFirstElement();

    //debug
    void inorderTraversal(Node*);
    void inorderTraversal(Node*, std::vector<Value>&);
//    iterator kth(int);
public:
    struct iterator;

    RBTree(): _root(nullptr), _hot(nullptr), _size(0) {}
    ~RBTree() {
        this->destroy(_root);
    }

    iterator find(Key);

    Value& operator[](Key);
//    iterator lower_bound(T);
//    iterator upper_bound(T);
//    iterator getPre(Value);
//    iterator getNxt(Value);

    iterator begin();
    iterator end();

    bool remove(Key);
    iterator insert(Key, Value = Value());

    int size();
    bool empty();

//    iterator getKth(int);
//    int getRank(Value);

    //debug
    void checkConnect();
    void checkConnect(Node*);
    std::vector<Value> getInorderTraversal(bool);
};

template<typename Key, typename Value>
struct RBTree<Key, Value>::Node {
    Key key;
    Value val;
    bool col;   // true for red, false for black
    Node* fa;
    Node* ls;
    Node* rs;
//    int size;
//    int cnt;

    explicit Node(Value _val = Value(), Key _key = Key(), bool _col = true, Node* _fa = nullptr,
         Node* _ls = nullptr, Node* _rs = nullptr) :val(_val), key(_key),
         col(_col), fa(_fa), ls(_ls), rs(_rs) {}

    Node* succ() {      // 找后继
        Node* now = rs;
        while(now->ls != nullptr) {
            now = now->ls;
        }
        return now;
    }

    Node* leftNode() {
        Node* now = this;
        if (!ls) {
            while (now->fa && now->fa->ls == now) {
                now = now->fa;
            }
            now = now->fa;
        } else {
            now = now->ls;
            while (now->rs) {
                now = now->rs;
            }
        }
        return now;
    }

    Node* rightNode() {
        Node* now = this;
        if (!rs) {
            while (now->fa && now->fa->rs == now) {
                now = now->fa;
            }
            now = now->fa;
        } else {
            now = now->rs;
            while (now->ls) {
                now = now->ls;
            }
        }
        return now;
    }

//    void maintain() {
//        size = cnt;
//        if (ls) size += ls->size;
//        if (rs) size += rs->size;
//    }
};

template<typename Key, typename Value>
struct RBTree<Key, Value>::iterator {
private:
    Node* node;

public:
    iterator& operator--() {
        node = node->leftNode();
        return *this;
    }

    iterator& operator++() {
        node = node->rightNode();
        return *this;
    }

    std::pair<Key, Value> operator*() {
        return std::make_pair (node->key, node->val);
    }

    Node* operator->() {    //有破坏封装性的嫌疑
        return node;
    }

    explicit iterator(Node* _node = nullptr): node(_node) {}
    iterator(iterator const& iter): node(iter.node) {}

    bool operator!= (const iterator &iter) {
        return iter.node != node;
    }
    bool operator== (const iterator &iter) {
        return iter.node == node;
    }
};

template<typename Key, typename Value>
Value& RBTree<Key, Value>::operator[](Key key) {
    if (this->find(key) == this->end()) {
        this->insert(key);
    }
    return (this->find(key))->val;
}

template<typename Key, typename Value>
void RBTree<Key, Value>::destroy(RBTree::Node *now) {
    if (!now) return;
    destroy(now->ls);
    destroy(now->rs);
    delete now;
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::iterator RBTree<Key, Value>::insert(Key key, Value val) {
    Node* now = rfind(key);
    if (now != nullptr) {
        now->val = val;
        return iterator(now);
    }
    this->_size++;
    if (_hot == nullptr) {
        _root = new Node(val, key, false, nullptr, nullptr, nullptr);
        return iterator(_root);
    }
    now = new Node(val, key, true, _hot, nullptr, nullptr);
    if (_hot->key < key) {
        _hot->rs = now;
    } else {
        _hot->ls = now;
    }
    solveDoubleRed(now);
    return iterator(now);
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::Node* RBTree<Key, Value>::getFirstElement() {
    if (_root == nullptr) return nullptr;
    Node* now = _root;
    while (now->ls) now = now->ls;
    return now;
}

template<typename Key, typename Value>
void RBTree<Key, Value>::inorderTraversal(Node* now) {
    if (!now) return;
    inorderTraversal(now->ls);
    printf("%d ", now->val);
    inorderTraversal(now->rs);
}

template<typename Key, typename Value>
void RBTree<Key, Value>::inorderTraversal(Node* now, std::vector<Value> &ans) {
    if (!now) return;
    inorderTraversal(now->ls, ans);
    ans.push_back(now->val);
    inorderTraversal(now->rs, ans);
}

template<typename Key, typename Value>
std::vector<Value> RBTree<Key, Value>::getInorderTraversal(bool output) {
    std::vector<Value> ans;
    if (output) inorderTraversal(_root);
    else inorderTraversal(_root, ans);
    return ans;
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::iterator RBTree<Key, Value>::begin() {
    return iterator(getFirstElement());
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::iterator RBTree<Key, Value>::end() {
    return iterator(nullptr);
}

template<typename Key, typename Value>   ////验证所有节点与父亲的连接是否正常、域s是否维护正常
void RBTree<Key, Value>::checkConnect(Node* now) {
    if(!now) return;
    //assert(now->size > 0);
    //assert(!(now->ls && now->ls->fa != now));
    //assert(!(now->rs && now->rs->fa != now));
    if(now->ls && now->ls->fa != now) {
        printf("Oops! %d has a ls %d, but it failed to point its fa!\n", now->val, now->ls->val);
    }
    if(now->rs && now->rs->fa != now) {
        printf("Oops! %d has a rs %d, but it failed to point its fa!\n", now->val, now->rs->val);
    }
    int sss = now->size;
    if(now->ls) sss -= now->ls->size;
    if(now->rs) sss -= now->rs->size;
    //assert(!(sss - 1));
    if(sss - now->cnt) {
        printf("Damn! %d's size is %d, but the sum of its children's size is %d!\n", now->val, now->size, now->size - sss);
    }
    checkConnect(now->ls);
    checkConnect(now->rs);
}

template<typename Key, typename Value>
void RBTree<Key, Value>::checkConnect() {
    checkConnect(_root);
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::iterator RBTree<Key, Value>::find(Key key) {
    Node* now = this->rfind(key);
    return iterator(now);
}

template<typename Key, typename Value>
typename
RBTree<Key, Value>::Node* RBTree<Key, Value>::rfind(Key key) {
    Node* now = _root;
    _hot = nullptr;
    while (now != nullptr && now->key != key) {
        _hot = now;
        now = (key < now->key) ? now->ls : now->rs;
    }
    return now;
}

template<typename Key, typename Value>
void RBTree<Key, Value>::solveDoubleRed(Node* now) {
    while (now->fa == nullptr || now->fa->col) {
        if (now == _root) {
            now->col = false;
            return;
        }
        Node* fa = now->fa;
        if (!fa->col) return;
        Node* uncle = bro(fa);
        Node* grdfa = fa->fa;
        if (uncle != nullptr && uncle->col) {
            grdfa->col = true;
            uncle->col = false;
            fa->col = false;
            now = grdfa;
        } else {
            if (isls(fa)) {
                if (isls(now)) {            // ll
                    fa->fa = grdfa->fa;
                    if (grdfa == _root) _root = fa;
                    else if (grdfa->fa->ls == grdfa) grdfa->fa->ls = fa;
                    else grdfa->fa->rs = fa;
                    connect34(fa, now, grdfa, now->ls, now->rs, fa->rs, uncle);
                    fa->col = false;
                    grdfa->col = true;
                } else {                    // lr
                    now->fa = grdfa->fa;
                    if (grdfa == _root) _root = now;
                    else if (grdfa->fa->ls == grdfa) grdfa->fa->ls = now;
                    else grdfa->fa->rs = now;
                    connect34(now, fa, grdfa, fa->ls, now->ls, now->rs, uncle);
                    now->col = false;
                    grdfa->col = true;
                }
            } else {
                if (isls(now)) {            // rl
                    now->fa = grdfa->fa;
                    if (grdfa == _root) _root = now;
                    else if (grdfa == grdfa->fa->ls) grdfa->fa->ls = now;
                    else grdfa->fa->rs = now;
                    connect34(now, grdfa, fa, uncle, now->ls, now->rs, fa->rs);
                    now->col = false;
                    grdfa->col = true;
                } else {                    // rr
                    fa->fa = grdfa->fa;
                    if (grdfa == _root) _root = fa;
                    else if (grdfa == grdfa->fa->ls) grdfa->fa->ls = fa;
                    else grdfa->fa->rs = fa;
                    connect34(fa, grdfa, now, uncle, fa->ls, now->ls, now->rs);
                    fa->col = false;
                    grdfa->col = true;
                }
            }
            return;
        }
    }
}

template<typename Key, typename Value>
void RBTree<Key, Value>::connect34(Node* root, Node* rootls, Node* rootrs,
                                   Node* tree1, Node* tree2, Node* tree3, Node* tree4) {
    rootls->ls = tree1;
    if (tree1) tree1->fa = rootls;
    rootls->rs = tree2;
    if (tree2) tree2->fa = rootls;
    rootrs->ls = tree3;
    if (tree3) tree3->fa = rootrs;
    rootrs->rs = tree4;
    if (tree4) tree4->fa = rootrs;
    root->ls = rootls;
    root->rs = rootrs;
    rootls->fa = root;
    rootrs->fa = root;
//    rootls->maintain();
//    rootrs->maintain();
//    root->maintain();
}

//template<typename Key, typename Value>
//typename
//RBTree<Key, Value>::iterator RBTree<Key, Value>::getKth(int rank) {
//    Node* now = _root;
//    while(true) {
//        if (rank > (now->ls ? now->ls->size : 0) + now->cnt) {
//            rank -= (now->ls ? now->ls->size : 0) + now->cnt;
//            now = now->rs;
//        } else if (now->ls && now->ls->size >= rank) now = now->ls;
//        else return iterator(now);
//    }
//}
//
//template<typename T>
//int RBTree<T>::getRank(T val) {
//    Node* now = _root;
//    int sum = 1;
//    while (now) {
//        if (val > now->val) {
//            sum += now->cnt + (now->ls ? now->ls->size : 0);
//            now = now->rs;
//        } else now = now->ls;
//    }
//    return sum;
//}

template<typename Key, typename Value>
int RBTree<Key, Value>::size() {
    return _size;
}

template<typename Key, typename Value>
bool RBTree<Key, Value>::empty() {
    return !_root;
}

//template<typename Key, typename Value>
//void RBTree<Key, Value>::update(Node* now) {
//    while (now) {
//        now->maintain();
//        now = now->fa;
//    }
//}

template<typename Key, typename Value>
bool RBTree<Key, Value>::remove(Key key) {
    Node *now = rfind(key);
    if (!now) return false;
    this->_size--;
    Node *node_suc;
    while (now->ls || now->rs) {
        if (!now->ls) {
            node_suc = now->rs;
        } else if (!now->rs) {
            node_suc = now->ls;
        } else {
            node_suc = now->succ();
        }
        now->key = node_suc->key;
        now->val = node_suc->val;
        now = node_suc;
    }
    if (!now->col) {
        solveDoubleBlack(now);
    }
    if (now == _root) {
        _root = nullptr;
        delete now;
        return true;
    }
    if (now->fa->ls == now) now->fa->ls = nullptr;
    else now->fa->rs = nullptr;
    delete now;
    return true;
}

template<typename Key, typename Value>
void RBTree<Key, Value>::solveDoubleBlack(Node* now) {
    while (now != _root) {
        Node* fa = now->fa;
        Node* brother = bro(now);
        if (brother->col) {     // 兄弟节点是红色 父亲是黑色
            brother->col = false;
            fa->col = true;
            if (fa == _root) _root = brother;
            if (fa->fa) {
                if (fa->fa->ls == fa) fa->fa->ls = brother;
                else fa->fa->rs = brother;
            }
            brother->fa = fa->fa;
            if (isls(now)) {
                connect34(brother, fa, brother->rs, now, brother->ls, brother->rs->ls, brother->rs->rs);
            } else {
                connect34(brother, brother->ls, fa, brother->ls->ls, brother->ls->rs, brother->rs, now);
            }
            brother = bro(now);
            fa = now->fa;
        }
        if (brother->ls && brother->ls->col) {
            bool oldcol = fa->col;
            fa->col = false;
            if (fa->ls == now) {
                if (fa->fa) {
                    if (fa->fa->ls == fa) fa->fa->ls = brother->ls;
                    else fa->fa->rs = brother->ls;
                }
                brother->ls->fa = fa->fa;
                if (fa == _root) _root = brother->ls;
                connect34(brother->ls, fa, brother, now, brother->ls->ls, brother->ls->rs, brother->rs);
            } else {
                brother->ls->col = false;
                if (fa->fa) {
                    if (fa->fa->ls == fa) fa->fa->ls = brother;
                    else fa->fa->rs = brother;
                }
                brother->fa = fa->fa;
                if (fa == _root) _root = brother;
                connect34(brother, brother->ls, fa, brother->ls->ls, brother->ls->rs, brother->rs, now);
            }
            fa->fa->col = oldcol;
            return;
        } else if (brother->rs && brother->rs->col) {
            bool oldcol = fa->col;
            fa->col = false;
            if (fa->ls == now) {
                brother->rs->col = false;
                if (fa->fa) {
                    if (fa->fa->ls == fa) fa->fa->ls = brother;
                    else fa->fa->rs = brother;
                }
                brother->fa = fa->fa;
                if (fa == _root) _root = brother;
                connect34(brother, fa, brother->rs, now, brother->ls, brother->rs->ls, brother->rs->rs);
            } else {
                if (fa->fa) {
                    if (fa->fa->ls == fa) fa->fa->ls = brother->rs;
                    else fa->fa->rs = brother->rs;
                }
                brother->rs->fa = fa->fa;
                if(fa == _root) _root = brother->rs;
                connect34(brother->rs, brother, fa, brother->ls, brother->rs->ls, brother->rs->rs, now);
            }
            fa->fa->col = oldcol;
            return;
        }
        if (fa->col) {
            fa->col = false;
            brother->col = true;
            return;
        } else {
            brother->col = true;
            now = fa;
        }
    }
}

//template<typename Key, typename Value>
//typename
//RBTree<Key, Value>::iterator RBTree<Key, Value>::getPre(T val) {
//    Node* now = _root;
//    _hot = nullptr;
//    while (now) {
//        if (now->val < val) {
//            _hot = now;
//            now = now->rs;
//        } else now = now->ls;
//    }
//    return iterator(_hot);
//}
//
//template<typename T>
//typename
//RBTree<T>::iterator RBTree<T>::getNxt(T val) {
//    Node* now = _root;
//    _hot = nullptr;
//    while (now) {
//        if (now->val > val) {
//            _hot = now;
//            now = now->ls;
//        } else now = now->rs;
//    }
//    return iterator(_hot);
//}
#endif //DATA_STRUCTURE_HOMEWORK_RBTREE_HPP
