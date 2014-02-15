/* Tomasz Zakrzewski, tz336079 /
/  Egzamin z ASD2013 	      */
#include <cstdio>
#include <vector>

using std::vector;

const int NODES = 500050;

struct Node {
	vector<int> neighbours;
	int colour;

	Node() : colour(0) {}
};

Node nodes[NODES];

bool res = true;

void dfs(const int v) {
	for (const auto u: nodes[v].neighbours) {
		if (nodes[u].colour == 0) {	//nieodwiedzony
			nodes[u].colour = nodes[v].colour == 1 ? 2 : 1;
			dfs(u);
		} else if (nodes[u].colour == nodes[v].colour) {	//błąd!
			res = false;
			return;
		}
	}
}

int main() {
	int n, m, u, v;
	scanf("%d%d", &n, &m);

	while (m--) {
		scanf("%d%d", &u, &v);
		--u;
		--v;

		nodes[u].neighbours.push_back(v);
		nodes[v].neighbours.push_back(u);
	}

	for (v = 0; v < n && res; ++v)
		if (nodes[v].colour == 0) {
			nodes[v].colour = 1;
			dfs(v);
		}

	puts(res ? "TAK" : "NIE");
	return 0;
}
