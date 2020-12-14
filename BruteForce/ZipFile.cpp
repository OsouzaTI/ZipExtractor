#pragma warning(disable : 4996)
#include "ZipFile.h"
#include <string>

ZipFile::ZipFile(std::string fp, std::string fn)
	:filePath(fp), fileName(fn), filePassWord(""),
	hasPassWord(false), passWordLength(0)
{
	std::cout << "FileZip Class Instantiate :: " << fn << " in " << fp << std::endl;
}

ZipFile::~ZipFile()
{
	std::cout << "ZipFile Class has destruct" << std::endl;
}

std::string ZipFile::getFileName()
{
	return fileName;
}

std::string ZipFile::getFilePassWord()
{
	return filePassWord;
}

std::string ZipFile::getFilepath()
{
	return filePath;
}

void ZipFile::setFilePassWord(std::string passWord)
{
	filePassWord = passWord;
}

void ZipFile::setFileHasPassWord(bool hP)
{
	hasPassWord = hP;
}

bool ZipFile::extractFiles()
{
	std::string fileWithPath = filePath + '\\' + fileName;
	std::cout << fileWithPath << std::endl;
	if ((unzipFile = unzOpen(fileWithPath.c_str())) == NULL)return false;
	if ((unzGetGlobalInfo(unzipFile, &global_info)) != UNZ_OK) {
		std::cerr << "Error in open file " << fileName << " in path " << filePath << std::endl;
		unzClose(unzipFile);
		return false;
	}

	uLong i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		unz_file_info file_info;
		char file_name[MAX_FILENAME];
		if (unzGetCurrentFileInfo(
			unzipFile,
			&file_info, file_name,
			MAX_FILENAME, NULL,
			0, NULL, 0) != UNZ_OK)
		{
			unzClose(unzipFile);
			return false;
		}

		const size_t filename_length = strlen(file_name);
		if (file_name[filename_length - 1] == dir_delimter)
		{
			std::cout << "dir: " << file_name << std::endl;
		}
		else {
			std::cout << "file: " << file_name << std::endl;
			if (hasPassWord) {
				int findPassWord = BruteForcePassWord4Digits();
				if (findPassWord != 0) {
					std::cerr << "Could not possible find password" << std::endl;
					return false;
				}
			}
			else {
				int err = unzOpenCurrentFile(unzipFile);
				if (err != UNZ_OK) {
					std::cout << "Could not open file: " << file_name << std::endl;
					return false;
				}
				else {
					std::cout << "File " << file_name 
						<< " extrated. [" << ((i+1)/global_info.number_entry)*100 << " %]\n";
				}
			}
		}
		
		FILE* out = fopen(file_name, "wb");
		if (out == NULL) {
			std::cerr << " Could not open destination file" << std::endl;
			unzCloseCurrentFile(unzipFile);
			unzClose(unzipFile);
			return false;
		}

		int error = UNZ_OK;
		do
		{
			error = unzReadCurrentFile(unzipFile, readBuffer, READ_SIZE);
			if (error < 0) {
				std::cerr << " Could not read file " << std::endl;
				unzCloseCurrentFile(unzipFile);
				unzClose(unzipFile);
				return false;
			}

			if (error > 0) {
				fwrite(readBuffer, error, 1, out);
			}

		} while (error > 0);

		fclose(out);

	}

	unzCloseCurrentFile(unzipFile);
	
	if ((i + 1) < global_info.number_entry)
	{
		if (unzGoToNextFile(unzipFile) != UNZ_OK)
		{
			std::cerr << "cound not read next file " << std::endl;			
			unzClose(unzipFile);
			return false;
		}
	}

	return true;

}

int ZipFile::BruteForcePassWord4Digits()
{
	int error = -1;
	for (int i = 0; i < 9; i++)	{
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				for (int p = 0; p < 9; p++) {
					std::string ii = std::to_string(i);
					std::string jj = std::to_string(j);
					std::string kk = std::to_string(k);
					std::string pp = std::to_string(p);
					std::string pass = ii + jj + kk + pp;
					int err = unzOpenCurrentFilePassword(unzipFile, pass.c_str());
					if (err != UNZ_OK)
					{
						std::cout << "could not open file: " << err
							<< " passWord: " << pass << std::endl;						
					}
					else {
						std::cout << "Password OK: " << pass << std::endl;
						error = 0;
						return error;
					}
				}
			}
		}
	}
	return error;
}


