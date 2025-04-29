#include <iostream>
#include <string>
#include <sstream>
#include <map>

struct AverageCalculator
{
    double total = 0;
    size_t count = 0;

    void add(double size) {
        total += size;
        count++;
    }

    double average() const {
        return count == 0 ? 0 : static_cast<float>(total) / count;
    }
};

std::pair<int, double> free_block_count_and_average_size(const std::map<size_t, size_t>& allocations) 
{
    if (allocations.size() < 2) return {0, 0};
    int count = 0;
    AverageCalculator average_free_block_size;
    for (auto it = allocations.begin(); it != allocations.end(); ++it) {
        auto next_it = std::next(it);
        if (next_it != allocations.end()) 
        {
            if (it->first + it->second < next_it->first) 
            {
                count++;
                average_free_block_size.add(next_it->first - (it->first + it->second));
            }
        }
    }

    return {count, average_free_block_size.average()};
}

double heap_usage(const std::map<size_t, size_t>& allocations) 
{
    if (allocations.empty()) return 1;
    double total = 0;
    for (const auto& [address, size] : allocations) {
        total += size;
    }
    return total / (allocations.rbegin()->first + allocations.rbegin()->second - allocations.begin()->first);
}

int main() {

    std::map<size_t, size_t> allocations;
    std::string line;
    AverageCalculator average_alloc_block_count;
    AverageCalculator average_alloc_block_size;
    AverageCalculator average_free_block_count;
    AverageCalculator average_average_free_block_size;
    AverageCalculator average_heap_usage;


    while (std::getline(std::cin, line)) {
        if (line.starts_with("End")) break;
        if (line.starts_with("Allocated")) 
        {
            std::stringstream s(line);
            std::string str;
            size_t size = 0, address = 0;
            s >> str >> str;
            size = std::stoull(str);
            s >> str >> str >> str;
            address = std::stoull(str, nullptr, 16);
            allocations[address] = size;
        }
        if (line.starts_with("Deallocated")) 
        {
            std::stringstream s(line);
            std::string str;
            size_t address = 0;
            s >> str >> str >> str >> str >> str;
            address = std::stoull(str, nullptr, 16);
            allocations.erase(address);
        }
        if (line.starts_with("Iteration End"))
        {
            average_alloc_block_count.add(allocations.size());
            average_alloc_block_size.add(allocations.empty() ? 0 : allocations.rbegin()->second);
            auto [free_block_count, average_free_block_size] = free_block_count_and_average_size(allocations);
            average_free_block_count.add(free_block_count);
            average_average_free_block_size.add(average_free_block_size);
            average_heap_usage.add(heap_usage(allocations));
        }
    }

    std::cout << std::format("Average allocated block count: {:>8.2f}\n", average_alloc_block_count.average());
    std::cout << std::format("Average allocated block size : {:>8.2f}\n", average_alloc_block_size.average());
    std::cout << std::format("Average free      block count: {:>8.2f}\n", average_free_block_count.average());
    std::cout << std::format("Average free      block size : {:>8.2f}\n", average_average_free_block_size.average());
    std::cout << std::format("Average heap usage           : {:>8.2f}%\n", average_heap_usage.average() * 100);
    return 0;
}