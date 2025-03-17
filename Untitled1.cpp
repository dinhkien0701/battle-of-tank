#include <bits/stdc++.h>
using namespace std;
bool sol(int l, int r ,int c){
	if(l<=0&&r>0)return false;
	if(l<c)return false;
	return true;
}
int main(){
	int m,n,k;
	int x,y,a,b;
	cin>>m>>n>>k;
	int ans=0;
	for(int i=1;i<=k;i++){
		cin>>x>>y>>a>>b;
		a+=x;
		b+=y;
		ans+=(sol(x,a,m)||sol(y,b,n));
	}
	cout<<ans;
}