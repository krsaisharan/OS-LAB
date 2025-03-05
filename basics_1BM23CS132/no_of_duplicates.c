#include<stdio.h>
int main(){
    int n;
    printf("enter no of elements in array : ");
    scanf("%d",&n);
    int nums[n];
    int cnt=0;
        for(int i=0;i<n;i++){
            scanf("%d",&nums[i]);
        }
        for(int i=0;i<n;i++){
            if(nums[i]!=-1){
            for(int j=i+1;j<n;j++){
                if(nums[i]==nums[j]){
                    cnt++;
                    nums[j]=-1;
                    break;
                }
            }
        }
    }
    printf("no of duplicates is %d",cnt);
}
