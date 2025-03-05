#include<stdio.h>
int main(){
    int n;
    printf("enter no of elements in array : ");
    scanf("%d",&n);
    int nums[n];
    int cnt=0;
    int pos;
    printf("enter position : ");
    scanf("%d",&pos);
        for(int i=0;i<n;i++){
            scanf("%d",&nums[i]);
        }
        for(int i=pos-1;i<n;i++){
        nums[i]=nums[i+1];
    }
    nums[n--];
        for(int i=0;i<n;i++){
            printf("%d ",nums[i]);
        }
}
