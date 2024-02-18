#include "bigint.h"
#include "bigfloat.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace aak;
using namespace std;

const int threads = 16;

bigfloat pi=0_bf;

bigfloat four=4_bf;
bigfloat two=2_bf;
bigfloat one=1_bf;

vector<bigfloat> v;

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
    cout << "Enter precision" << endl;
    int n;
    cin >> n;
    int len=n;

    if (n==0) n=threads;
    else while (n%threads!=0) ++n;

    auto start = chrono::high_resolution_clock::now();
    bigfloat base = 1_bf;
    v.resize(n);
    for (int k = 0; k < v.size(); k++) {
        v[k] = base;
        base *= 16_bf;
    }

    vector<thread> vv(threads);
    for (int i = 0; i < vv.size(); ++i) {
        vv[i] = thread(foo, i*(n/threads), (i+1)*(n/threads));
    }
    for (auto &i: vv) i.join();

    cout << pi.decimal(len) << endl;
    auto finish = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(finish - start);
    cout << duration.count() << "ms" << endl;

    return 0;
}
