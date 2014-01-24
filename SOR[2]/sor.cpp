#include <cstdio>
#include <algorithm>

#define SIZE 1010
#define MOD 1000000000

using namespace std;

unsigned int dp[SIZE][SIZE][2];	//id, długość, lewy/prawy
int t[SIZE];

int main() {
	int n;
	scanf("%d", &n);
	
	if (n == 1) {
		puts("1");
		return 0;
	}
	
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t[i]);
		dp[i][0][0] = dp[i][0][1] = 1;
	}
	
	for (int j = 1; j < n; ++j)
		for (int i = 0; i < n - j; ++i) {
			dp[i][j][0] = ((t[i] < t[i + 1]) ? dp[i + 1][j - 1][0] : 0) + 
				((i + 1 != i + j && t[i] < t[i + j]) ? dp[i + 1][j - 1][1] : 0);
				
			dp[i][j][1] = ((t[i + j] > t[i]) ? dp[i][j - 1][0] : 0) + 
				((i != i + j - 1 && t[i + j] > t[i + j - 1]) ? dp[i][j - 1][1] : 0);
				
			dp[i][j][0] %= MOD;
			dp[i][j][1] %= MOD;
		}
		
		
		
	printf("%u\n", (dp[0][n-1][0] + dp[0][n-1][1]) % MOD);
	return 0;
}