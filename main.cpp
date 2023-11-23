#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>

void worker(std::queue<std::pair<int, std::string>>& data, std::mutex mutex, std::vector<std::pair<int, int>> ret) {
    while (true) {
        mutex.lock();
        if (data.empty()) {
            return;
        }
        auto t = data.front();
        data.pop();
        mutex.unlock();
        std::stringstream ss(t.second);
        int val, a, b;
        ss >> val >> a >> b;
        if (val == 1) {
            ret.emplace_back(t.first, a+b);
            continue;
        }
        if (val == 2) {
            ret.emplace_back(t.first, a*b);
            continue;
        }
        if (val == 3) {
            ret.emplace_back(t.first, (a*a)+(b*b));
            continue;
        }
    }
}


std::vector<std::pair<int, int>> MultiProcEval(std::queue<std::pair<int, std::string>> data, int num_workers) {
    std::vector<std::pair<int, int>> ret;
    std::mutex mutex;
    std::vector<std::thread> threads;
    std::vector<std::vector<std::pair<int, int>>> rets(num_workers);

    for (int i = 0; i < num_workers; ++i) {
        threads.emplace_back(&worker, std::ref(data), std::ref(mutex), std::ref(rets[i]));
    }
    for (int i = 0; i < num_workers; ++i) {
        threads[i].join();
        ret.insert(ret.end(), rets[i].begin(), rets[i].end());
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}


int main()
{

}
