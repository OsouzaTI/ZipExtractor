#ifndef ZIP_FILE_H
#define ZIP_FILE_H

#include <iostream>
#include <minizip/unzip.h>
#include <minizip/zip.h>

#define READ_SIZE 8192
#define dir_delimter '/'
#define MAX_FILENAME 512

class ZipFile
{
public:
	ZipFile(std::string fp, std::string fn);
	~ZipFile();
	std::string getFileName();
	std::string getFilePassWord();
	std::string getFilepath();

	void setFilePassWord(std::string passWord);
	void setFileHasPassWord(bool hP);
	bool extractFiles();

	int BruteForcePassWord4Digits();

private:
	bool hasPassWord;
	unsigned short int passWordLength;
	char readBuffer[READ_SIZE];
	std::string filePath;
	std::string fileName;
	std::string filePassWord;

	unzFile unzipFile;
	unz_global_info global_info;

};

#endif // !ZIP_FILE

