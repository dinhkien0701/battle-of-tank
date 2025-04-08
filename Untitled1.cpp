#include <bits/stdc++.h>
using namespace std;
int main(){
	long long n,k,res=0;
	cin>>n>>k;
	long long a[n+1];
	long long before = -2*k;
	long long present= -2*k;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	sort(a+1,a+n+1);
	for(int i=1;i<=n;i++){
		if(a[i]-k<=before){
			present =a[i];
		}
		else if(a[i]-k>present){
			present =a[i];
			res++;
			before =a[i];
		}
		
	}
	cout<<res;
}