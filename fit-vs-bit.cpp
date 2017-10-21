#include<bits/stdc++.h>
#define N 1000010
#define int long long
using namespace std;

int fit[N], bit[N]; //fibonacci and binary indexed trees
vector <int> fibs; //fibonacci numbers
vector <int> pows; //powers of 2
int LSF[N], LSB[N]; //Least Significant Fibonacci bit, Least Significant Binary bit
int n, u;
int dp[N], dpfib[N], dpbin[N], child[N];
bool reps[N][60];  //stores the entire Fibonacci coding of numbers 1 to N. Used to construct the LSF array

void fit_update(int idx, int val) {
    while (idx < N) {
        fit[idx] += val;
        idx += fibs[LSF[idx]-1];
    }
}

void bit_update(int idx, int val) {
    while (idx < N) {
        bit[idx] += val;
        idx += pows[LSB[idx]];
        //equivalent to idx += (idx&(-idx)), but it is implemented in this manner so as to equalize the number of cache hits and get a better estimate of the relative theoretical speeds of the two query functions.
    }
}

int fit_query(int idx) {
    int ret = 0;
    while (idx > 0) {
        ret += fit[idx];
        idx -= fibs[LSF[idx]];
    }
    return ret;
}

int bit_query(int idx) {
    int ret = 0;
    while (idx > 0) {
        ret += bit[idx];
        idx -= pows[LSB[idx]];
        //equivalent to idx -= (idx&(-idx)), but it is implemented in this manner so as to equalize the number of cache hits and get a better estimate of the relative theoretical speeds of the two query functions.
    }
    return ret;
}

// A hybrid approach to the query function. Combines the fibonacci and the binary indexed trees to by taking the shortest path from idx to 0. (shortest path is calculated by using dynamic programming)
int super_query(int idx) {
    int ret = 0;
    while (idx > 0) {
        if (child[idx]==1) {
            ret += bit[idx];
            idx -= pows[LSB[idx]];
        }
        else {
            ret += fit[idx];
            idx -= fibs[LSF[idx]];
        }
    }
    return ret;
}

main() {

    //Precomputations for the Fibonacci Indexed Tree.
    int a = 1, b = 0;
    for (int i=1; i<60; i++) {
        fibs.push_back(a);
        int tmp = b;
        b = a;
        a += tmp;
    }

    //Computing the Least Significant Fibonacci bit of numbers 1 to N.
    int cnt = 1;
    for (int i=1; i<N; i++) {
        if (fibs[cnt+1] <= i) cnt++;
        reps[i][cnt] = 1;
        for (int j=1; j<cnt; j++) reps[i][j] = reps[i-fibs[cnt]][j];
        for (int j=1; j<=cnt; j++) {
            if (reps[i][j]) {
                LSF[i] = j;
                break;
            }
        }
    }

    //Precomputations for the Binary Indexed Tree.
    a = 1;
    pows.push_back(0);
    for (int i=1; i<60; i++) {
        pows.push_back(a);
        a *= 2;
    }

    //Computing the Least Significant Binary bit of numbers 1 to N.
    for (int i=1; i<N; i++) {
        int tmp = i, c = 1;
        while (1) {
            if (tmp%2==1) {
                LSB[i] = c;
                break;
            }
            c++;
            tmp/=2;
        }
    }

    //calculates the shortest path from a number n to 0.
    //dpfib[n] stores number of steps in the path from n to 0 by using the fibonacci indexed tree (that is, it stores the number of 'on' bits in the Fibonacci Coding)
    //dpbin[n] stores number of steps in the path from n to 0 by using the binary indexed tree (that is, it stores the number of 'on' bits in the Binary Representation)
    //dp[n] stores the minimum number of steps required to go from n to 0. A step can be turning 'off' the Least significant Fibonacci Bit or Binary Bit.

    int dpsum = 0, sumBin = 0, sumFib = 0;
    for (int i=1; i<=1000000; i++) {
        dpfib[i] = dpfib[i-fibs[LSF[i]]] + 1;
        dpbin[i] = dpbin[i-pows[LSB[i]]] + 1;
        sumFib += dpfib[i];
        sumBin += dpbin[i];

        if (dp[i-fibs[LSF[i]]] > dp[i-pows[LSB[i]]]) {
            dp[i] = dp[i-pows[LSB[i]]] + 1;
            child[i] = 1;  // The first step in the shortest path from i to 0 is to turn 'off' the LSB while doing a super_query on i.
        }
        else {
            dp[i] = dp[i-fibs[LSF[i]]] + 1;
            child[i] = 2;  // The first step in the shortest path from i to 0 is to turn 'off' the LSF while doing a super_query on i.
        }

        dpsum += dp[i];
    }
    cout<<"Total number of 'on' Binary bits in numbers 1 to 1000000:    "<<sumBin<<endl;
    cout<<"Total number of 'on' Fibonacci bits in numbers 1 to 1000000: "<<sumFib<<endl;
    cout<<"Sum of shortest paths for numbers 1 to 1000000               "<<dpsum<<endl;


    vector <pair<int,int> > u;
    vector <int> q;
    for (int i=1; i<=10000000; i++) {
        srand(time(NULL));
        int x = rand()%1000000 + 1;
        int val = rand()%1000;
        u.push_back(make_pair(x,val));
    }
    for (int i=1; i<=10000000; i++) {
        srand(time(NULL));
        int x = rand()%1000000 + 1;
        q.push_back(x);
    }

    int time1 = clock();
    for (int i=0; i<10000000; i++) fit_update(u[i].first, u[i].second);
    int fituptime = clock() - time1;

    time1 = clock();
    for (int i=0; i<10000000; i++) bit_update(u[i].first, u[i].second);
    int bituptime = clock() - time1;

    int superuptime = fituptime + bituptime; // big disadvantage of the Hybrid: Need for an update on both the trees

    time1 = clock();
    for (int i=0; i<10000000; i++) fit_query(q[i]);
    int fitqtime = clock() - time1;

    time1 = clock();
    for (int i=0; i<10000000; i++) bit_query(q[i]);
    int bitqtime = clock() - time1;

    time1 = clock();
    for (int i=0; i<10000000; i++) super_query(q[i]);
    int superqtime = clock() - time1;

    cout<<endl;
    cout<<"        "<<"UPDATE  QUERY"<<endl;
    cout<<"FIT:    "<<fituptime<<"    "<<fitqtime<<endl;
    cout<<"BIT:    "<<bituptime<<"    "<<bitqtime<<endl;
    cout<<"HYBRID: "<<superuptime<<"    "<<superqtime<<endl;
}
