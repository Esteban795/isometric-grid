#include <stdlib.h>
#include <stdio.h>


/*
Input : ./file_generator filename n low high
- filename : filename of output
- n : number of 2D coordinates points you wanna get.
- [low,high] : interval for which random coordinates will emerge from
*/

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}



void write_file(FILE* f,int n,int l,int r){
    int temp_1,temp_2,temp_3;
    fprintf(f,"%d\n",n);
    for (int i = 0; i < n;i++){
        temp_1 = rand_between(l,r);
        temp_2 = rand_between(l,r);
        temp_3 = rand_between(l,r);
        fprintf(f,"%d;%d;%d\n",temp_1,temp_2,temp_3);
    }
}