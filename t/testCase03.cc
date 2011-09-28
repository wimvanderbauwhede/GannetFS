#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST (test03)
{
     GannetFileSystem gfs3;
     //open file using c++ std library 
     FILE *fp;
     off_t len;
     const char *file;

     //file name
     file = "../head.txt";
     fp = fopen (file,"r");
     fseek(fp,0,SEEK_END);
     len = ftell(fp);
     fseek(fp,0, SEEK_SET);
        
     char buffer3[len];

     //store the data to buffer_full
     fread(buffer3,len,1,fp);
     fclose(fp);
     // end of std library call


     int mkdirCheck, fdCheck1, fdCheck2, writeCheck, readCheck, length, verifyCheck, removeCheck, moveCheck;
     mkdirCheck = gfs3.mkdir("./test-three", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
     CHECK_EQUAL(0,mkdirCheck);
     
     fdCheck1 = gfs3.creat("./test-three/testFile-three",S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
     
     
     writeCheck = gfs3.write(fdCheck1,buffer3,len);
     CHECK_EQUAL(len, writeCheck);

     gfs3.close(fdCheck1);
    

     //test moving file
     moveCheck = gfs3.mv("./test-three/testFile-three", "./test-three/testFile-moved");
     CHECK_EQUAL(0, moveCheck);

     //verifying
     fdCheck1 = gfs3.open("./test-three/testFile-moved", O_RDONLY,S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO );
     
     
     filestat fstat;
     gfs3.stat("./test-three/testFile-moved", &fstat);
     length = fstat.st_size;
     gfs3.seek(fdCheck1, 0, SEEK_SET);
     char buffer_read[length];
     readCheck = gfs3.read(fdCheck1, buffer_read, length);  
     CHECK_EQUAL(readCheck, len);
     
     gfs3.close(fdCheck1);   

     //gfs3.listdir("./test-three");
     //file name
	const char* file_out = "../buffout_big.txt";
	fp = fopen (file_out,"w");
	fwrite(buffer_read,length,1,fp);
	fclose(fp);

     verifyCheck =  memcmp(buffer3,buffer_read,length); 
     CHECK_EQUAL(0, verifyCheck);
     cout<< "Verified: " << verifyCheck << endl;
     removeCheck = gfs3.remove("./test-three/testFile-moved");
     CHECK_EQUAL(0, removeCheck);
}
