#include <iostream>
#include <vector>
#include <algorithm>

// ./graphdiv <test/in.txt | dot -Tpng -o test/diagram.png

using namespace std;

vector<long> list;

bool compare(long i,long j) {
    return  i > j;
}

void func_div(long a, long b, long c) {
    list.push_back(b);
    if ( b != c ) {
        for (b++; (b << 1) <= a; b++) {
            if (!(a % b)) {
                break;
            }
        }
        if (b * b <= a) { func_div(a, b, a / b); }
        list.push_back(c);
    }
}

bool compThreeElem(long a, long b, long c) {
    return ((a % c == 0)  && (c % b == 0) && (c != b) && (c != a)) ;
}

bool compareVector(long a, long b) {

    if((list[a] % list[b])) {
        return false;
    }

    for (int i = a; i < b; i++) {
        if(compThreeElem(list[a],list[b],list[i])) {
            return false;
        }
    }
    return true;
}


int main(long n) {

    cin >> n;

    func_div(n, 1, n);

    sort(list.begin(), list.end(), compare);

    cout << "graph\n{" << endl;

    int count = 0;
    for (vector<long>::iterator it=list.begin(); it!= list.end(); ++it, count++) {
        cout << "\t" << *it << endl;
    }

    for(int i = 0; i < list.size(); i++) {
        for(int k = i+1; k < list.size(); k++) {
            if (compareVector(i, k)) {
                cout << "\t" << list[i] << " -- " << list[k] << endl;
            }
        }
    }

    cout << "}" << endl;

    return 0;
}
