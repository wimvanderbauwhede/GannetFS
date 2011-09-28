#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST (test01)
{
     GannetFileSystem gfs1;
     //open file using c++ std library 
     FILE *fp;
     off_t len;
     const char *file;

     //file name
     file = "../testing.txt";
     fp = fopen (file,"r");
     fseek(fp,0,SEEK_END);
     len = ftell(fp);
     fseek(fp,0, SEEK_SET);
        
     char buffer[len];

     //store the data to buffer_full
     fread(buffer,len,1,fp);
     fclose(fp);
     // end of std library call


     int mkdirCheck, fdCheck1, fdCheck2, writeCheck, readCheck, length, verifyCheck, removeCheck;
     mkdirCheck = gfs1.mkdir("./test-one", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
     CHECK_EQUAL(0,mkdirCheck);
     
     fdCheck1 = gfs1.creat("./test-one/testFile-one",S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
     CHECK_EQUAL(0,mkdirCheck);

     writeCheck = gfs1.write(fdCheck1,buffer,len);
     CHECK_EQUAL(len,writeCheck);

     int checkClose1 = gfs1.close(fdCheck1);
     CHECK_EQUAL(0,checkClose1);

     //verifying
     fdCheck1 = gfs1.open("./test-one/testFile-one", O_RDONLY,S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO );
     CHECK_EQUAL(0,fdCheck1);
     
     filestat fstat;
     gfs1.stat("./test-one/testFile-one", &fstat);
     length = fstat.st_size;
     gfs1.seek(fdCheck1, 0, SEEK_SET);
     char buffer_read[length];
     readCheck = gfs1.read(fdCheck1, buffer_read, length);   
     CHECK_EQUAL(length,readCheck);
     
     checkClose1 = gfs1.close(fdCheck1);  
     CHECK_EQUAL(0,checkClose1 );

     //file name
	const char* file_out = "../buffout_big.txt";
	fp = fopen (file_out,"w");
	fwrite(buffer_read,length,1,fp);
	fclose(fp);

     verifyCheck =  memcmp(buffer,buffer_read,length); 
     CHECK_EQUAL(0, verifyCheck);

     //cout<< "Verified: " << verifyCheck << endl;
      
     //cout<<"removing!"<<endl;
     //removeCheck = gfs1.remove("./test-one/testFile-one");
     removeCheck = gfs1.unlink("./test-one/testFile-one");
     CHECK_EQUAL(0, removeCheck);
     int rmCheck = gfs1.rmdir ("./test-one");
     CHECK_EQUAL(0, rmCheck);
     
    
}
