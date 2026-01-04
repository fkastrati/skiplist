#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
#include <string>
#include <map>

#define OPT

#ifndef OPT
#include "skiplist.hh"
#else
#include "skiplist_opt.hh"
#endif

void run_test_harness_map(const int num_elements) {
    std::map<int, int> map;
    std::vector<int> keys;
    for (int i = 0; i < num_elements; ++i) keys.push_back(i);
    // shuffle keys for random insertion order
    std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device{}()));
    std::cout << "--- Testing std::map (Elements: " << num_elements << ") ---\n";
    
    // 1. measure Insertion
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        map[k] = k;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> insert_time = end - start;
    std::cout << "Insert Time: " << insert_time.count() << " ms\n";
    
    // 2. measure Search
    std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device{}()));
    start = std::chrono::high_resolution_clock::now();
    long long sum = 0; // Use a side effect
    for (int k : keys) {
        auto it = map.find(k);
        if (it != map.end()) sum += it->second; // Forces the compiler to actually perform the search
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> search_time = end - start;
    // Do something with 'sum' at the end so it's not optimized away
    if (sum == -1) std::cout << "This won't happen";
    std::cout << "Search Time: " << search_time.count() << " ms\n";
    
    // 3. measure Removal
    start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        map.erase(k);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> remove_time = end - start;
    std::cout << "Remove Time: " << remove_time.count() << " ms\n";
    std::cout << "-----------------------------------------------\n\n";
}

void run_test_harness(const int num_elements, const int max_lvl) {
#ifndef OPT
    SkipList<int, int> skipList(max_lvl);
#else
    SkipListOpt<int, int> skipList(max_lvl);
#endif
    std::vector<int> keys;
    for (int i = 0; i < num_elements; ++i) keys.push_back(i);
    
    // shuffle keys for random insertion order
    std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device{}()));

    std::cout << "--- Testing SkipList (Elements: " << num_elements 
              << ", Max Levels: " << max_lvl << ") ---\n";

    // 1. measure Insertion
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        skipList.insert(k, k);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> insert_time = end - start;
    std::cout << "Insert Time: " << insert_time.count() << " ms\n";

    // 2. measure Search
    std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device{}()));
    start = std::chrono::high_resolution_clock::now();
    long long sum = 0; // Use a side effect
    for (int k : keys) {
        int* val = skipList.search(k);
        if (val) sum += *val; // Forces the compiler to actually perform the search
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> search_time = end - start;
    // Do something with 'sum' at the end so it's not optimized away
    if (sum == -1) std::cout << "This won't happen";
    std::cout << "Search Time: " << search_time.count() << " ms\n";

    // 3. measure Removal
    start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        skipList.erase(k);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> remove_time = end - start;
    std::cout << "Remove Time: " << remove_time.count() << " ms\n";
    std::cout << "-----------------------------------------------\n\n";
}

int main(const int argc, const char** argv) {

    if (argc != 2) {
        std::cerr << "usage: ./main <nr_levels>" << std::endl;
        return -1;
    }

    int levels = std::atoi(argv[1]);

    // Basic test
    //run_test_harness(1000'000, levels);
    // Performance test
    //run_test_harness(10'000'000, levels);

    SkipListOpt<int, std::string> skp(levels);

    skp.insert(1, "Foo");
    skp.insert(4, "Bar");
    skp.insert(2, "Baz");
    skp.insert(3, "Zoo");
    skp.insert(5, "Qenef");
    skp.insert(6, "Goo");

    return 0;
}
