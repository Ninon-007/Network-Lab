#include<stdio.h>

int main()
{
    int bucketcap, leakrate;
    int packets, i;

    int bucket = 0;
    int leaked;

    printf("Enter bucket capacity: ");
    scanf("%d",&bucketcap);

    printf("Enter leak rate: ");
    scanf("%d",&leakrate);

    printf("Enter number of packets: ");
    scanf("%d",&packets);

    int packet[packets];

    printf("Enter packet sizes:\n");

    for(i=0;i<packets;i++)
    {
        scanf("%d",&packet[i]);
    }

    printf("\nLeaky Bucket Simulation\n");

    printf("Time\tInput\tBucket\tLeaked\tRemaining\n");

    for(i=0;i<packets;i++)
    {
        bucket = bucket + packet[i];

        printf("%d\t%d",
               i+1,
               packet[i]);

        if(bucket > bucketcap)
        {
            printf("\t%d (overflow %d)",
                   bucketcap,
                   bucket - bucketcap);

            bucket = bucketcap;
        }

        else
        {
            printf("\t%d",
                   bucket);
        }

        if(bucket >= leakrate)
        {
            leaked = leakrate;
        }

        else
        {
            leaked = bucket;
        }

        bucket = bucket - leaked;

        printf("\t%d\t%d\n",leaked, bucket);
    }

    int time = i + 1;

    while(bucket > 0)
    {
        if(bucket >= leakrate)
        {
            leaked = leakrate;
        }

        else
        {
            leaked = bucket;
        }

        printf("%d\t0\t%d\t%d\t%d\n",time,bucket,leaked,bucket - leaked);

        bucket = bucket - leaked;

        time++;
    }

    return 0;
}