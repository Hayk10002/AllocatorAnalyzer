#include <iostream>
#include <random>
#include <format>
#include <vector>
#include <algorithm>

int generateRandomNumber(int min, int max) {
    static std::random_device rd; 
    static std::mt19937 eng(rd()); 
    std::uniform_int_distribution<> distr(min, max); 
    return distr(eng); 
}

template<class T>
class LoggingAllocator : public std::allocator<T>
{
public:
    using size_type = typename std::allocator<T>::size_type;

    T* allocate(size_type n) {
        T* p = std::allocator<T>::allocate(n);
        std::cout << "Allocated " << n * sizeof(T) << " bytes, address " << p << std::endl;
        return p;
    }

    void deallocate(T* p, size_type n) {
        std::cout << "Deallocated " << n * sizeof(T) << " bytes, address " << p << std::endl;
        std::allocator<T>::deallocate(p, n);
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
    
    std::vector<std::vector<int, LoggingAllocator<int>>, LoggingAllocator<std::vector<int, LoggingAllocator<int>>>> arr;
    for (int i = 0; i < iter_count; i++)
    {
        arr.emplace_back(generateRandomNumber(1, 10000));
        arr.erase(std::remove_if(arr.begin(), arr.end(), [prob](const auto&) {
            return generateRandomNumber(1, 100) < prob;
        }), arr.end());
    }
    std::cout << "End" << std::endl;
    return 0;
}