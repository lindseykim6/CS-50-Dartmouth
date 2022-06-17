#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

 int words(FILE* fp);


int
main(const int argc, const char* argv[])
{   
    if(argc==1){ // if no arguments, use stdin
        words(stdin);
    }
    else{
        for(int i=1; i<argc; i++) { 
            if(argv[i][0]=='-') { // if special argument -, switch to stdin
                words(stdin);

            }
            else{
                FILE* file = fopen(argv[i], "r"); // use file otherwise
                if(file ==NULL) { //print usage if file is null
                    fprintf(stderr,"usage: ./words %s; could not open file \n", argv[i]);
                    exit(1);
                }
                else{
                    words(file);
                }
            }

        }
    }
    exit(0);
    
}
int 
words(FILE* fp){
    char c; 
    //char* buffer=malloc(BUFSIZ); 
    //int i=0;
    c=fgetc(fp); // gets each character from file
    while(c != EOF) {
        if(isalpha(c)) // if it is an alphabetical character
        {  
            //buffer[i++]=c; //put it in buffer
            putchar(c); // print it
            c= fgetc(fp); 
        } else { // if it is not an alphabetical character
            char newlines='\n';
            //buffer[i++]=newlines;
            putchar(newlines); // print new line
            c=fgetc(fp); // keep parsing through until an alphabetical character is found or EOF
            while((c !=EOF) && (!isalpha(c))) {
                c = fgetc(fp);
            }
        }
    }
    //printf("%s", buffer); // print the buffer
    //free(buffer);
    //buffer=NULL; // reset buffer
    return 0;
}