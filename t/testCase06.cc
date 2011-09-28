#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST (test06)
{
        GannetFileSystem gfs6;
	int mkdirCheck;
	mkdirCheck = gfs6.mkdir("./test-six", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO); 
        CHECK_EQUAL(0,mkdirCheck);
	int fd0,fd1,fd2,fd3,fd4,fd5;
	fd0=gfs6.open("./test-six/file",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0,fd0);	
	fd1=gfs6.open("./test-six/file1",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(1,fd1);
	fd2=gfs6.open("./test-six/file2",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(2,fd2);
	fd3=gfs6.open("./test-six-a/file3",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(-1,fd3);
	fd4=gfs6.open("./test-siz/file4",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(-1,fd4);
	fd5=gfs6.open("./test-siy/file5",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(-1,fd5);


	int closeCheck = gfs6.close(fd0);
	CHECK_EQUAL(0,closeCheck);
	closeCheck = gfs6.close(fd1);
	CHECK_EQUAL(0,closeCheck);
	closeCheck = gfs6.close(fd2);
	CHECK_EQUAL(0,closeCheck);
	closeCheck = gfs6.close(fd3);
	CHECK_EQUAL(-1,closeCheck);
	closeCheck = gfs6.close(fd4);
	CHECK_EQUAL(-1,closeCheck);
	closeCheck = gfs6.close(fd5);
	CHECK_EQUAL(-1,closeCheck);
	//gfs6.listdir("./test-six/");

}
