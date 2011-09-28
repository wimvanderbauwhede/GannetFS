#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST(test02)
{
     GannetFileSystem gfs2;
     //open file using c++ std library 
     FILE *fp1;
     off_t len1;
     const char *file1;

     //file name
     file1 = "../head.txt";
     fp1 = fopen (file1,"r");
     fseek(fp1,0,SEEK_END);
     len1 = ftell(fp1);
     fseek(fp1,0, SEEK_SET);
        
     char buffer2[len1];
     
     //store the data to buffer_full
     fread(buffer2,len1,1,fp1);
     fclose(fp1);
     // end of std library call

    

     int mkdirCheck, fdCheck1, fdCheck2, writeCheck, readCheck, length, verifyCheck, removeCheck, copyCheck;
     mkdirCheck = gfs2.mkdir("./test-two", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
     CHECK_EQUAL(0, mkdirCheck);
     
     fdCheck1 = gfs2.creat("./test-two/testFile-two",S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
     CHECK_EQUAL(0,fdCheck1);
  
     writeCheck = gfs2.write(fdCheck1,buffer2,len1);
     CHECK_EQUAL(writeCheck, len1);

     gfs2.close(fdCheck1);


     //test copying
     copyCheck = gfs2.cp("./test-two/testFile-two", "./test-two/testFile-copied");
     CHECK_EQUAL(0, copyCheck);


     //verifying the file copy
     fdCheck1 = gfs2.open("./test-two/testFile-copied", O_RDONLY,S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO );
    CHECK_EQUAL(0, fdCheck1);
     filestat fstat;
     gfs2.stat("./test-two/testFile-two", &fstat);
     length = fstat.st_size;
     CHECK_EQUAL(length,len1); 

     gfs2.seek(fdCheck1, 0, SEEK_SET);
     char buffer_read[length];
     readCheck = gfs2.read(fdCheck1, buffer_read, length);    
     CHECK_EQUAL(readCheck, len1);
     gfs2.close(fdCheck1);

     //file name
	const char* file_out = "../buffout_big_2.txt";
	fp1 = fopen (file_out,"w");
	fwrite(buffer_read,length,1,fp1);
	fclose(fp1);

     verifyCheck =  memcmp(buffer2,buffer_read,length); 
     CHECK_EQUAL(0, verifyCheck);
     cout<< "Verified: " << verifyCheck << endl;

     removeCheck = gfs2.remove("./test-two/testFile-two");
     CHECK_EQUAL(0, removeCheck);

}

