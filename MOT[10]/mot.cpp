/* Tomasz Zakrzewski /
/  ASD2013, MOT */
#include <cstdio>
#include <vector>
#include <set>

using std::set;
using std::vector;
using std::pair;
using std::make_pair;
using std::max;
using std::min;
using std::swap;

const long long int INF = 9999999999LL;
const int SIZE = 50050;

struct Node {
	vector<pair<int, long long int>> edges;
	vector<pair<int, long long int>> revEdges;
	long long int d, revD;
	
	Node() : d(INF), revD(INF) {}
};

Node nodes[SIZE];

struct cmp {
	inline bool operator() (const int &a, const int &b) {
		if (nodes[a].d != nodes[b].d)
			return nodes[a].d < nodes[b].d;
		return a < b;
	}
};

set<int, cmp> s;

void dijikstra(int v) {
	nodes[v].d = 0;
	s.insert(v);
	
	while (!s.empty()) {
		v = *s.begin();
		s.erase(s.begin());
		
		for (auto e: nodes[v].edges) 
			if (nodes[e.first].d > nodes[v].d + e.second) {
				auto iter = s.find(e.first);
				if (iter != s.end())
					s.erase(iter);
				nodes[e.first].d = nodes[v].d + e.second;
				s.insert(e.first);
			}
	}
}

int main() {
	int n, m, u, v;
	long long int c;
	scanf("%d%d", &n, &m);
	
	while (m--) {
		scanf("%d%d%lld", &u, &v, &c);
		nodes[u].edges.push_back(make_pair(v, c));
		nodes[v].revEdges.push_back(make_pair(u, c));
	}
	
	dijikstra(0);
	
	for (v = 0; v < n; ++v) {
		swap(nodes[v].d, nodes[v].revD);
		swap(nodes[v].edges, nodes[v].revEdges);
	}
	
	dijikstra(n - 1);
	
	c = INF;
	for (v = 0; v < n; ++v)
		c = min(c, max(nodes[v].d, nodes[v].revD));
	
	printf("%lld\n", c);
	
	return 0;
}