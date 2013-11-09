#include <cstdio>
#include <algorithm>

#define INF 99999999
#define SIZE 500050

using namespace std;

int t[SIZE];

int main() {
	int n, x;
	scanf("%d", &n);

	for (int i = 0; i <= n; ++i)
		t[i] = INF;
	
	for (int i = 0; i < n; ++i) {
		scanf("%d", &x);
		int* a = lower_bound(t, t + n, x);
		*a = x;
	}
	
	int res = 0;
	for (int i = n - 1; i >= 0; --i)
		if (t[i] != INF) {
			res = i + 1;
			break;
		}
	
	printf("%d\n", n - res);
	
	return 0;
}