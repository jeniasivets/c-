#include <iostream>
#include <vector>
#include <ctime>
#include <random>

class HashFunction {
public:
    long long GetHashValue(long long argument) const;
    void GenerateParameters(size_t size);
    
private:
    static const long long prime_p = 2147483647;
    static std::mt19937_64 generator;
    long long first_coefficient;
    long long second_coefficient;
    long long size_of_table;
};

std::mt19937_64 HashFunction::generator(static_cast<unsigned int>(time(0)));

long long HashFunction::GetHashValue(long long argument) const {
    if (argument < 0) {
        return GetHashValue(argument + prime_p);
    }
    if (size_of_table == 0) {
        return 0;
    }
    long long result = ((argument * first_coefficient +
                         second_coefficient) % prime_p) % size_of_table;
    
    return result;
}


void HashFunction::GenerateParameters(size_t size) {
    first_coefficient = generator() % prime_p;
    second_coefficient = generator() % prime_p;
    size_of_table = size;
}

struct OneCell_in_bucket {
    OneCell_in_bucket() {
        value = 0;
        cell_is_empty = true;
    }
    long long value;
    bool cell_is_empty;
};


class SecondLevelHashTable {
public:
    void Initialize(const std::vector<long long> &vect_values);
    bool Contains(const long long checked_elem) const;
    
private:
    HashFunction hash_function;
    std::vector<OneCell_in_bucket> hash_table_of_bucket;
    void GenerateHash(size_t vect_len);
    bool CheckCollisionsInHash(const std::vector<long long> &vect_values);
};

void SecondLevelHashTable::GenerateHash(size_t vect_len) {
    long long square_size = vect_len * vect_len;
    hash_function.GenerateParameters(square_size);
}


bool SecondLevelHashTable::CheckCollisionsInHash(const std::vector<long long> &vect_values) {
    bool bad_hash = false;
    for (int it = 0; it < static_cast<int>(vect_values.size()); ++it) {
        long long index = hash_function.GetHashValue(vect_values[it]);
        if (!(hash_table_of_bucket[index].cell_is_empty)) {
            bad_hash = true;
        }
        
        hash_table_of_bucket[index].cell_is_empty = false;
        hash_table_of_bucket[index].value = vect_values[it];
    }
    return bad_hash;
}

void SecondLevelHashTable::Initialize(const std::vector<long long> &vect_values) {
    long long square_table_size = vect_values.size() * vect_values.size();
    hash_table_of_bucket.assign(square_table_size, OneCell_in_bucket());
    GenerateHash(vect_values.size());
    
    while (CheckCollisionsInHash(vect_values)) {
        hash_table_of_bucket.assign(square_table_size, OneCell_in_bucket());
        GenerateHash(vect_values.size());
    }
}

bool SecondLevelHashTable::Contains(const long long checked_elem) const {
    if (hash_table_of_bucket.empty()) {
        return false;
    }
    long long second_level_hash = hash_function.GetHashValue(checked_elem);
    
    if (hash_table_of_bucket[second_level_hash].cell_is_empty) {
        return false;
    }
    return (hash_table_of_bucket[second_level_hash].value == checked_elem);
}

