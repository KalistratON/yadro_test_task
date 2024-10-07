#include <Tape.h>
#include <TapeSort.h>

#include <fstream>
#include <iostream>
#include <string>

#define CONFIG_FILE_PATH "../config.txt"


void SaveBinToTxt (const std::string& theFileName)
{
    std::ifstream aInFile (theFileName, std::ios::binary | std::ios::in);
    std::ofstream anOutFile (theFileName + "_TXT_VERSION.txt");

    int aNum;
    aInFile >> aNum;
    while (!aInFile.eof()) {
        anOutFile << aNum << '\n';
        aInFile >> aNum;
    }

    aInFile.close();
    anOutFile.close();
}

int main (int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Input file name of tape which should be sorted and file name of tape where sort should be execute!";
    }

    {
        Tape anInTape (argv[1], CONFIG_FILE_PATH);
        Tape anOutTape (argv[2], CONFIG_FILE_PATH);

        SortTape (anInTape, anOutTape, 512, 4);
    }

    SaveBinToTxt (argv[1]);
    SaveBinToTxt (argv[2]);

    return 0;
}
