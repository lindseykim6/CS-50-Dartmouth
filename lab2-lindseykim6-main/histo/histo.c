#include <stdio.h>
#include <stdlib.h>

int
main()
{ 
    int data=0;
    int num_bins=16; // number of bins set to 16
    int bin_count[num_bins]; // array for keeping track of bins
    int range = num_bins; // the range of the histogram
    int bin_size= range/num_bins; // bin size of histogram

    for(int i=0; i<num_bins; i++) {
        bin_count[i]=0; // initializes all bins to 0 count initially
    }

    printf("%d bins of size %d for range [0,%d)\n", num_bins, bin_size, range);

    while(scanf("%d", &data) != EOF) {
        if(data>=0){
            while(data >= range){ // if the data isn't in range
                range=range*2; // double range
                bin_size=bin_size*2; // double bin size
                printf("%d bins of size %d for range [0,%d)\n", num_bins, bin_size, range);
                for(int i=0; i<(num_bins/2); i++) {
                    bin_count[i]=bin_count[2*i]+bin_count[(2*i)+1]; // compress bins
                }
                for(int i=(range/2); i<range; i+=bin_size) {
                    bin_count[i/bin_size]=0; // set remaining bins to 0
                }
            }            
            for(int i=0; i<range; i+=bin_size) {
                if((i<=data) && (data<(i+bin_size))) {
                    bin_count[i/bin_size]++; // increments bins if within bin range

                }
            }
        }
                
            
    }
    for(int i=0; i<range; i+=bin_size) {
        printf("[%3d: %3d]", i, i+bin_size-1); 
        for(int j=0; j<bin_count[i/bin_size]; j++) {
            putchar('*'); // prints out histogram
        }
        putchar('\n');
    }
    exit(0);
} 
