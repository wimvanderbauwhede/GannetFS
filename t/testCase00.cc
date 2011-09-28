#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST (testCase00)
{
  	GannetFileSystem gfs;
	FILE *fp_big;
	off_t len_big;
	const char *file_big;
	
	//file name
	file_big = "../testing.txt";
	
	fp_big = fopen (file_big,"r");
	fseek(fp_big,0,SEEK_END);
	len_big = ftell(fp_big);
	fseek(fp_big,0, SEEK_SET);
	
	char buffer_full_big[len_big];
        
	//store the data to buffer_full
	fread(buffer_full_big,len_big,1,fp_big);
	fclose(fp_big);

	int fd_big;
	const char *GFSfile_big;
	GFSfile_big = "head-gfs.txt";	
	
	//create and write file
	fd_big = gfs.open(GFSfile_big,O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(0,fd_big);

	int written = gfs.write(fd_big, buffer_full_big, len_big);
	CHECK_EQUAL(len_big,written);

        gfs.close (fd_big);
        fd_big = gfs.open(GFSfile_big,O_RDONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0,fd_big);
        
        char buff_output_big[len_big];
        gfs.read(fd_big, buff_output_big, len_big);
        gfs.close (fd_big);
	
	int verifyCheck =  memcmp(buffer_full_big,buff_output_big,len_big); 
	CHECK_EQUAL(0,verifyCheck);
        
        //file name
	const char* file_out = "../buffout_big.txt";
	FILE* fp = fopen (file_out,"w");
	fwrite(buff_output_big,len_big,1,fp);
	fclose(fp);
        
	gfs.remove("head-gfs.txt");
        //gfs.listdir(".");

}
