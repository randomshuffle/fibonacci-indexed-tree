// The below code was verified by testing against the following problem:
// http://www.spoj.com/problems/UPDATEIT/

#include<bits/stdc++.h>
#define N 100010
using namespace std;

bool reps[N][26]; //stores the entire Fibonacci coding of numbers 1 to N. Used to construct the LSF array
int fit[N]; //fibonacci indexed tree
vector <int> fibs; //fibonacci numbers
int LSF[N]; //Least Significant Fibonacci bit
int n, u;

void update(int idx, int val) {
    while (idx <= n) {
        fit[idx] += val;
        idx += fibs[LSF[idx]-1];
    }
}

int query(int idx) {
    int ret = 0;
    while (idx > 0) {
        ret += fit[idx];
        idx -= fibs[LSF[idx]];
    }
    return ret;
}

main() {

    //Precomputations for the Fibonacci Indexed Tree.
    int a = 1, b = 0;
    for (int i=1; i<26; i++) {
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

    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %d", &n, &u);
        for (int i=0; i<=n; i++) fit[i] = 0;

        while (u--) {
            int l, r, val;
            scanf("%d %d %d", &l, &r, &val);
            update(l+1, val);
            update(r+2, -val);
        }

        int q;
        scanf("%d", &q);
        while (q--) {
            int x;
            scanf("%d", &x);
            printf("%d\n", query(x+1));
        }
    }
}
