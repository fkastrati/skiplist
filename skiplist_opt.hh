//
//  skiplist_opt.hh
//
//  Created by Fisnik Kastrati on 03.01.2026.
//
#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <bit>

template <typename K, typename V>
class SkipListOpt {
    using node_type = node_t<K, V>;
    node_type* _head;
    int _maxLevel;           // maximum level index
    int _currLevel;          // current highest occupied level index
    float _p{0.5f};

    std::vector<node_type*> _update;

    uint64_t _rng_state;
    
    uint64_t xorshift64() {
        uint64_t x = _rng_state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        _rng_state = x;
        return x;
    }
    
    int randomLevel() {
        uint32_t r = static_cast<uint32_t>(xorshift64());
        r |= (r == 0); // ensure non-zero
        int lvl = __builtin_ctz(r);
        return (lvl > _maxLevel) ? _maxLevel : lvl;
    }

    int computeMaxLevel(size_t n) {
        if (n <= 1) return 1;
        return std::bit_width(n - 1);
    }

public:
    SkipListOpt(int aNnodes)
        : _maxLevel(computeMaxLevel(aNnodes)), _currLevel(0), _update(_maxLevel + 1, nullptr)
    {
        _head = node_type::create(K(), V(), _maxLevel + 1);
    }

    ~SkipListOpt() {
        // destroy all the nodes in our skiplist
        node_type* curr = _head->_forward[0];
        while (curr != nullptr) {
            node_type* tmp = curr;
            curr = curr->_forward[0];
            node_type::destroy(tmp);
        }
        node_type::destroy(_head);
    }

    void insert(const K& aKey, const V& aVal) {
        node_type* curr = _head;
        for (int i = _currLevel; i >= 0; --i) {
            while (curr->_forward[i] && curr->_forward[i]->_key < aKey) {
                curr = curr->_forward[i];
            }
            _update[i] = curr;
        }

        curr = curr->_forward[0];

        if (curr && curr->_key == aKey) {
            curr->_val = aVal;
            return;
        }

        int levels = randomLevel();
        if (levels > _currLevel) {
            for (int i = _currLevel + 1; i <= levels; ++i)
                _update[i] = _head;
            _currLevel = levels;
        }

        node_type* lnode = node_type::create(aKey, aVal, levels + 1);
        
        for (int i = levels; i >= 0; --i) {
            lnode->_forward[i] = _update[i]->_forward[i];
            _update[i]->_forward[i] = lnode;
        }
    }

    // V* search(const K& aKey) {
    //     node_type* curr = _head;
    //     for (int i = _currLevel; i >= 0; --i) {
    //         while (curr->_forward[i] && curr->_forward[i]->_key < aKey) {
    //             curr = curr->_forward[i];
    //         }
    //     }
    //     curr = curr->_forward[0];
    //     if (curr && curr->_key == aKey) return &curr->_val;
    //     return nullptr;
    // }

    // search with prefetching
    V* search(const K& aKey) {
        node_type* curr = _head;
        
        for (int i = _currLevel; i >= 0; --i) {
            node_type* next = curr->_forward[i];
            
            while (next && next->_key < aKey) {
                __builtin_prefetch(next->_forward[i], 0, 1);
                curr = next;
                next = curr->_forward[i];
            }
            
            if (next && next->_key == aKey) {
                return &next->_val;
            }
        }
        
        return nullptr;
    }

    void erase(const K& aKey) {
        node_type* curr = _head;
        for (int i = _currLevel; i >= 0; --i) {
            node_type* next = curr->_forward[i];

            while (next && next->_key < aKey) {
                __builtin_prefetch(next->_forward[i], 0, 1);
                curr = next;
                next = curr->_forward[i];
            }
            _update[i] = curr;
        }

        curr = curr->_forward[0];

        if (curr && curr->_key == aKey) {
            for (int i = 0; i <= _currLevel; ++i) {
                if (_update[i]->_forward[i] != curr) break;
                _update[i]->_forward[i] = curr->_forward[i];
            }
            node_type::destroy(curr);

            // adjust current level if necessary
            while (_currLevel > 0 && _head->_forward[_currLevel] == nullptr) {
                --_currLevel;
            }
        }
    }

    std::ostream& print(std::ostream& os) const {
        for (int i = _currLevel; i >= 0; --i) {
            const node_type* curr = _head;
            os << i << ": ";
            while (curr->_forward[i] != nullptr) {
                curr = curr->_forward[i];
                os << curr->_key << " -> ";
            }
            os << "Nil\n";
        }
        return os;
    }
};

template <typename K, typename V, int TMaxLevel>
std::ostream& operator<<(std::ostream& os, const SkipListOpt<K, V>& list) {
    return list.print(os);
}
