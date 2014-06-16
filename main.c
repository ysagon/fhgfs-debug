#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <sys/mman.h>

int main(){
   
   char * fname = "test619.chk";

   int fh;

   void * mh;

   int res;

   int bs;

   int buffSize;

   struct statfs   sts;

   if (open(fname, O_RDWR) != -1){
      fprintf(stderr, "file %s already exist\n", fname);
      exit(1);
   }  

   fh = open(fname, O_RDWR|O_CREAT|O_TRUNC, 0666);

   if(fh <= 0){
      perror("file could not be created\n");
      exit(1);
   }

   if(res = lseek(fh, 0, SEEK_END) != 0){
      fprintf(stderr, "newly created file size is not 0\n");
      exit(1);
   }

   if(fstatfs(fh, & sts) !=0){
      fprintf(stderr, "could not get fstatfs\n");
      exit(1);
   }

   bs = sts.f_bsize;

   buffSize = bs+4096; 

   mh = mmap(NULL, buffSize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
   if(mh <= 0){
      fprintf(stderr, "could not create buffer\n");
      exit(1);
   }

   res = write(fh, mh, bs);
   if(res <= 0){
      fprintf(stderr, "could not write to the file\n");
      exit(1);
   }

   res = lseek(fh, 0, SEEK_END);

   if (res <= 0){
      fprintf(stderr, "the file is still empty after writing to it\n");
      exit(1);
   }

   fprintf(stdout, "Ok, working fine");
   exit(0);
}
