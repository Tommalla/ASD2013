/* Tomasz Zakrzewski, tz336079 /
/  Pierwsze Zadanie Zaliczeniowe (Bajtobity) */
#include <cstdio>
#include <algorithm>
#include <unordered_set>

using std::max;
using std::min;
using std::fill;
using std::unordered_set;

const int SIZEN = 1010;
const int SIZEM = 10010;

unordered_set <int> dp[2][SIZEM];

int main() {
	int n, m, last, curr, M, lastM, id = 0, next;
	scanf("%d%d", &n, &m);
	last = curr = -1;
	M = lastM = 0;
	
	dp[0][0].insert(0);
	
	for (int i = 0; i < n; ++i, id = (id + 1) % 2) {
		last = curr;
		lastM = M;
		next = (id + 1) % 2;
		scanf("%d", &curr);
		
		for (int j = 0; j <= lastM; ++j)
			dp[next][j].clear();
		
		for (int j = 0; j <= lastM; ++j)	//po osiągniętych sumach
			if (dp[id][j].empty() == false) 
				for (int d: dp[id][j]) 
					if (j + d <= m) {
						int k = d;
						if (dp[next][j + k].empty() || dp[next][j + k].find(k) == dp[next][j + k].end()) {
							dp[next][j + k].insert(k);
							M = max(j + k, M);
						}
					
						//printf("\nStartuję z (%d; %d; %d)\n", j, d, i);
						if (curr > last)
							for (k = last + 1; k <= curr && j + k <= m; ++k) {	//po stanach, do których możemy dojść
								//printf("(%d; %d; %d)\n", j + k, k, i + 1);
								if (dp[next][j + k].empty() || dp[next][j + k].find(k) == dp[next][j + k].end())
									dp[next][j + k].insert(k);

								M = max(j + k, M);
							}
					}
	}
	
	printf("%d\n", M);
	return 0;
}