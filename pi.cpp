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
bigfloat eight=8_bf;
bigfloat five=5_bf;
bigfloat six=6_bf;
bigfloat sixteen=16_bf;

int p;

bigfloat binpow(bigfloat x, int y) {
    if (y==0) return 1_bf;
    if (y==1) return x;
    bigfloat res=binpow(x,y/2);
    if (y%2==0) return res*res;
    return res*res*x;
}

void foo(int l, int r) {
    static std::mutex m;
    bigfloat b=binpow(sixteen,l);
    bigfloat add=0_bf;
    for (int i=l; i<r; ++i) {
        add += ((four / (eight * i + one)) - (two / (eight * i + four)) - (one / (eight * i + five)) - (one / (eight * i + six))) / b;
        b*=sixteen;

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

    p=(n+7)/8;
    four=bigfloat(4,p);
    two=bigfloat(2,p);
    one=bigfloat(1,p);
    sixteen=bigfloat(16,p);

    auto start = std::chrono::high_resolution_clock::now();

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
