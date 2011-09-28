#include "../GannetFileSystem.h"
#include <cstdio>
#include <iostream>

#include "../../src/UnitTest++.h"
#include "../../src/ReportAssert.h"
#include "../../src/TestReporterStdout.h"

TEST(test10)
{
	GannetFileSystem gfs10;
	//open file using c++ std library and
	FILE *fp;
	off_t len;
	const char *file,*file1, *file2;
	
	//file name
	file = "../byte.txt";
	file1= "../byte-1.txt";
	file2= "../byte-2.txt";
	fp = fopen (file,"r");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0, SEEK_SET);
	        
	char buffer_full[len];

	//store the data to buffer_full
	fread(buffer_full,len,1,fp);
	fclose(fp);
	
	//store the dummy-part1.txt
	fp = fopen(file1,"r");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0, SEEK_SET);

	char buffer_first[len];
	fread(buffer_first,len,1,fp);
	fclose(fp);

	//strore the dummy-part2.txt
	fp = fopen(file2,"r");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0, SEEK_SET);
	
	char buffer_second[len];
	fread(buffer_second,len,1,fp);
 	fclose(fp);

	//write the file into GannetFS
	int fd,fd1;
	const char *gfs10file;
	gfs10file = "dummy-1-gfs10.txt";	
	
	//create and write file
	fd = gfs10.open(gfs10file,O_WRONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
        CHECK_EQUAL(0,fd);
	int written = gfs10.write(fd, buffer_first, sizeof(buffer_first));
	CHECK_EQUAL(written,sizeof(buffer_first));
	
	//open the corresponding file for reading
	fd1 = gfs10.open(gfs10file,O_RDONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
	CHECK_EQUAL(1,fd1);
	gfs10.seek(fd1,0,SEEK_END);
	int length = gfs10.tell (fd1); 
	gfs10.seek(fd1,0,SEEK_SET);
	char bufout_1[length];
	int read = gfs10.read(fd1,bufout_1,length);
        CHECK_EQUAL(length,read);
	
	//append dummy-part2.txt to dummy-1-gfs10.txt 	
	gfs10.seek(fd,-1,SEEK_END); // offset set to -1 to overwrite 'EOF' char in the end file
	written = gfs10.write(fd,buffer_second, sizeof(buffer_second));
	CHECK_EQUAL(written,sizeof(buffer_second)+sizeof(buffer_first)-1);
	
	//re-read the file from begining
	gfs10.seek(fd1, 0, SEEK_END);
	length = gfs10.tell(fd1);
	gfs10.seek(fd1, 0, SEEK_SET);
	char bufout_full[length];
	read = gfs10.read(fd1, bufout_full, length);
	CHECK_EQUAL(length, read);


	int close=gfs10.close(fd);
	CHECK_EQUAL(0, close);
	close= gfs10.close(fd1);
	CHECK_EQUAL(0, close);
    
	//file name
	file = "../buffout.txt";
	fp = fopen (file,"w");
	fseek(fp,0,SEEK_END);
	int testlen = ftell(fp); 
	fseek(fp,0, SEEK_SET);
	
	//store the data in the buf buffer
	fwrite(bufout_full,length,1,fp);
	fclose(fp);

	int verifyCheck =  memcmp(buffer_full,bufout_full,len); 
	CHECK_EQUAL(0,verifyCheck);
       
}

