#include <iostream>
#include <vector>


std::vector<std::vector<int>> ReadArray() {
    int amount;
    int length;
    std::cin >> amount >> length;
    std::vector<std::vector<int>> data;
    for (int i = 0; i < amount; ++i) {
        std::vector<int> current(length);
        for (int j = 0; j < length; ++j) {
            std::cin >> current[j];
        }
        data.push_back(current);
    }
    return data;
}


void PrintTable(std::vector<std::vector<int>>& sequences) {
    for (size_t i = 0; i < sequences.size(); ++i) {
        for (auto x : sequences[i]) {
            std::cout << x << " ";
        }
        std::cout << '\n';
    }
    return;
}


void PrintResult(std::vector<std::vector<int>>& sequences) {
    for (size_t i = 0; i < sequences[0].size(); ++i) {
        std::cout << sequences[0][i] << " ";
    }
    std::cout << '\n';
    return;
}


std::vector<int> MergeTwo(std::vector<int>& seq_first, std::vector<int>& seq_second) {
    std::vector<int> result;

    size_t cur_index_first = 0;
    size_t cur_index_second = 0;

    while (cur_index_first != seq_first.size() || cur_index_second != seq_second.size()) {
        if (cur_index_first == seq_first.size()) {
            result.push_back(seq_second[cur_index_second]);
            cur_index_second += 1;
        } else if (cur_index_second == seq_second.size()) {
            result.push_back(seq_first[cur_index_first]);
            cur_index_first += 1;
        } else {
            if (seq_first[cur_index_first] <= seq_second[cur_index_second]) {
                result.push_back(seq_first[cur_index_first]);
                cur_index_first += 1;
            } else {
                result.push_back(seq_second[cur_index_second]);
                cur_index_second += 1;
            }
        }
    }
    return result;
}


void MergeSequences(std::vector<std::vector<int>>& sequences) {
    std::vector<std::vector<int>> assist;
    while (sequences.size() > 1) {

        for (size_t i = 0; i < sequences.size() / 2; ++i) {
            assist.push_back(MergeTwo(sequences[2 * i], sequences[2 * i + 1]));
        }
        if (sequences.size() % 2 == 1) {
            assist.push_back(sequences[sequences.size() - 1]);
        }
        sequences.clear();
        for (auto& x : assist) {
            sequences.push_back(x);
        }
        assist.clear();
    }
    PrintResult(sequences);
    return;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto sequences = ReadArray();
    MergeSequences(sequences);

    return 0;
}
