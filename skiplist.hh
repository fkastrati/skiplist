#pragma once

//
//  skiplist.hh
//
//  Created by Fisnik Kastrati on 03.01.2026.
//

#include <iostream>
#include <vector>
#include <random>
#include <climits>

template <typename K, typename V> 
class SkipList {
    struct node_t {
        K _key;
        V _val;
        std::vector<node_t*> _forward;
        node_t(K aKey, V aVal, size_t aTowerSize) : _key(aKey), _val(aVal), _forward(aTowerSize+1, nullptr) {}
    };
    using node_type = node_t;
    node_type* _head;
    int _maxLevel;
    int _currLevel; // current highest tower
    float _p{0.5};  // p hard coded to `0.5' but it can also be moved to ctor

    // random number generation for Level height
    std::mt19937 _gen;
    std::uniform_real_distribution<float> _dis;

    int randomLevel() {
        int lvl = 0;
        while (_dis(_gen) < _p && lvl < _maxLevel) {
            lvl++;
        }
        return lvl;
    }

    // formula: max_level = ceil(log(n) / log(1/p))
    inline int computeMaxLevel(size_t expectedElements) {
        if (expectedElements <= 1) return 1;
        
        double logP = std::log(1.0 / _p);
        double logN = std::log(static_cast<double>(expectedElements));
        return static_cast<int>(std::ceil(logN / logP));
    }
public:
    SkipList(int aNodes) 
        : _maxLevel(computeMaxLevel(aNodes)), _currLevel(0), _gen(std::random_device{}()), _dis(0, 1) {
        
        // initialize head with a "dummy" key (assuming K can handle a default ctor)
        _head = new node_t(K(), V(), _maxLevel);
    }

    ~SkipList() {
        auto curr = _head->_forward[0];
        while (curr != nullptr) {
            auto tmp = curr;
            curr = curr->_forward[0];
            delete tmp;
        }
        delete _head;
    }

public:
    void insert(K aKey, V aVal) {
        std::vector<node_type*> update(_maxLevel+1, nullptr);
        node_type* curr = _head;
        
        // set all the pointers in the tower to the correct position for insertion (to follow)
        for (int i=_currLevel; i>=0; --i) {
            while (curr->_forward[i] != nullptr && curr->_forward[i]->_key < aKey)  {
                curr = curr->_forward[i];
            }
            update[i] = curr;
        }

        curr = curr->_forward[0];

        // check if we already have the key
        if (curr && curr->_key == aKey) {
            curr->_val = aVal;
            return;
        }

        int levels = randomLevel();

        if (levels > _currLevel) {
            for (int i=_currLevel + 1; i<=levels; ++i)
                update[i] = _head;
            _currLevel = levels;
        }

        node_type* lnode = new node_type(aKey, aVal, levels);

        // insert the new key
        for (int i=levels; i>=0; --i) {
            lnode->_forward[i] = update[i]->_forward[i];
            update[i]->_forward[i] = lnode;
        }
    }

    V* search(K aKey) {
        node_type * curr = _head;

        for (int i=_currLevel; i>=0; --i) {
            while (curr->_forward[i] && curr->_forward[i]->_key < aKey)  {
                curr = curr->_forward[i];
            }
        }
        
        curr = curr->_forward[0];

        if (curr != nullptr && curr->_key == aKey) {
            return &(curr->_val);
        }

        return nullptr;
    }


	void erase(K aKey) {
		node_type* curr = _head;
		std::vector<node_type*> update(_maxLevel + 1, nullptr);

		for (int i = _currLevel; i >= 0; --i) {
			while (curr->_forward[i] && curr->_forward[i]->_key < aKey) {
				curr = curr->_forward[i];
			}
			update[i] = curr;
		}

		curr = curr->_forward[0];

		if (curr && curr->_key == aKey) {
			for (int i = 0; i <= _currLevel; ++i) {
				// only update pointers that actually point to the node we are removing
				if (update[i]->_forward[i] != curr) break;
				update[i]->_forward[i] = curr->_forward[i];
			}
			delete curr;

			// shrink the current level if the tallest node was removed
			while (_currLevel > 0 && _head->_forward[_currLevel] == nullptr) {
				--_currLevel;
			}
		}
	}

    std::ostream& print(std::ostream& os) const {
        for (int i=_currLevel; i>=0; --i) {
            node_type* curr = _head;
            os << i << ": ";
            while (curr->_forward[i] != nullptr)  {
                curr = curr->_forward[i];
                os << curr->_key << " -> "; 
            }
            os << "Nil\n";
        }
        return os;
    }
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const SkipList<K, V>& list) {
    return list.print(os);
}
