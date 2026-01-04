#include <iostream>
#include <vector>
#include <random>
#include <climits>


template <typename K, typename V> 
struct node_t {
    K _key;
    V _val;
    std::vector<node_t*> _forward;
    node_t(K aKey, V aVal, size_t aTowerSize) : _key(aKey), _val(aVal), _forward(aTowerSize+1, nullptr) {}
};


template <typename K, typename V> 
class SkipListOpt {
    using node_type = node_t<K, V>;
    node_type* _head;
    int _maxLevel;
    int _currLevel; // current highest tower
    float _p{0.5};  // p hard coded to `0.5' but it can also be moved to ctor

    // random number generation for Level height
    std::mt19937 _gen;
    std::uniform_real_distribution<float> _dis;
    std::vector<node_type*> _update;


    int randomLevel() {
        // generate a random 32-bit integer
        uint32_t r = _gen();

        // count trailing zeros gives us the level height based on powers of 2
        int lvl = __builtin_ctz(r);
        return (lvl > _maxLevel) ? _maxLevel : lvl;
    }
public:
    SkipListOpt(int aMaxLevel = 16) 
        : _maxLevel(aMaxLevel), _currLevel(0), _gen(std::random_device{}()), _dis(0, 1), _update(aMaxLevel+1, nullptr) {
        
        // initialize head with a "dummy" key (assuming K can handle a default ctor)
        _head = new node_t(K(), V(), _maxLevel);
    }

    ~SkipListOpt() {
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
        node_type* curr = _head;
        
        // set all the pointers in the tower to the correct position for insertion (to follow)
        for (int i=_currLevel; i>=0; --i) {
            while (curr->_forward[i] != nullptr && curr->_forward[i]->_key < aKey)  {
                curr = curr->_forward[i];
            }
            _update[i] = curr;
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
                _update[i] = _head;
            _currLevel = levels;
        }

        node_type* lnode = new node_type(aKey, aVal, levels);

        // insert the new key
        for (int i=levels; i>=0; --i) {
            lnode->_forward[i] = _update[i]->_forward[i];
            _update[i]->_forward[i] = lnode;
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

		for (int i = _currLevel; i >= 0; --i) {
			while (curr->_forward[i] && curr->_forward[i]->_key < aKey) {
				curr = curr->_forward[i];
			}
			_update[i] = curr;
		}

		curr = curr->_forward[0];

		if (curr && curr->_key == aKey) {
			for (int i = 0; i <= _currLevel; ++i) {
				// only update pointers that actually point to the node we are removing
				if (_update[i]->_forward[i] != curr) break;
				_update[i]->_forward[i] = curr->_forward[i];
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
std::ostream& operator<<(std::ostream& os, const SkipListOpt<K, V>& list) {
    return list.print(os);
}
