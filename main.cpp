#include "buffer_level.h"
#include "run.h"
#include "lsm_tree.h"
#include <iostream>


int main(){

    int buffer_size = 2000; 
    BufferLevel test_large(buffer_size);
    
    int search_key = 756;
    for( int i = 0; i < buffer_size; i++){
        if (i == search_key){
            test_large.insert(i, 42069);
        } else {
            test_large.insert(i, 42);
        }
    }

    std::vector<Entry_t> vec_large = test_large.convert_tree_to_vector();
    std::vector<KEY_t> fence_pointer;
    BloomFilter bloom_large(20000); 

    test_large.save_to_memory("fillname.dat", &fence_pointer, vec_large);
    test_large.create_bloom_filter(&bloom_large, vec_large);

    // add test for the run functionalities. 
    std::cout << "\ntesting using basic run functionalities: \n" << std::endl;

    Run run1("fillname.dat", &bloom_large, &fence_pointer);
    std::unique_ptr<Entry_t> entry = nullptr;
  
    std::cout << run1.search_bloom(search_key) << std::endl;

    //print the fence pointers
    std::cout << "reading from fence pointers: " << std::endl;
    for (int i = 0; i < fence_pointer.size(); ++i) {
        std::cout << fence_pointer[i];
        if (i < fence_pointer.size() - 1) {
            std::cout << ", "; // Print a comma between elements, but not after the last element
        }
    }
    std::cout << std::endl; // Print a newline at the end

    int starting_point = run1.search_fence(search_key);
    std::cout << starting_point << std::endl;
    if (starting_point != -1){
        entry = run1.disk_search(starting_point, 512, search_key);
        std::cout << entry->key << std::endl;
    }

    if (entry){
        std::cout << "we found value: " << entry->val << std::endl; 
    } else { 
        std::cout << "value not found!" <<std::endl; 
    }

    // Testing for constructing the LSM tree. 
    std::cout << "\nTesting LSM tree construction\n" << std::endl;

    LSM_Tree lsm_tree(10, 3);

    std::cout << "The Root node has these attributes"; 

    std::cout << "\nlevel:" << lsm_tree.root->level << std::endl; 
    std::cout << "num of runs:" << lsm_tree.root->max_num_of_runs << std::endl; 
    std::cout << "Run_storage:" << typeid(lsm_tree.root->run_storage).name() << std::endl; 

    //inserting into buffer in LSM tree. 
    std::cout << "The Root node has these attributes" << std::endl;

    for( int i = 0; i < 1000000; i++){
        if (i == search_key){
            lsm_tree.buffer_insert(i, 42069);
        } else {
            lsm_tree.buffer_insert(i, 42);
        }   
    }

    std::cout << "\nTesting LSM tree search" << std::endl;
    KEY_t test_key_1 = 20;
    auto result = lsm_tree.search_value(test_key_1);
    if (result) {
        std::cout << "Searching on disk with key 20, found value: " << result->val << std::endl;
    } else {
        std::cout << "Searching on disk with key 20, value not found" << std::endl;
    }

    result = lsm_tree.search_value(search_key);
    if (result) {
        std::cout << "Searching on disk with key 756, found value: " << result->val << std::endl;
    } else {
        std::cout << "Searching on disk with key 756, value not found" << std::endl;
    }

    // TODO: segmentation error when value not found. Something wrong. 
    result = lsm_tree.search_value(3000);
    if (result) {
        std::cout << "Searching in memory with key 3001, found value: " << result->val << std::endl;
    } else {
        std::cout << "Searching in memory with key 3001, value not found" << std::endl;
    }

    // TODO: there is a bug here when writing to or coming back from binary files. Need to fix. 


    return 0;
};