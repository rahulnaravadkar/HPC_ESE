#include <omp.h>
#include <bits/stdc++.h>
using namespace std;

int main() {
	int n = 5;
	#pragma omp parallel
	{
		#pragma omp master
		{
			for(int i = n; i>= 1; i--)
			{
				for(int j = 1; j <= i; j++)
				{
					cout<<"*";
				}
				cout<<endl;
			}
		}
	}
	
}
