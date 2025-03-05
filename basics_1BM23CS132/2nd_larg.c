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
    int max=INT_MIN,smax=INT_MIN;
    for(int i=0;i<n;i++){
    if(nums[i]>max){
        max=nums[i];
    }
    }
    for(int i=0;i<n;i++){
        if(nums[i]>smax && nums[i]!=max){
            smax=nums[i];
        }
    }
    printf("second largest is %d",smax);
    }
