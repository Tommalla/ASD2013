/* Tomasz Zakrzewski /
/  ASD2013, PRO */
#include <cstdio>
#include <set>
#include <vector>
#include <algorithm>

using std::set;
using std::pair;
using std::make_pair;
using std::max;
using std::vector;

const int SIZE = 100010;

struct Node {
	int w, inDeg;
	vector<int> neighbours;
};

Node nodes[SIZE];
set<pair<int, int>> s;

int main() {
	int n, m, k, u, v, res = 0;
	scanf("%d%d%d", &n, &m, &k);
	
	for (int i = 0; i < n; ++i)
		scanf("%d", &nodes[i].w);
	
	while(m--) {
		scanf("%d%d", &v, &u);
		--u;
		--v;
		nodes[v].inDeg++;
		nodes[u].neighbours.push_back(v);
	}
	
	for (int i = 0; i < n; ++i)
		if (nodes[i].inDeg == 0)
			s.insert(make_pair(nodes[i].w, i));
		
	while (k-- && !s.empty()) {
		u = s.begin()->second;
		s.erase(s.begin());
		
		res = max(res, nodes[u].w);
		for (int t: nodes[u].neighbours)
			if (--nodes[t].inDeg == 0)
				s.insert(make_pair(nodes[t].w, t));
	}
	
	printf("%d\n", res);
}