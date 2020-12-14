#include "ZipFile.h"

int main() {
	const char* meuArquivo = "facil.zip";
	ZipFile myZip = ZipFile("F:\\Projects\\cpp\\BruteForce", meuArquivo);
	myZip.setFileHasPassWord(true);
	if(myZip.extractFiles()) std::cout << "Extracao Concluida vadias" << std::endl;
	return 0;
}