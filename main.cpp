#include <iostream>
#include <string>
#include <random>
#include <map>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int roll_dice(int num_faces)
{
    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(1, num_faces);
    return distribution(generator);
}

int dice(const string &input)
{
    int total = 0;
    size_t start = 0;

    while (start < input.length()) {
        size_t end = input.find(',', start);
        if (end == string::npos) {
            end = input.length();
        }

        string spec = input.substr(start, end - start);
        size_t d_pos = spec.find('d');
        size_t mod_pos = spec.find_first_of('+', d_pos);

        int num_dice = stoi(spec.substr(0, d_pos));
        int num_faces = stoi(spec.substr(d_pos + 1, mod_pos - d_pos - 1));
        int modifier = (mod_pos != string::npos) ? stoi(spec.substr(mod_pos)) : 0;

        int roll_sum = 0;
        for (int i = 0; i < num_dice; ++i) {
            roll_sum += roll_dice(num_faces);
        }
        total += roll_sum + modifier;

        start = end + 1;
    }
    return total;
}

void build_histogram(const vector<int> &results, const string &filename) {
    map<int, int> frequency;

    for (int result : results) {
        frequency[result]++;
    }

    int max_count = 0;
    for (const auto &entry : frequency) {
        if (entry.second > max_count) {
            max_count = entry.second;
        }
    }

    ofstream file(filename);

    for (int i = max_count; i > 0; --i) {
        for (const auto &entry : frequency) {
            if (entry.second >= i) {
                file << "|  ";
            } else {
                file << "   ";
            }
        }
        file << endl;
    }

    for (const auto &entry : frequency) {
        file << entry.first << setw(3);
    }
    file << endl;
}

int main() {
    vector<string> dice_specs = {"1d6", "2d6", "3d6", "1d10", "2d10", "3d10", "2d6+2,3d10"};

    fs::create_directory("results");
    int trials = 1000;

    for (const string &dice_spec : dice_specs) {
        cout << "Rolling: " << dice_spec << endl;
        vector<int> results(trials);

        for (int i = 0; i < trials; ++i) {
            results[i] = dice(dice_spec);
        }

        string filename = "results/" + dice_spec + "_histogram.txt";
        build_histogram(results, filename);
    }
    return 0;
}