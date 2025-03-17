#include <bits/stdc++.h>
using namespace std;
void sol(string &str, string &ans,int k,bool ck[]){
    if(k>=2){
        cout<<ans<<'\n';
        if(k==3)return;
    }
    for(int i=0;i<(int)str.size();i++){
        if(!ck[i]){
            ans+=str[i];
            ck[i]=true;
            sol(str,ans,k+1,ck);
            ans.pop_back();
            ck[i]=false;
        }
    }
}
int main(){
    string str;
    cin>>str;
    int n=(int)str.size();
    string ans;
    bool ck[n];
    for(int i=0;i<n;i++)ck[i]=false;
    sol(str,ans,0,ck);
}