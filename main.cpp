//
//  main.cpp
//
//  Created by Fisnik Kastrati on 03.01.2026.
//


#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
#include <string>
#include <map>

#include "node.hh"
#include "skiplist_opt.hh"
#include "skiplist.hh"
#include "skiplist_super.hh"

int main(int argc, char** argv) {
    using namespace std;

    size_t N = 10'000'000;
    if (argc >= 2) N = stoull(argv[1]);
    cout << "Benchmark N = " << N << "\n";

    // generate keys 0..N-1 and shuffle
    vector<uint64_t> keys; keys.reserve(N);
    for (uint64_t i = 0; i < N; ++i) keys.push_back(i);
    mt19937_64 rng(123456);
    shuffle(keys.begin(), keys.end(), rng);

    // SkipList test
    {
        SkipListOpt<uint64_t, uint64_t> skip(N);
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) skip.insert(keys[i], keys[i]);
        auto t1 = chrono::high_resolution_clock::now();
        cout << "SkipList Opt Insert Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms\n";

        t0 = chrono::high_resolution_clock::now();
        size_t found = 0;
        for (size_t i = 0; i < N; ++i) if (skip.search(keys[i])) ++found;
        t1 = chrono::high_resolution_clock::now();
        cout << "SkipList Opt Search Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms (found=" << found << ")\n";

        t0 = chrono::high_resolution_clock::now();
        size_t removed = 0;
        for (size_t i = 0; i < N; ++i) {
            skip.erase(keys[i]); ++removed;
        }
        t1 = chrono::high_resolution_clock::now();
        cout << "SkipList Opt Remove Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms (removed=" << removed << ")\n";
    }
    // SkipList Unopt test
    {
        // SkipList<uint64_t, uint64_t> skip(expected);
        // auto t0 = chrono::high_resolution_clock::now();
        // for (size_t i = 0; i < N; ++i) skip.insert(keys[i], keys[i]);
        // auto t1 = chrono::high_resolution_clock::now();
        // cout << "SkipList Insert Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms\n";

        // t0 = chrono::high_resolution_clock::now();
        // size_t found = 0;
        // for (size_t i = 0; i < N; ++i) if (skip.search(keys[i])) ++found;
        // t1 = chrono::high_resolution_clock::now();
        // cout << "SkipList Search Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms (found=" << found << ")\n";

        // t0 = chrono::high_resolution_clock::now();
        // size_t removed = 0;
        // for (size_t i = 0; i < N; ++i) skip.erase(keys[i]); 
        // t1 = chrono::high_resolution_clock::now();
        // cout << "SkipList Remove Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms\n";
    }
    // std::map test
    {
        map<uint64_t, uint64_t> m;
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) m.emplace(keys[i], keys[i]);
        auto t1 = chrono::high_resolution_clock::now();
        cout << "std::map Insert Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms\n";

        t0 = chrono::high_resolution_clock::now();
        size_t found = 0;
        for (size_t i = 0; i < N; ++i) if (m.find(keys[i]) != m.end()) ++found;
        t1 = chrono::high_resolution_clock::now();
        cout << "std::map Search Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms (found=" << found << ")\n";

        t0 = chrono::high_resolution_clock::now();
        size_t removed = 0;
        for (size_t i = 0; i < N; ++i) removed += (m.erase(keys[i]) > 0);
        t1 = chrono::high_resolution_clock::now();
        cout << "std::map Remove Time: " << chrono::duration<double, milli>(t1 - t0).count() << " ms (removed=" << removed << ")\n";
    }

    return 0;
}


