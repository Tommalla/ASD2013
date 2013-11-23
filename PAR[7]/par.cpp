/* Tomasz Zakrzewski /
/  ASD2013, PAR */
#include <cstdio>
#include <algorithm>
#include <vector>

using std::fill;
using std::vector;
using std::swap;
using std::min;

const int SIZE = 500050;

struct Node {
	int depth, deepestSon, furthestNode;
	int sons[2];
	
	vector<int> parent;
	
	Node() : depth(0), deepestSon(-1), furthestNode(-1), sons{-1, -1} {
	}
};

Node nodes[SIZE];

//returns the number of the desired node
int goUp(int v, int h) {
	if (h < 0)
		exit(666);
	//printf("goUp(%d, %d) = ", v + 1, h);
	int m = 1;
	for (int i = 0; m <= h; ++i, m <<= 1) 
		if (h & m) {
			v = nodes[v].parent[i];
			h ^= m;
		}
		
	//printf("%d\n", v + 1);
	return v;
}

int lca(int u, int v) {
	//printf("LCA(%d, %d)\n", u + 1, v + 1);
	//get to the same level
	if (nodes[u].depth < nodes[v].depth)
		swap(u, v);	//u is deeper
	u = goUp(u, nodes[u].depth - nodes[v].depth);
	if (u == v)
		return u;
	
	int prevU, prevV;
	prevU = u;
	prevV = v;
	int i = 0;
	
	while (u != v) {
		prevU = u;
		prevV = v;
		while (nodes[u].parent.size() <= i || nodes[v].parent.size() <= i) {
			//printf("RE!, u = %d, v = %d, i = %d\n", u + 1, v + 1, i);
			i--;
		}
		u = nodes[u].parent[i];
		v = nodes[v].parent[i];
		++i;
	}
	--i;
	
	u = prevU;
	v = prevV;
	
	while (nodes[u].parent.size() > 0 && nodes[v].parent.size() > 0 && nodes[u].parent[0] != nodes[v].parent[0] && --i >= 0) {
		int j;
		j = min(min(i, (int)nodes[u].parent.size() - 1), (int)nodes[v].parent.size() - 1);
		if (j >= 0 && nodes[u].parent[j] != nodes[v].parent[j]) {
			u = nodes[u].parent[j];
			v = nodes[v].parent[j];
		}
	}
	
	return nodes[u].parent.size() > 0 ? nodes[u].parent[0] : u;
}

//through should be LCA(u, v)
inline int distance(const int u, const int v, const int through) {
	return nodes[u].depth + nodes[v].depth - 2 * nodes[through].depth;
}

void dfsDepth(const int v) {
	//printf("dfsDepth(%d)\n", v);
	int w;
	
	for (int i = 0; i < 2; ++i)
		if ((w = nodes[v].sons[i]) != -1) {
			nodes[w].depth = nodes[v].depth + 1;
			nodes[w].parent.push_back(v);
			for (int j = 0; j < nodes[v].parent.size() && j < nodes[nodes[w].parent[j]].parent.size(); ++j)
				nodes[w].parent.push_back(nodes[nodes[w].parent[j]].parent[j]);
				
			dfsDepth(w);
			
			if (nodes[v].deepestSon == -1 || nodes[nodes[v].deepestSon].depth < nodes[nodes[w].deepestSon].depth)
				nodes[v].deepestSon = nodes[w].deepestSon;
		}
		
	if (nodes[v].deepestSon == -1)
		nodes[v].deepestSon = v;
}

void dfsDistance(const int v) {
	int w, t;
	
	for (int i = 0; i < 2; ++i) 
		if ((t = nodes[v].sons[i]) != -1 ) {
			w = nodes[v].sons[(i + 1) % 2];
			
			//printf("t = %d, w = %d\n", t, w);
			nodes[t].furthestNode = (w != -1) ? 
			(nodes[nodes[w].deepestSon].depth + 2 >= distance(t, nodes[v].furthestNode, lca(t, nodes[v].furthestNode)) ?
				nodes[w].deepestSon : nodes[v].furthestNode)
				: (nodes[v].furthestNode);
			
			dfsDistance(t);
		}
		
	if (nodes[v].furthestNode == -1)
		nodes[v].furthestNode = v;
}

int main() {
	int n, m, a, b, da, db;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &nodes[i].sons[0], &nodes[i].sons[1]);
		for (int j = 0; j < 2; ++j)
			if (nodes[i].sons[j] != -1)
				nodes[i].sons[j]--;
	}
	
	//preprocessing
	nodes[0].furthestNode = 0;
	dfsDepth(0);
	dfsDistance(0);
	
	/*for (int i = 0; i < n; ++i) {
		printf("Node %d:\ndepth = %d, dS = %d, fN = %d\n", i + 1, nodes[i].depth, nodes[i].deepestSon + 1, nodes[i].furthestNode + 1);
		printf("Parents table:");
		for (int p: nodes[i].parent)
			printf(" %d", p + 1);
		puts("\n");
	}*/
	
	scanf("%d", &m);
	while (m--) {
		scanf("%d%d%d%d", &a, &b, &da, &db);
		--a;
		--b;
		
		int anc = lca(a, b);
		int dist = distance(a, b, anc);
		int res = -1;
		//printf("distance(%d, %d) = %d\n", a + 1, b + 1, dist);
		
		if (da + db == dist)
			res = da >= nodes[a].depth - nodes[anc].depth ? goUp(b, db) : goUp(a, da);	//on the path
		else if (abs(da - db) == dist) {	//below one
			int id = (db > da) ? a : b;
			int d = (db > da) ? da : db;
			//printf("id = %d, d = %d\n", id + 1, d);
			if (nodes[nodes[id].deepestSon].depth - nodes[id].depth < d) 
				res = -1;
			else
				res = goUp(nodes[id].deepestSon, nodes[nodes[id].deepestSon].depth - nodes[id].depth - d);
		} else if (da - nodes[a].depth + nodes[anc].depth == db - nodes[b].depth + nodes[anc].depth &&
			da >= nodes[a].depth - nodes[anc].depth && db >= nodes[b].depth - nodes[anc].depth) {	//above
			int d = da - nodes[a].depth + nodes[anc].depth;
			if (d > nodes[anc].depth)  {
				int v = nodes[anc].furthestNode;
				if (d - nodes[anc].depth > nodes[v].depth)
					res = -1;
				else
					res = goUp(v, nodes[v].depth - d + nodes[anc].depth);
			} else
				res = goUp(anc, d);
		} else {
			
			//printf("The worst case!\n");
			int x = (da - db + dist) / 2;
			int start = -1, mid;
			if (x > nodes[a].depth - nodes[anc].depth) {	//b branch
				x = (db - da + dist) / 2;
				if (x <= nodes[b].depth - nodes[anc].depth) {	//otherwise -1
					swap(a, b);
					swap(da, db);
					start = a;
				}
			} else {
				start = a;
			}
			
			//printf("a = %d, b = %d, da = %d, db = %d, start = %d\n", a + 1, b + 1, da, db, start);
			
			if (start != -1 && x >= 0 && da - x == db - dist + x ) {
				mid = goUp(start, x);
				
				
				int v = -1;
				for (int k = 0; k < 2; ++k)
					if (nodes[mid].sons[k] != -1) {
						if (lca(nodes[mid].sons[k], start) == mid) {
							v = nodes[mid].sons[k];
							break;
						}
					}
				
				//printf("start = %d, da = %d, mid = %d, v = %d, x = %d\n", start + 1, da, mid + 1, v + 1, x);
					
				if (v != -1) {
					v = nodes[v].deepestSon;
					int vd = nodes[v].depth - nodes[mid].depth;
					if (vd >= da - x) {
						vd = vd - (da - x);
						res = goUp(v, vd);
					}
				}
			}
			
		}
		
		//printf("LCA(%d, %d) = %d\n", a + 1, b + 1, lca(a, b) + 1);
		printf("%d\n", res > -1 ? res + 1 : res);
	}
	return 0;
}