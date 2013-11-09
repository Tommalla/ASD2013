#include <cstdio>
#include <algorithm>

using namespace std;

const int BASE = 32768;
const int TREESIZE = 2 * BASE;
const int SIZE = 20020;
const int MOD = 1000000000;

int tree[TREESIZE];
int a[SIZE];
int tmp[SIZE];

void insert(int v, const int val) {
	v += BASE;
	
	while (v) {
		tree[v] = (tree[v] + val) % MOD;
		v /= 2;
	}
}

int query(int u, int v) {
	u += BASE;
	v += BASE;
	
	int res = (tree[u] + ((u != v) ? tree[v] : 0)) % MOD;
	
	while (u / 2 != v / 2) {
		if (u % 2 == 0) {
			res += tree[u + 1];
			res %= MOD;
		}
		
		if (v % 2 == 1) {
			res += tree[v - 1];
			res %= MOD;
		}
		
		u /= 2;
		v /= 2;
	}
	return res;
}

void clear(int v) {
	v += BASE;
	
	while (v && tree[v]) {
		tree[v] = 0;
		v /= 2;
	}
}

int main() {
	int n, k, maks = 0;
	scanf("%d%d", &n, &k);
	
	for (int i = 0; i < n; ++i) {
		scanf("%d", &a[i]);
		maks = max(maks, a[i]);
		tmp[i] = query(a[i], maks);
		insert(a[i], 1);
	}
	
	for (int j = 2; j < k; ++j) {
		for (int i = 1; i <= maks; ++i)
			clear(i);
		for (int i = 0; i < n; ++i) {
			int t = tmp[i];
			tmp[i] = query(a[i], maks);
			insert(a[i], t);
		}
	}
	
	int res = 0;
	for (int i = 0; i < n; ++i) {
		res += tmp[i];
		res %= MOD;
	}
	printf("%d\n", res);
	
	return 0;
}