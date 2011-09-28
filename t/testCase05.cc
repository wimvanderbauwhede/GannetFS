#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

using std::cout;
using std::endl;

TEST(test05)
{
	GannetFileSystem gfs5;
	int mkdirCheck0, mkdirCheck1,mkdirCheck2,mkdirCheck3,mkdirCheck4,mkdirCheck5;
	mkdirCheck0= gfs5.mkdir("./test-five", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(0, mkdirCheck0);
	mkdirCheck1= gfs5.mkdir("./test-five/abcdefghijklmnopq", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0, mkdirCheck1);
	mkdirCheck2= gfs5.mkdir("./test-five/abcdefghijklmnopqrs", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(0, mkdirCheck2);
	mkdirCheck3= gfs5.mkdir("./test-five/abcdefghijklmnopqrst", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(1, mkdirCheck3);//ETOOBIG
	mkdirCheck4= gfs5.mkdir("./test-five/abcdefghijklmnopqrstuv", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(1, mkdirCheck4);//ETOOBIG
	mkdirCheck5= gfs5.mkdir("./test-five/abcdefghijklmnopqrstuvwx", S_IFDIR | S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(1, mkdirCheck5);//ETOOBIG

	//gfs5.listdir("./test-five/");


}

