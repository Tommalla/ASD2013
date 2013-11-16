/* Tomasz Zakrzewski /
/  ASD 2013, Zadanie OPE */
#include <cstdio>
#include <algorithm>

using std::max;
using std::min;

const int SIZE = 100010;

struct Node {
	const long long int val;
	const Node* left;
	const Node* right;
	
	Node(const Node* left, const Node* right, const long long int val) : val(val), left(left), right(right) {}
};

const Node* roots[SIZE];

const Node* insert(const Node* v, int i, int j, const int begin, const int end, const int val) {
	//printf("Insert v = %d (%llu), i = %d, j = %d, begin = %d, end = %d, val = %d\n", v, v->val, i, j, begin, end, val);
	if (j < begin || i > end)
		return v;
	
	if (i == begin && j == end)
		return new Node(v->left, v->right, v->val + val);
	
	int l, r, c;
	c = (begin + end) / 2;
	l = max(begin, i);
	r = min(end, j);
	
	return new Node(insert(v->left, l, min(j, c), begin, c, val), insert(v->right, max(i, c + 1), r, c + 1, end, val), v->val);
}

void insert(int u, int v, const int val, const int t) {
	//printf("insert(u = %d, v = %d, val = %d, t = %d)\n", u, v, val, t);
	if (u <= v)
		roots[t] = insert(roots[t - 1], u, v, 0, SIZE - 10, val);
}

void reinsert(int u, int v, const int val, const int t) {
	if (u <= v)
		roots[t] = insert(roots[t], u, v, 0, SIZE - 10, val);
}

long long int query(const Node* v, const int begin, const int end, const int i) {
	//printf("Query: v = %d, begin = %d, end = %d, i = %d\n", v, begin, end, i);
	if (i == begin && i == end)
		return v->val;
	
	int c = (begin + end) / 2;
	if (i <= c)
		return v->val + query(v->left, begin, c, i);
	else
		return v->val + query(v->right, c + 1, end, i);
}

long long int query(const int v, const int t) {
	//printf("query(node = %d, v = %d, t = %d) = ", roots[t], v, t);
	long long int x = query(roots[t], 0, SIZE - 10, v);
	//printf("%d\n", x);
	return x;
}

const Node* create(const int begin, const int end) {
	//printf("Create: begin = %d, end = %d\n", begin, end);
	if (begin == end)
		return new Node(nullptr, nullptr, 0);
	
	int c = (begin + end) / 2;
	return new Node(create(begin, c), create(c + 1, end), 0);
}

long long int third (const long long int x, const long long int n) {
	return (((x * x) % n) * x) % n;
}

long long int thirdMod(const long long int x, const long long int n) {
	return third(x % n, n);
}

int main() {
	int n, T;
	long long int c, x, i, j, i0, t;
	scanf("%d", &n);
	
	roots[0] = create(0, SIZE - 10);
	
	for (T = 0, x = 0; T < n; ++T) {
		i = 1 + third(x + T, n);
		j = i + thirdMod(x + T, n - i + 1);
		i--;
		j--;
		c = thirdMod(x + T, 10);
		i0 = 1 + thirdMod(x + 1 + T, n);
		--i0;
		t = (x + 1234567L) % (T + 1);
		
		//printf("\nT = %d, c = %llu, x = %llu, i = %llu, j = %llu, i0 = %llu, t = %llu\n", T, c, x, i, j, i0, t);
		
		if (c > 0) {
			insert(0, j, c, T + 1);
			reinsert(0, i - 1, -c, T + 1);
		} else
			roots[T + 1] = roots[T];
		
		x = query(i0, t);
	}
	
	for (int i = 0; i < n; ++i)
		printf("%lld\n", query(i, T));
	
	return 0;
}