#include <vector>
#include <random>
#include <optional>


class HashFunction {
public:
    HashFunction();
    HashFunction(int64_t alpha, int64_t beta, int p_m = kIntMax);
    int CalculateHash(int number) const;
    template <typename RandomGenerator>
    static HashFunction BuildHashFunction(RandomGenerator& generator); 

private:
    int64_t alpha_;
    int64_t beta_;
    int p_max_;
    static const int kIntMax = 2147483647;
    static const int kMaxAbsValue = 1000000000;
};


class InnerHashTable {
public:
    InnerHashTable();
    template <typename RandomGenerator>
    void Initialize(const std::vector<int>& chain, RandomGenerator& generator);
    bool Contains(int number) const;
  
private:
    std::vector<std::optional<int>> cells_;
    HashFunction hash_function_;
    bool ProbeInnerHashFunction(const std::vector<int>& chain);\
    static const int kAlphaFactor = 2;
};


class FixedSet {
public:
    FixedSet();
    void Initialize(const std::vector<int>& numbers);
    bool Contains(int number) const;

private:
    std::vector<InnerHashTable> hash_table_;
    HashFunction hash_function_out_;
    bool ProbeOuterHashFunction(const std::vector<int>& lengths, size_t count);
    void InitializeChains(std::vector<std::vector<int>>& chains,
                          const std::vector<int>& numbers);
    template <typename RandomGenerator>
    void InitializeHashTable(const std::vector<std::vector<int>>& chains,
                             RandomGenerator& generator);
    static const int kOuterProbe = 4;    
};


std::vector<int> CalculateBucketDistribution(HashFunction hash, const std::vector<int>& numbers,
                                             size_t table_size);


// ==========IMPLEMENTATION========== //


HashFunction::HashFunction() {}


HashFunction::HashFunction(int64_t alpha, int64_t beta, int p_m) :
    alpha_(alpha), beta_(beta), p_max_(p_m) {}


int HashFunction::CalculateHash(int number) const {
    return (alpha_ % p_max_ * (number + kMaxAbsValue) + beta_ % p_max_) % p_max_;
}


template <typename RandomGenerator>
HashFunction HashFunction::BuildHashFunction(RandomGenerator& generator) {
    std::uniform_int_distribution<int64_t> dist_alpha(1, kIntMax);
    std::uniform_int_distribution<int64_t> dist_beta(0, kIntMax);

    return {dist_alpha(generator), dist_beta(generator)};    
}


InnerHashTable::InnerHashTable() {}


template <typename RandomGenerator>
void InnerHashTable::Initialize(const std::vector<int>& chain, RandomGenerator& generator) {
    cells_.clear();
    do {
        hash_function_ = HashFunction::BuildHashFunction(generator);
    } while (ProbeInnerHashFunction(chain));
    
    cells_.resize(kAlphaFactor * chain.size() * chain.size(), std::nullopt);
    
    for (auto number : chain) {
        cells_[hash_function_.CalculateHash(number) % cells_.size()] = number;
    }
}


bool InnerHashTable::Contains(int number) const {
    if (cells_.empty()) {
        return false;
    }
    return cells_[hash_function_.CalculateHash(number) % cells_.size()] &&
          *cells_[hash_function_.CalculateHash(number) % cells_.size()] == number;
}


bool InnerHashTable::ProbeInnerHashFunction(const std::vector<int>& chain) {
    size_t chain_module = kAlphaFactor * chain.size() * chain.size();

    auto lengths = CalculateBucketDistribution(hash_function_, chain, chain_module);

    for (auto length : lengths) {
        if (length > 1) {
            return true;
        }
    }

    return false;
}


std::vector<int> CalculateBucketDistribution(HashFunction hash, const std::vector<int>& numbers,
                                             size_t table_size) {
    std::vector<int> lengths(table_size, 0);

    for (auto number : numbers) {
        ++lengths[hash.CalculateHash(number) % table_size];
    }

    return lengths;
}


FixedSet::FixedSet() {}


void FixedSet::Initialize(const std::vector<int>& numbers) {
    hash_table_.clear();
    std::mt19937 generator;
    std::vector<int> lengths;

    do {
        hash_function_out_ = HashFunction::BuildHashFunction(generator);
        lengths = CalculateBucketDistribution(hash_function_out_, numbers, numbers.size());
    } while (ProbeOuterHashFunction(lengths, numbers.size()));

    std::vector<std::vector<int>> chains(numbers.size());

    InitializeChains(chains, numbers);
    InitializeHashTable(chains, generator);
}


bool FixedSet::Contains(int number) const {
    size_t m_mod = hash_table_.size();
    if (m_mod == 0) {
        return false;
    }
    return hash_table_[hash_function_out_.CalculateHash(number) % m_mod].Contains(number);
}


bool FixedSet::ProbeOuterHashFunction(const std::vector<int>& lengths, size_t count) {
    size_t sum = 0;
    for (auto x : lengths) {
        sum += x * x;
    }
    return sum > kOuterProbe * count;
}


void FixedSet::InitializeChains(std::vector<std::vector<int>>& chains,
                                const std::vector<int>& numbers) {
    size_t numbers_module = numbers.size();
    for (auto number : numbers) {
        chains[hash_function_out_.CalculateHash(number) % numbers_module].push_back(number);
    }
}


template <typename RandomGenerator>
void FixedSet::InitializeHashTable(const std::vector<std::vector<int>>& chains,
                         RandomGenerator& generator) {
    for (auto chain : chains) {
        InnerHashTable current_table;
        current_table.Initialize(chain, generator);
        hash_table_.push_back(current_table);
    }
}
