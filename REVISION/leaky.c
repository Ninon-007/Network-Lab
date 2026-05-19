#include<stdio.h>

int main()
{
    int stored=0,bucketcap,leaked,leakrate,n,i=0;
    printf("Enter the bucket capacity: ");
    scanf("%d",&bucketcap);
    printf("Enter the leakrate: ");
    scanf("%d",&leakrate);
    printf("Enter the number of incoming packets: ");
    scanf("%d",&n);
    int packet[n];

    for(i=0;i<n;i++)
    {
        printf("Enter the packets %d: ",i+1);
        scanf("%d",&packet[i]);
    }

    printf("Time\t\tPacket\t\tBucket\t\tLeaked\t\tRemaining\t\t\n");
    for(i=0;i<n;i++)
    {
        printf("%d\t\t%d",i+1,packet[i]);
        stored=stored+packet[i];
        
    
    if(stored>bucketcap)
    {
        printf("\t\t%d(overflow %d)",bucketcap,stored-bucketcap);
        stored=bucketcap;
    }
    else
    {
        printf("\t\t%d",stored);
    }
    if(stored>=leakrate)
    {
        leaked=leakrate;
    }
    else
    {
        leaked=stored;
    }
    stored=stored-leaked;
    printf("\t\t%d\t\t%d\n",leaked,stored);
    }
    int time=i+1;
    while(stored>0)
    {
      if(stored>=leakrate)
    {
        leaked=leakrate;
    }
    else
    {
        leaked=stored;
    }  

    printf("%d\t\t0\t\t%d\t\t%d\t\t%d\n",time,stored,leaked,stored - leaked);

        stored = stored - leaked;

        time++;
    }

    return 0;
}