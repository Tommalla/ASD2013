/* Tomasz [Tommalla] Zakrzewski, ASD 2013/2014 /
/  "Podciągi" */
#include <cstdio>

#define SIZE 1000010

int modTab[SIZE];

int main() {
	modTab[0] = 1;
	int n;
	unsigned long long int res = 1;
	unsigned int sum = 0, m, a;
	scanf("%d%u", &n, &m);

	while (n--) {
		scanf("%u", &a);
		sum = (sum + a) % m;
		res += modTab[sum];
		modTab[sum]++;
	}

	printf("%llu\n", res);

	return 0;
}
