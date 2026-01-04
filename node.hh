#pragma once
template <typename K, typename V>
struct node_t {
    K _key;
    V _val;
    node_t* _forward[1]; // tail array hack

    node_t(const K& key, const V& val, int level) : _key(key), _val(val) {
        for (int i = 0; i < level; ++i) _forward[i] = nullptr;
    }

    // Create: exception safe
    static node_t* create(const K& key, const V& val, int level) {
        if (level <= 0) level = 1;
        const size_t size = sizeof(node_t) + size_t(level - 1) * sizeof(node_t*);
        void* mem = ::operator new(size); // may throw
        return new (mem) node_t(key, val, level);
    }

    // Destroy: call dtor and free memory (portable)
    static void destroy(node_t* p) noexcept {
        if (!p) return;
        p->~node_t();
        ::operator delete(static_cast<void*>(p));
    }

    // disable copy/move (avoid shallow copies of trailing array)
    node_t(const node_t&) = delete;
    node_t& operator=(const node_t&) = delete;
    node_t(node_t&&) = delete;
    node_t& operator=(node_t&&) = delete;
};
