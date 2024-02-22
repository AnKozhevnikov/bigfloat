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

bigint onebi=1_bi;
bigint eightbi=8_bi;
bigint fourbi=4_bi;
bigint fivebi=5_bi;
bigint sixbi=6_bi;
bigint sixteen=16_bi;

int p;

bigint binpow(bigint x, int y) {
    if (y==0) return onebi;
    if (y==1) return x;
    bigint res=binpow(x,y/2);
    if (y%2==0) return res*res;
    return res*res*x;
}

void foo(int l, int r) {
    static std::mutex m;
    bigint b=binpow(sixteen,l);
    bigfloat add=0_bf;
    bigint mult=eightbi*l;
    for (int i=l; i<r; ++i) {
        add += ((four / bigfloat(mult + onebi, p)) - (two / bigfloat(mult + fourbi, p)) - (one / bigfloat(mult + fivebi,p)) - (one / bigfloat(mult + sixbi,p))) / bigfloat(b,p);
        mult+=eightbi;
        b<<=fourbi;
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