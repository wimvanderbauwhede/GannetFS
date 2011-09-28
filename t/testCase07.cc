#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"
TEST (test07)
{
	GannetFileSystem gfs7;
	int mkdirCheck1, mkdirCheck2, mkdirCheck3, fd1, fd2, fd3, fd4;
	mkdirCheck1 = gfs7.mkdir("./test-seven", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
	CHECK_EQUAL(0,mkdirCheck1);
	fd1 = gfs7.open("./test-seven/File",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(0,fd1);	
	mkdirCheck2 = gfs7.mkdir("./test-seven/test-seven-a", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0,mkdirCheck2);
	fd2 = gfs7.open("./test-seven/test-seven-a/aFile",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);	
	CHECK_EQUAL(1,fd2); 
	mkdirCheck3 = gfs7.mkdir("./test-seven/test-seven-a/test-seven-b", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
	CHECK_EQUAL(0,mkdirCheck3);
	fd3 = gfs7.open("./test-seven/test-seven-a/test-seven-b/bFile",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(2,fd3);

	int closeCheck=gfs7.close(fd1);
	CHECK_EQUAL(0,closeCheck);
	closeCheck=gfs7.close(fd2);
	CHECK_EQUAL(0,closeCheck);
	closeCheck=gfs7.close(fd3);
	CHECK_EQUAL(0,closeCheck);
	
	
	//cout<<"mkdirCheck1: "<<mkdirCheck1<<endl;
	//cout<<"mkdirCheck2: "<<mkdirCheck2<<endl;
	//cout<<"mkdirCheck3: "<<mkdirCheck3<<endl;

	//gfs7.listdir("./test-seven");
	//gfs7.listdir("./test-seven/test-seven-a");
	//gfs7.listdir("./test-seven/test-seven-a/test-seven-b");

}
