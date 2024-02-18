#include "bigint.h"
#include "bigfloat.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace aak;

const int threads = 16;

bigfloat pi=0_bf;

bigfloat four=4_bf;
bigfloat two=2_bf;
bigfloat one=1_bf;

std::vector<bigfloat> v;

void foo(int l, int r) {
    static std::mutex m;
    bigfloat add=0_bf;
    for (int i=l; i<r; ++i) {
        add += ((four / (8 * i + 1)) - (two / (8 * i + 4)) - (one / (8 * i + 5)) - (one / (8 * i + 6))) / v[i];
    }
    m.lock();
    pi+=add;
    m.unlock();
}

int main() {
    std::cout << "Enter precision" << std::endl;
    int n;
    std::cin >> n;
    int len=n;

    if (n==0) n=threads;
    else while (n%threads!=0) ++n;

    auto start = std::chrono::high_resolution_clock::now();
    bigfloat base = 1_bf;
    v.resize(n);
    for (int k = 0; k < v.size(); k++) {
        v[k] = base;
        base *= 16_bf;
    }

    std::vector<std::thread> vv(threads);
    for (int i = 0; i < vv.size(); ++i) {
        vv[i] = std::thread(foo, i*(n/threads), (i+1)*(n/threads));
    }
    for (auto &i: vv) i.join();

    std::cout << pi.decimal(len) << std::endl;
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << duration.count() << "ms" << std::endl;

    return 0;
}
