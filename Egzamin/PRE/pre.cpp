/* Tomasz Zakrzewski, tz336079 /
/  Egzamin z ASD2013 	      */
#include <cstdio>
#include <set>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;
using std::min;
using std::make_pair;
using std::sort;

const int NODES = 500050;
const unsigned int INF = 1999999999;

struct Node {
	vector<pair<int, unsigned int>> neighbours;
	unsigned int cost;
};

Node nodes[NODES];
int id[NODES], qty[NODES];

inline bool cmp(const int& a, const int& b) {
	if (nodes[a].cost != nodes[b].cost)
		return nodes[a].cost < nodes[b].cost;
	return a < b;
}

inline bool edgeCmp(const pair<int, unsigned int>& a, const pair<int, unsigned int>& b) {
	return cmp(a.first, b.first);
}

int main() {
	int n, m, u, v;
	unsigned int c;
	unsigned int res = INF;
	scanf("%d", &n);

	for (v = 0; v < n; ++v) {
		id[v] = v;
		scanf("%u", &nodes[v].cost);
	}

	scanf("%d", &m);

	while (m--) {
		scanf("%d%d%u", &u, &v, &c);
		--u;
		--v;
		c *= 2;
		nodes[u].neighbours.push_back(make_pair(v, c));
		nodes[v].neighbours.push_back(make_pair(u, c));

		c = nodes[u].cost + nodes[v].cost + c;
		res = min(res, c);
	}

	sort(id, id + n, cmp);
	c = nodes[id[n-1]].cost;

	for (int i = 0; i < n && nodes[id[i]].cost < res; ++i) {
		v = id[i];

		if (qty[v] < i) {	//da się
			sort(nodes[v].neighbours.begin(), nodes[v].neighbours.end(), edgeCmp);

			int j,  resV = -1;
			j = 0;
			//szukamy pierwszego wierzchołka odwiedzonego, takiego, że nie ma go na liście sąsiedztwa
			for (; j < min(i,  (int)nodes[v].neighbours.size()); ++j) {
				u = nodes[v].neighbours[j].first;
				if (nodes[id[j]].cost != nodes[u].cost || id[j] != u) {
					resV = id[j];
					break;
				}
			}

			if (resV == -1)
				resV = id[j];

			res = min(res, nodes[v].cost + nodes[resV].cost);
		}

		for (const auto t: nodes[v].neighbours)
			qty[t.first]++;
	}

	printf("%u\n", res);
	return 0;
}
