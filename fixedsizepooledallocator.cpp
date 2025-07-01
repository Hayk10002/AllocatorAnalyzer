#include <iostream>
#include <random>
#include <format>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

int generateRandomNumber(int min, int max) {
    static std::random_device rd; 
    static std::mt19937 eng(rd()); 
    std::uniform_int_distribution<> distr(min, max); 
    return distr(eng); 
}

const int MEMORY_POOL_SIZE = 1024 * 1024 * 8;
const int MEMORY_BLOCK_SIZE = 1024 * 8;
unsigned char memory_pool[MEMORY_POOL_SIZE];

std::set<int> free_blocks{0};

template<class T>
class LoggingAllocator : public std::allocator<T>
{
public:
    using size_type = typename std::allocator<T>::size_type;

    T* allocate(size_type n) {
        if (n != MEMORY_BLOCK_SIZE / sizeof(T)) {
            throw std::bad_alloc();
        }
        if (free_blocks.empty()) 
            throw std::bad_alloc();

        int block_index;
        
        block_index = *(free_blocks.begin());
        free_blocks.erase(free_blocks.begin());
        
        if (free_blocks.empty()) free_blocks.insert(block_index + 1);

        T* p = reinterpret_cast<T*>(memory_pool + block_index * MEMORY_BLOCK_SIZE);
        std::cout << "Allocated " << MEMORY_BLOCK_SIZE << " bytes, address " << p << std::endl;
        return p;
    }

    void deallocate(T* p, size_type n) {
        if (n != MEMORY_BLOCK_SIZE / sizeof(T)) {
            return;
        }
        int block_offset = reinterpret_cast<unsigned char*>(p) - memory_pool;
        if (block_offset < 0 || block_offset >= MEMORY_POOL_SIZE || block_offset % MEMORY_BLOCK_SIZE != 0) {
            throw std::invalid_argument("Invalid pointer for deallocation");
        }
        int block_index = block_offset / MEMORY_BLOCK_SIZE;
        free_blocks.insert(block_index);
        if (*(free_blocks.rbegin()) == *(++free_blocks.rbegin())) {
            free_blocks.erase(--free_blocks.end());
        }
        std::cout << "Deallocated " << MEMORY_BLOCK_SIZE << " bytes, address " << p << std::endl;
    }
};


int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <probability> <iteration_count>" << std::endl;
        return 1;
    }

    int prob = std::stoi(argv[1]);
    int iter_count = std::stoi(argv[2]);
    {
        std::vector<std::vector<int, LoggingAllocator<int>>> arr;
        for (int i = 0; i < iter_count; i++)
        {
            arr.emplace_back(MEMORY_BLOCK_SIZE / sizeof(int));
            arr.erase(std::remove_if(arr.begin(), arr.end(), [prob](const auto&) {
                return generateRandomNumber(1, 100) <= prob;
            }), arr.end());

            std::cout << "Iteration End" << std::endl;
        }
    }
    std::cout << "Iteration End" << std::endl;
    std::cout << "End" << std::endl;
    return 0;
}