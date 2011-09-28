#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

TEST(test09)
{
	int mkdirCheck, fd;
	GannetFileSystem gfs9;
	fd = gfs9.open("./existingFile",O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0,fd);
	mkdirCheck = gfs9.mkdir("./existingFile",S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO );
	CHECK_EQUAL(17,mkdirCheck);
        mkdirCheck = gfs9.mkdir("./existingfile",S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO );
	CHECK_EQUAL(0,mkdirCheck);
	
	//cout << "mkdir: " << mkdirCheck<<endl;

}
