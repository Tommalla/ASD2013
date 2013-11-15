/* Tomasz Zakrzewski /
/  ASD 2013, Zadanie OPE */
#include <cstdio>
#include <algorithm>

using std::max;
using std::min;

const int SIZE = 100010;

struct Node {
	unsigned long long int val;
	const Node* left;
	const Node* right;
	
	Node(const Node* left, const Node* right, const unsigned long long int val) : left(left), right(right)
		, val(val) {}
};

const Node* roots[SIZE];

const Node* insert(const Node* v, int i, int j, const int begin, const int end, const int val) {
	if (j < begin || i > end)
		return v;
	
	if (j == begin && i == end)
		return new Node(nullptr, nullptr, v->val + val);
	
	int l, r, c;
	c = (begin + end) / 2;
	l = max(begin, i);
	r = min(end, j);
	
	return new Node(insert(v->left, l, i, begin, c, val), insert(v->right, j, r, c + 1, end, val), v->val + val);
}

void insert(int u, int v, const int val, const int t) {
	roots[t] = insert(roots[t - 1], u, v, 0, SIZE - 10, val);
}

unsigned long long int query(const Node* v, const int begin, const int end, const int i) {
	if (i == begin && i == end)
		return v->val;
	
	int c = (begin + end) / 2;
	if (i <= c)
		return v->val + query(v->left, begin, c, i);
	else
		return v->val + query(v->right, c + 1, end, i);
}

unsigned long long int query(const int v, const int t) {
	return query(roots[t], 0, SIZE - 10, v);
}

const Node* create(const int begin, const int end) {
	if (begin == end)
		return new Node(nullptr, nullptr, 0);
	
	int c = (begin + end) / 2;
	return new Node(create(begin, c), create(c + 1, end), 0);
}

unsigned long long int third (unsigned long long int x, const unsigned long long int n) {
	return (((x * x) % n) * x) % n;
}

unsigned long long int thirdMod(unsigned long long int x, const unsigned long long int n) {
	return third(x % n, n);
}

int main() {
	int n, T;
	unsigned long long int c, x, i, j, i0, t;
	scanf("%d", &n);
	
	roots[0] = create(0, SIZE - 10);
	
	for (T = 0, x = 0; T <= n; ++T) {
		i = 1 + third(x + T, n);
		j = i + thirdMod(x + T, n - i + 1);
		c = thirdMod(x + T, 10);
		i0 = 1 + thirdMod(x + 1 + T, n);
		t = (x + 1234567L) % (T + 1),
		
		insert(i, j, c, T + 1);
		x = query(i0, t);
	}
	
	for (int i = 0; i < n; ++i)
		printf("%llu\n", query(i, T));
}