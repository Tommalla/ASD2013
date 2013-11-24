/* Tomasz Zakrzewski /
/  ASD2013, PAR */
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>

using std::fill;
using std::vector;
using std::swap;
using std::min;

const int SIZE = 500050;

struct Node {
	int depth, deepestSon, furthestNode, pre, post;
	int sons[2];
	
	vector<int> parent;
	
	Node() : depth(0), deepestSon(-1), furthestNode(-1), sons{-1, -1} {
	}
};

Node nodes[SIZE];
int t;

//returns the number of the desired node
int goUp(int v, int h) {
	if (h < 0 || h > nodes[v].depth)
		exit(666);
	assert(v >= 0 && v < SIZE);
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
	nodes[v].pre = t++;
	
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
	
	nodes[v].post = t++;
}

void dfsDistance(const int v) {
	int w, t;
	
	for (int i = 0; i < 2; ++i) 
		if ((t = nodes[v].sons[i]) != -1 ) {
			w = nodes[v].sons[(i + 1) % 2];
			
			//printf("t = %d, w = %d\n", t, w);
			nodes[t].furthestNode = (w != -1) ? 
			(nodes[nodes[w].deepestSon].depth - nodes[w].depth + 2 >= distance(t, nodes[v].furthestNode, lca(t, nodes[v].furthestNode)) ?
				nodes[w].deepestSon : nodes[v].furthestNode)
				: (nodes[v].furthestNode);
			
			dfsDistance(t);
		}
		
	if (nodes[v].furthestNode == -1)
		nodes[v].furthestNode = v;
}

inline bool isSon(const int father, const int son) {
	return nodes[father].pre <= nodes[son].pre && nodes[father].post >= nodes[son].post;
}

int a, b, da, db, anc, dist, lastcase;

inline int caseOne() {
	lastcase = 1;
	if (da + db == dist)
		return da >= nodes[a].depth - nodes[anc].depth ? goUp(b, db) : goUp(a, da);	//on the path
	return -2;
}

inline int caseTwo() {
	lastcase = 2;
	int id = (db > da) ? a : b;
	int d = (db > da) ? da : db;
	int id2 = (db > da) ? b : a;
	if (abs(da - db) == dist &&  nodes[nodes[id].deepestSon].depth - nodes[id].depth >= d) {	//below one
		if (anc != id || lca(id2, nodes[id].deepestSon) == id) 
			return goUp(nodes[id].deepestSon, nodes[nodes[id].deepestSon].depth - nodes[id].depth - d);
		else {
			int v = -1;
			v = (nodes[id].sons[0] != -1 && !isSon(nodes[id].sons[0], id2)) ? nodes[id].sons[0] : nodes[id].sons[1];
			if (isSon(v, id2))
				return -2;
			if (v != -1 && nodes[nodes[v].deepestSon].depth - nodes[id].depth >= d)
				return goUp(nodes[v].deepestSon, nodes[nodes[v].deepestSon].depth - nodes[id].depth - d);
		}
	}
	return -2;
}

inline int caseThree() {
	lastcase = 3;
	//printf("da = %d, db = %d\n", da, db);
	if (da - nodes[a].depth == db - nodes[b].depth &&
		da >= nodes[a].depth - nodes[anc].depth && db >= nodes[b].depth - nodes[anc].depth) {	//above
			//printf("#3\n");
			int d = da - nodes[a].depth + nodes[anc].depth;
			if (d > nodes[anc].depth)  {
				int v = nodes[anc].furthestNode;
				if (d - nodes[anc].depth > nodes[v].depth)
					return -1;
				else
					return goUp(v, nodes[v].depth - d + nodes[anc].depth);
			} else
				return goUp(anc, d);
		} 
	return -2;
}

inline int caseFour() {
	lastcase = 4;
	//printf("The worst case!\n");
	int x = (da - db + dist) / 2;
	int start = -1, mid;
	if (x > nodes[a].depth - nodes[anc].depth) {	//b branch
		x = (db - da + dist) / 2;
		if (x > 0 && x <= nodes[b].depth - nodes[anc].depth) {	//otherwise -1
			swap(a, b);
			swap(da, db);
			start = a;
		} else
			return -2;
	} else if (x > 0) {
		start = a;
	}
	
	//printf("a = %d, b = %d, da = %d, db = %d, start = %d\n", a + 1, b + 1, da, db, start + 1);
	
	if (start != -1 && x > 0 && da - x == db - dist + x && da - x > 0) {
		mid = goUp(start, x);
		
		int v = -1;
		for (int k = 0; k < 2; ++k)
			if (nodes[mid].sons[k] != -1) {
				if (!isSon(nodes[mid].sons[k], start)) {
					v = nodes[mid].sons[k];
					break;
				}
			}
			
		//printf("start = %d, da = %d, mid = %d, v = %d, x = %d\n", start + 1, da, mid + 1, v + 1, x);
			
		if (v != -1) {
			v = nodes[v].deepestSon;
			int vd = nodes[v].depth - nodes[mid].depth;
			vd = vd - (da - x);
			//printf("going up from %d by %d\n", v, vd);
			if (vd >= 0 && vd < nodes[v].depth - nodes[mid].depth)
				return goUp(v, vd);
		}
	}
	return -2;
}

int main() {
	int n, m;
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
	int cnt = 0;
	while (m--) {
		++cnt;
		scanf("%d%d%d%d", &a, &b, &da, &db);
		--a;
		--b;
		
		if (nodes[a].depth > nodes[b].depth) {
			swap(a, b);
			swap(da, db);
		}
		
		anc = lca(a, b);
		dist = distance(a, b, anc);
		int res = -1;
		//printf("distance(%d, %d) = %d\n", a + 1, b + 1, dist);
		
		if (da + db < dist) {
			puts("-1");
			continue;
		}
		
		if ((res = caseOne()) == -2 && (res = caseTwo()) == -2 && (res = caseThree()) == -2 && (res = caseFour()) == -2)
			puts("-1");
		else {
			//if (cnt == 24362)
				//return lastcase;
			printf("%d\n", res > -1 ? res + 1 : res);
		}
	}
	return 0;
}