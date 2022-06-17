#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main(const int argc, const char* argv[])
{
    if(argc==1) { // if no arguments are given, use default values
        printf("  Temp   Wind  Chill\n");
        printf(" -----  -----  -----\n");
        for(float temp = -10.0; temp <=40.0; temp +=10.0) {
            for(float wind = 5.0; wind <= 15.0; wind += 5.0) {
                float chill=35.74 + (0.6215*temp) - (35.75*(pow(wind, 0.16))) + (0.4275*temp*(pow(wind, 0.16))); // calculates chill based on wind and temp
                printf("%6.1f%7.1f%7.1f\n", temp, wind, chill); // prints as a table
            }
            printf("\n");
        }
        exit(0); //exits if successful
    }
    else if(argc==2) { // if user provides temperature
        float temp=0; // temperature
        if(sscanf(argv[1], "%f", &temp) && (temp < 50) && (temp > -99)) { // temperature within range
            printf("  Temp   Wind  Chill\n");
            printf(" -----  -----  -----\n");
            for(float wind = 5.0; wind <= 15.0; wind += 5.0) { //use default wind values
                float chill=35.74 + (0.6215*temp) - (35.75*(pow(wind, 0.16))) + (0.4275*temp*(pow(wind, 0.16))); // calculates chill based on temp and wind
                printf("%6.1f%7.1f%7.1f\n", temp, wind, chill);
            }
            exit(0);
        }
        else { // usage statement (range error)
            printf("%s: Temperature must be less than 50.0 degrees Fahrenheit and greater than -99.0 degrees Fahrenheit.\n", argv[0]);
            exit(1);
        }
    }
    else if (argc==3) { // if user provides both temperature and wind speed 
       float temp=0; // temperature
       float wind=0; // wind speed
       if(sscanf(argv[1], "%f", &temp) && (temp < 50) && (temp > -99)) { // if temperature is within range 
           if(sscanf(argv[2], "%f", &wind) && (wind >=0.5)) { // if wind chill is within range
               printf("  Temp   Wind  Chill\n");
               printf(" -----  -----  -----\n");
               float chill=35.74 + (0.6215*temp) - (35.75*(pow(wind, 0.16))) + (0.4275*temp*(pow(wind, 0.16))); 
               printf("%6.1f%7.1f%7.1f\n", temp, wind, chill);
               exit(0);
            }
            else { // usage statement (range error for wind)
               printf("%s: Wind velocity must be greater than or equal to 0.5 MPH.\n", argv[0]);
               exit(2); 
            }
       }
       else { // usage statement (range error for temp)
            printf("%s: Temperature must be less than 50.0 degrees Fahrenheit and greater than -99.0 degrees Fahrenheit.\n", argv[0]);
            if(sscanf(argv[2], "%f", &wind) && (wind < 0.5)) {
               printf("%s: Wind velocity must be greater than or equal to 0.5 MPH.\n", argv[0]);
               exit(2);  
            }
            exit(2);
       }
        
    }
    else{
        printf("usage: %s; too many arguments \n", argv[0]);
        exit(3);
    }
}
