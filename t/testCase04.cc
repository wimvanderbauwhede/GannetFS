#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST(test04)
{
	GannetFileSystem gfs4;
	int mkdirCheck;
	mkdirCheck = gfs4.mkdir("./test-four", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
        CHECK_EQUAL(0,mkdirCheck);
	
	int fd0,fd1,fd2,fd3,fd4,fd5,fd6,fd7,fd8,fd9,fd10,fd11,fd12,fd13,fd14,fd15,fd16,fd17;

	FILE *fp;
	off_t len;
	const char *file;
	
	//file name
	file = "../byte.txt";
	fp = fopen (file,"r");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0, SEEK_SET);
	        
	char buffer[len];

	//store the data to buffer_full
	fread(buffer,len,1,fp);
	fclose(fp);
	

	fd0=gfs4.open("./test-four/file",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(0, fd0);	
	fd1=gfs4.open("./test-four/file1",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(1, fd1);		
	fd2=gfs4.open("./test-four/file2",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(2, fd2);	
	fd3=gfs4.open("./test-four/file3",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(3, fd3);	
	fd4=gfs4.open("./test-four/file4",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(4, fd4);	
	fd5=gfs4.open("./test-four/file5",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(5, fd5);	
	fd6=gfs4.open("./test-four/file6",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(6, fd6);	
	fd7=gfs4.open("./test-four/file7",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(7, fd7);	
	fd8=gfs4.open("./test-four/file8",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(8, fd8);	
	fd9=gfs4.open("./test-four/file9",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(9, fd9);	
	fd10=gfs4.creat("./test-four/file10", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(10, fd10);	
	fd11=gfs4.creat("./test-four/file11", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(11, fd11);	
	fd12=gfs4.creat("./test-four/file12", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(12, fd12);	
	fd13=gfs4.creat("./test-four/file13", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(13, fd13);	
	fd14=gfs4.creat("./test-four/file14", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(14, fd14);	
	fd15=gfs4.creat("./test-four/file15", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(15, fd15);	
	fd16=gfs4.creat("./test-four/file16", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(-1, fd16);	
	fd17=gfs4.creat("./test-four/file17", S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(-1, fd17);	

	int written = gfs4.write(fd0, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd1, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd2, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd3, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd4, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd5, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd6, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd7, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd8, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd9, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd10, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd11, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd12, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd13, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd14, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd15, buffer, len);
        CHECK_EQUAL(len, written);
	written = gfs4.write(fd16, buffer, len);
        CHECK_EQUAL( 0, written);
	written = gfs4.write(fd17, buffer, len);
	CHECK_EQUAL(0, written);

	//gfs4.listdir("./test-four/");
	
	int closeCheck = gfs4.close(fd0);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd1);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd2);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd3);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd4);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd5);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd6);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd7);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd8);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd9);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd10);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd11);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd12);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd13);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd14);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd15);
	CHECK_EQUAL(0,closeCheck);
	closeCheck =gfs4.close(fd16);
	CHECK_EQUAL(-1,closeCheck);
	closeCheck =gfs4.close(fd17);
	CHECK_EQUAL(-1,closeCheck);
	

	int remCheck =gfs4.remove("./test-four/file");
	CHECK_EQUAL(0,remCheck); 
	remCheck =gfs4.remove("./test-four/file0"); //shall not OK
	CHECK_EQUAL(2,remCheck); //ENOENT
	remCheck =gfs4.remove("./test-four/file1");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file2");
	CHECK_EQUAL(0,remCheck);	
	remCheck =gfs4.remove("./test-four/file3");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file4");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file5");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file6");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file7");
	CHECK_EQUAL(0,remCheck);	
	remCheck =gfs4.remove("./test-four/file8");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file9");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file10");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file11");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file12");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file13");
	CHECK_EQUAL(0,remCheck);	
	remCheck =gfs4.remove("./test-four/file14");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file15");
	CHECK_EQUAL(0,remCheck);
	remCheck =gfs4.remove("./test-four/file16");
	CHECK_EQUAL(2,remCheck);//ENOENT
	remCheck =gfs4.remove("./test-four/file17");
	CHECK_EQUAL(2,remCheck);//ENOENT


	//gfs4.listdir("./test-four/");


}
