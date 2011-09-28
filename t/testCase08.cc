#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

TEST(test08)
{
	GannetFileSystem gfs8;
	int fd,fd1, mkdirCheck;
        mkdirCheck = gfs8.mkdir("./existingDir",S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0,mkdirCheck);
	fd = gfs8.open("./existingDir",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);	
        CHECK_EQUAL(-1,fd);
	fd1 = gfs8.open("./existingdir",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);	
        CHECK_EQUAL(0,fd1);
	int closeCheck =gfs8.close(fd);
	CHECK_EQUAL(-1,fd);	
	closeCheck =gfs8.close(fd1);
	CHECK_EQUAL(0,fd1);

	//cout<<"fd: "<<fd<<endl;
	//gfs8.listdir(".");

}
