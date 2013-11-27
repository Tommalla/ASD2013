/* Tomasz Zakrzewski /
/  ASD2013, OBC */
#include <cstdio>
#include <set>
#include <vector>

using std::pair;
using std::vector;
using std::make_pair;
using std::max;

const int SIZE = 500050;

struct Query {
	int v, id;
	unsigned long long int k;
	Query(const int v, const int id, const unsigned long long int k) : v(v), id(id), k(k) {}
};

struct Node {
	int parent, anc;
	bool visited, finished;
	unsigned long long int query, mtb;
	vector<pair<int, unsigned long long int>> edges;
	vector<Query> queries;
	
	Node() : anc(-1), visited(false), query(0), mtb(0) {}
};

Node nodes[SIZE];
int ancestors[SIZE];
unsigned long long int weights[SIZE];

const int Find(const int v) {
	if (nodes[v].parent == v)
		return v;
	return nodes[v].parent = Find(nodes[v].parent);
}

void Union(int u, int v) {
	v = Find(v);
	u = Find(u);
	nodes[u].parent = v;
}

void Tarjan(const int v) {
	nodes[v].anc = v;
	nodes[v].parent = v;
	
	for (const auto& e: nodes[v].edges)
		if (nodes[e.first].anc == -1) {
			Tarjan(e.first);
			Union(v, e.first);
			nodes[Find(v)].anc = v;
	}
	
	nodes[v].finished = true;
	
	for (auto& q: nodes[v].queries)
		if (nodes[q.v].finished) {
			ancestors[q.id] = nodes[Find(q.v)].anc;
			nodes[ancestors[q.id]].mtb += weights[q.id];
		}
}

const pair<unsigned long long int, unsigned long long int> dfs(const int v, unsigned long long int sum) {
	unsigned long long int newSum = 0, belowSum = 0;
	nodes[v].visited = true;
	for (auto q: nodes[v].queries)
		if (nodes[q.v].visited == false) {
			newSum += q.k;
			nodes[q.v].query += q.k;
		}
		
	belowSum += nodes[v].query - nodes[v].mtb;
	newSum -= nodes[v].mtb;
	sum -= nodes[v].query;
	sum += newSum;
	
	for (auto& e: nodes[v].edges)
		if (nodes[e.first].visited == false) {
			e.second += sum;	//dodaj do krawędzi
			auto res = dfs(e.first, sum);
			
			belowSum += res.second;
			newSum += res.first;
			
			e.second -= sum - res.second;
			e.second += res.first;
			
			sum -= res.second;
			sum += res.first;
		}
		
	return make_pair(newSum, belowSum);
}

int main() {
	int n, m, u, v;
	unsigned long long int res = 0, k;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n - 1; ++i) {
		scanf("%d%d", &u, &v);
		nodes[u].edges.push_back(make_pair(v, 0));
		nodes[v].edges.push_back(make_pair(u, 0));
	}
	
	for (int i = 0; i < m; ++i) {
		scanf("%d%d%llu", &u, &v, &k);
		nodes[u].queries.push_back(Query(v, i, k));
		nodes[v].queries.push_back(Query(u, i, k));
		weights[i] = k;
	}
	
	Tarjan(0);
	dfs(0, 0);
	
	for (int i = 0; i < n; ++i)
		for (auto e: nodes[i].edges)
			res = max(res, e.second);
		
	printf("%llu\n", res);
	return 0;
}
