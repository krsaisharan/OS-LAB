#include<stdio.h>
void main()
{
    int Max[20] = {0};
    int n, start, count, end = 0;

    while(1)
    {
        count = 1;
        printf("Enter the Start index for occupying memory:");
        scanf("%d", &start);
        printf("Enter the number of blocks required:");
        scanf("%d", &n);

        for(int i = start; i < start + n; i++)
        {
            if(Max[i] == 1)
            {
                printf("not possible to allocate\n");
                count = 0;
                break;
            }
        }

        if(count == 1)
        {
            for(int i = start; i < start + n; i++)
            {
                Max[i] = 1;
            }
            printf("allocation successful\n");
        }

        printf("Do you want to continue? (0 to continue, 1 to stop):");
        scanf("%d", &end);
        if(end == 1)
            break;
    }
}
