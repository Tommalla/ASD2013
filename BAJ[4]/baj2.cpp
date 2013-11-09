/* Tomasz Zakrzewski /
/  Druga wersja BAJ */
#include <cstdio>
#include <queue>
#include <algorithm>
#include <set>

using std::queue;
using std::pair;
using std::make_pair;

const int SIZEN = 1010;
const int SIZEM = 10010;

int a[SIZEN];
int t[SIZEM];

queue <pair <int, int>> q[2];	//pary <k, b> oznaczają stan: osiągnąłem sumę k dając ostatniemu b
bool vis[2][SIZEM][SIZEN];

int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	
	for (int i = 0; i < n; ++i)
		scanf("%d", &a[i]);

	std::reverse(a, a + n);
	
	//transpozycja:
	for (int i = 0; i < n; ++i) {
		int j = i + 1;
		for (; j < n && a[i] == a[j]; ++j);
		for (int k = a[0] - a[i]; k < ((j >= 0) ? a[0] - a[j] : a[0]); ++k)
			t[k] = j;
	}
	
	int id = 0;
	q[0].push(make_pair(0, 0));
	
	for (int i = 0; i < a[0]; ++i, id = (id + 1) % 2) {
		int next = (id + 1) % 2;
		
		while (!q[id].empty()) {
			int k = q[id].front().first;
			int b = q[id].front().second;
			
			//porządki:
			q[id].pop();
			vis[id][k][b] = false;
			
			int s[2][2] = { {k + b, b}, {k + t[i], t[i]}};
			
			for (int j = 0; j < 2; ++j)
				if (s[j][0] <= m && vis[next][s[j][0]][s[j][1]] == false) {
					vis[next][s[j][0]][s[j][1]] = true;
					q[next].push(make_pair(s[j][0], s[j][1]));
				}
		}
	}
	
	int res = 0;
	while (!q[id].empty()) {
		res = std::max(res, q[id].front().first);
		q[id].pop();
	}
	printf("%d\n", res);
	return 0;
}