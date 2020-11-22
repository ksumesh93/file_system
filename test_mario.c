#include "LibFS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *prog)
{
  printf("USAGE: %s <disk_image_file>\n", prog);
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 2) usage(argv[0]);

  if(FS_Boot(argv[1]) < 0) {
    printf("ERROR: can't boot file system from file '%s'\n", argv[1]);
    return -1;
  } else printf("file system booted from file '%s'\n", argv[1]);

  char* fn;

  /**
  Try to delete a file that doesn't exist in the filesystem
  It should return an ERROR
  **/
  fn = "/first-file";
  if(File_Unlink(fn) < 0) printf("ERROR: can't unlink file '%s'\n", fn);
  else printf("file '%s' unlinked successfully\n", fn);

  /**
  Add a file to the file system
  **/
  fn = "/first-file";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);

  /**
  Add a file to the file system that already exists
  It should return and ERROR
  **/
  fn = "/first-file";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);

/**
  Read a file that does not exists
  It should return and ERROR
  **/
  fn = "/second-file";
  int fd = File_Open(fn);
  if(fd < 0) printf("ERROR: can't open file '%s'\n", fn);
  else printf("file '%s' opened successfully, fd=%d\n", fn, fd);

  /**
  Try to remove a directory that does not exist
  Should return an ERROR
  **/
  fn = "/first-dir";
  if(Dir_Unlink(fn) < 0) printf("ERROR: can't unlink dir '%s'\n", fn);
  else printf("dir '%s' unlinked successfully\n", fn);

/**
  Creates a directory
  **/
  fn = "/first-dir";
  if(Dir_Create(fn) < 0) printf("ERROR: can't create dir '%s'\n", fn);
  else printf("dir '%s' created successfully\n", fn);

  /**
  Try to create recursive directories
  Should return an ERROR
  **/
  fn = "/a/b";
  if(Dir_Create(fn) < 0) printf("ERROR: can't create dir '%s'\n", fn);
  else printf("dir '%s' created successfully\n", fn);

/**
  Creates a file inside a directory
  **/ 
  fn = "/first-dir/second-file";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);

 /**
  Try to delete the root directory
  Should return an ERROR
  **/
  fn = "/";
  if(Dir_Unlink(fn) < 0) printf("ERROR: can't unlink dir '%s'\n", fn);
  else printf("dir '%s' unlinked successfully\n", fn);

/**
  Try to delete a non empty directory
  Should return an ERROR
  **/
  fn = "/first-dir";
  if(Dir_Unlink(fn) < 0) printf("ERROR: can't unlink dir '%s'\n", fn);
  else printf("dir '%s' unlinked successfully\n", fn);

  /**
  Read a file inside a directory
  **/
  fn = "/first-dir/second-file";
  fd = File_Open(fn);
  if(fd < 0) printf("ERROR: can't open file '%s'\n", fn);
  else printf("file '%s' opened successfully, fd=%d\n", fn, fd);
 
  char buf[1024]; char* ptr = buf;
  for(int i=0; i<1000; i++) {
    sprintf(ptr, "%d %s", i, (i+1)%10==0 ? "\n" : "");
    ptr += strlen(ptr);
    if(ptr >= buf+1000) break;
  }
  /**
  Write to a file
  **/
  if(File_Write(fd, buf, 1024) != 1024)
    printf("ERROR: can't write 1024 bytes to fd=%d\n", fd);
  else printf("successfully wrote 1024 bytes to fd=%d\n", fd);
  /**
  Close file
  **/
  if(File_Close(fd) < 0) printf("ERROR: can't close fd %d\n", fd);
  else printf("fd %d closed successfully\n", fd);

  /**
  Creates a file that already exist inside a directory
  Should return an ERROR
  **/ 
  fn = "/first-dir/second-file";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);

  /**
  Creates a directory that already exist
  Should return ERROR
  **/
  fn = "/first-dir";
  if(Dir_Create(fn) < 0) printf("ERROR: can't create dir '%s'\n", fn);
  else printf("dir '%s' created successfully\n", fn);


  /**
  Creates a file with more than 16 characters
  Should return an ERROR
  **/ 
  fn = "/abcdefghijklmnopqrs";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);

/**
  Creates a file with non valid characters
  Should return an ERROR
  **/ 
  fn = "/abcd*hij";
  if(File_Create(fn) < 0) printf("ERROR: can't create file '%s'\n", fn);
  else printf("file '%s' created successfully\n", fn);


if(FS_Sync() < 0) {
    printf("ERROR: can't sync file system to file '%s'\n", argv[1]);
    return -1;
  } else printf("file system sync'd to file '%s'\n", argv[1]);


/**
  Try to create 1000 file
  Should return an ERROR for the last ones when over limit
  **/ 
for(int i=0;i<1000;i++){ 
  char result[10];
  fn = "/";
  sprintf(result,"%d",i);
  char *final_var = malloc(strlen(result) + strlen(fn) + 1); 
  strcpy(final_var, fn);
  strcat(final_var, result);
  if(File_Create(final_var) < 0) printf("ERROR: can't create file '%s'\n", final_var);
  //else printf("file '%s' created successfully\n", fn);
}
  /**
  Try to open more than 256 files
  Should return an ERROR for the last ones
  **/
 for(int i=0;i<258;i++){
  char result1[10];
  fn = "/";
  sprintf(result1,"%d",i);
  char *final_var1 = malloc(strlen(result1) + strlen(fn) + 1); 
  strcpy(final_var1, fn);
  strcat(final_var1, result1);
  fd = File_Open(final_var1);
  if(fd < 0) printf("ERROR: can't open file '%s'\n", final_var1);
  //else printf("file '%s' opened successfully, fd=%d\n", fn, fd);
}
  
  if(FS_Sync() < 0) {
    printf("ERROR: can't sync file system to file '%s'\n", argv[1]);
    return -1;
  } else printf("file system sync'd to file '%s'\n", argv[1]);
    
  return 0;
}
