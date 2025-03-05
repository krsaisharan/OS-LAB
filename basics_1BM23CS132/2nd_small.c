#include<stdio.h>
#include<limits.h>
int main(){
int n;
    printf("enter no of elements in array : ");
    scanf("%d",&n);
    int nums[n];
        for(int i=0;i<n;i++){
            scanf("%d",&nums[i]);
        }
    int min=INT_MAX,smin=INT_MAX;
    for(int i=0;i<n;i++){
    if(nums[i]<min){
        min=nums[i];
    }
    }
    for(int i=0;i<n;i++){
        if(nums[i]<smin && nums[i]!=min){
            smin=nums[i];
        }
    }
    printf("second smallest is %d",smin);
    }
