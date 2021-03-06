#include <bits/stdc++.h>
using namespace std;
const int N = (int)(1e5 + 5);
const long long INF = (long long)1e18;
long long S[N], val[N];
long long spare[2][2];
void PreCompute()
{
	for(int i = 1; i < N; i++)
		S[i] = i;
	
	for(int i = 2; i * i < N; i++)
		if(S[i] == i)
			for(int j = i; i * j < N; j++)
				S[i * j] = i;
}
long long Power(long long base, long long index, long long mod)
{
	long long res = 1, temp = base;
	while(index)
	{
		if(index & 1)
			res = (res * temp) % mod;
		temp = (temp * temp) % mod;
		index >>= 1;
	}
	return res;
}
void MatrixMultiply(long long a[][2], long long b[][2], long long mod)
{
	memset(spare, 0, sizeof(spare));
	
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
		{
			spare[i][j] = 0;
			for(int k = 0; k < 2; k++)
				spare[i][j] = (spare[i][j] + a[i][k] * b[k][j]) % mod;
		}
		
	memcpy(a, spare, sizeof(spare));
}
pair <long long, long long> FindFibTerms(long long index, long long mod)
{
	if(index == 0)
		return make_pair(0, 1);
	if(index == 1)
		return make_pair(1, 1);
	index--;
	long long res[2][2], temp[2][2];
	res[0][0] = res[1][1] = temp[0][0] = temp[0][1] = temp[1][0] = 1;
	res[0][1] = res[1][0] = temp[1][1] = 0;
	while(index)
	{
		if(index & 1)
			MatrixMultiply(res, temp, mod);
		MatrixMultiply(temp, temp, mod);
		index >>= 1;
	}
	return make_pair(res[0][0], res[1][0]);
}
long long FindPeriod(long long p)
{
	if(p == 2)
		return 3;
	else if(p == 3)
		return 8;
	else if(p == 5)
		return 20;
	
	int rem = p % 5;
	long long multiple;
	if(rem == 1 || rem == 4)
		multiple = p - 1;
	else
		multiple = 2 * p + 2;
	
	long long high = 0;
	for(long long i = 2; i * i <= multiple; i++)
		if(multiple % i == 0)
		{
			high = i;
			pair <long long, long long> temp = FindFibTerms(i, p);
			if(temp.first == 0 && temp.second == 1)
				return i;
		}
		
	for(long long i = high; i >= 2; i--)
		if(multiple % i == 0)
		{
			pair <long long, long long> temp = FindFibTerms(multiple / i, p);
			if(temp.first == 0 && temp.second == 1)
				return (multiple / i);
		}
		
	return multiple;
}
long long FindPisanoPeriod(long long num)
{
	long long period = 1;
	for(long long i = 2; i * i <= num; i++)
		if(num % i == 0)
		{
			int cnt = 0;
			while(num % i == 0)
			{
				cnt++;
				num /= i;
			}
			long long curr = Power(i, cnt - 1, INF) * FindPeriod(i);
			period = (period * curr) / __gcd(period, curr);
		}
	if(num > 1)
	{
		long long curr = FindPeriod(num);
		period = (period * curr) / __gcd(period, curr);
	}
	return period;
}
void Update(int num, int add)
{
	while(num > 1)
	{
		int curr = S[num];
		while(num % curr == 0)
		{
			val[curr] += add;
			num /= curr;
		}
	}
}
int main()
{
	int T;
	long long n, r, mod;
	PreCompute();
	
	scanf("%d", &T);
	while(T--)
	{
		scanf("%lld %lld %lld", &n, &r, &mod);
		long long period = FindPisanoPeriod(mod);
		memset(val, 0, sizeof(val));
		for(int i = 2; i <= n; i++)
			Update(i, 1);
		for(int i = 2; i <= r; i++)
			Update(i, -1);
		for(int i = 2; i <= n - r; i++)
			Update(i, -1);
		long long term = 1;
		for(int i = 2; i <= n; i++)
			term = (term * Power (i, val[i], period)) % period;
		
		pair <long long, long long> ans = FindFibTerms(term, mod);
		printf("%lld\n", ans.first);
	}
	return 0;
} 