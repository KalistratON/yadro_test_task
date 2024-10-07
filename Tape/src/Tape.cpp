#pragma warning (disable : 4996)

#include "Tape.h"
#include "TapeImpl.h"

#include <fstream>
#include <iostream>
#include <thread>

#define TEST_ALGO_MODE 1


namespace internal {

namespace {

bool ReadConfig (const std::string& theConfigFileNam, int (&theLatencies)[4])
{
    std::ifstream aFile (theConfigFileNam);

    if (!aFile.is_open()) {
        return false;
    }

    const int aBufferSize = 100;
    char aBuffer [aBufferSize];

    for (size_t i = 0; i < 4; ++i) {
        aFile.getline (aBuffer, aBufferSize, ':');
        aFile.getline (aBuffer, aBufferSize, '\n');

        if (aFile.eof()) {
            return false;
        }

        int aLatencyParam = atoi (aBuffer);
        if (aLatencyParam < 0) {
            return false;
        }

        theLatencies[i] = aLatencyParam;
    }
    return true;
}

}

TapeImpl::TapeImpl (const std::string& theTapeName)
{
    myFile = fopen (theTapeName.c_str(), "rb+");
    if (!myFile) {
        myFile = fopen (theTapeName.c_str(), "wb+");
        fclose (myFile);

        myFile = fopen (theTapeName.c_str(), "rb+");
    }

    myLatencies[0] = myLatencies[1] = myLatencies[2] = myLatencies[3] = 0;
}

TapeImpl::TapeImpl (const std::string& theTapeName, const std::string& theConfigFileName) : TapeImpl (theTapeName)
{
    ReadConfig (theConfigFileName, myLatencies);
}

TapeImpl::~TapeImpl()
{
    fclose (myFile);
}

bool TapeImpl::MoveToNext() const
{
#if TEST_ALGO_MODE
    auto aStartTime = std::chrono::system_clock::now();
#endif

    fseek (myFile, 0, SEEK_CUR);

    char aSymbol;
    do {
        fscanf (myFile, "%c", &aSymbol);
    } while (aSymbol != '\n' && !feof (myFile));

#if TEST_ALGO_MODE
    auto aDoneMSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - aStartTime).count();
    if (aDoneMSec < myLatencies[3]) {
        std::this_thread::sleep_for (std::chrono::milliseconds (myLatencies[3] - aDoneMSec));
    }
#endif
    return !feof (myFile);
}

bool TapeImpl::MoveToPrev() const
{
#if TEST_ALGO_MODE
    auto aStartTime = std::chrono::system_clock::now();
#endif

    fseek (myFile, 0, SEEK_CUR);

    fpos_t aFilePos;
    fgetpos (myFile, &aFilePos);

    if (!ftell (myFile)) {
        return false;
    }

    char aSymbol = '\0';
    do {
        fseek (myFile, -2, SEEK_CUR);
        fscanf (myFile, "%c", &aSymbol);

    } while (aSymbol != '\n' && ftell (myFile) != 1);

    if (ftell (myFile) == 1) {
        fseek (myFile, -1, SEEK_CUR);
    }

#if TEST_ALGO_MODE
    auto aDoneMSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - aStartTime).count();
    if (aDoneMSec < myLatencies[3]) {
        std::this_thread::sleep_for (std::chrono::milliseconds (myLatencies[3] - aDoneMSec));
    }
#endif
    return true;
}

void TapeImpl::Rewind() const
{
#if TEST_ALGO_MODE
    auto aStartTime = std::chrono::system_clock::now();
#endif

    fseek (myFile, 0, SEEK_SET);

#if TEST_ALGO_MODE
    auto aDoneMSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - aStartTime).count();
    if (aDoneMSec < myLatencies[2]) {
        std::this_thread::sleep_for (std::chrono::milliseconds (myLatencies[2] - aDoneMSec));
    }
#endif
}

int TapeImpl::Read() const
{
#if TEST_ALGO_MODE
    auto aStartTime = std::chrono::system_clock::now();
#endif

    fseek (myFile, 0, SEEK_CUR);

    fpos_t aFilePos;
    fgetpos (myFile, &aFilePos);

    int aNumber;
    fscanf (myFile, "%d", &aNumber);
    fsetpos (myFile, &aFilePos);

#if TEST_ALGO_MODE
    auto aDoneMSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - aStartTime).count();
    if (aDoneMSec < myLatencies[1]) {
        std::this_thread::sleep_for (std::chrono::milliseconds (myLatencies[1] - aDoneMSec));
    }
#endif

    return aNumber;
}

void TapeImpl::Write (int theNumber)
{
#if TEST_ALGO_MODE
    auto aStartTime = std::chrono::system_clock::now();
#endif

    fseek (myFile, 0, SEEK_CUR);

    fpos_t aFilePos;
    fgetpos (myFile, &aFilePos);

    fprintf (myFile, "%d", theNumber);

    fputc ('\n', myFile);
    fsetpos (myFile, &aFilePos);

#if TEST_ALGO_MODE
    auto aDoneMSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - aStartTime).count();
    if (aDoneMSec < myLatencies[0]) {
        std::this_thread::sleep_for (std::chrono::milliseconds (myLatencies[0] - aDoneMSec));
    }
#endif
}

bool TapeImpl::IsEnd() const
{
    fseek (myFile, 0, SEEK_CUR);

    char aValidator;
    fscanf (myFile, "%c", &aValidator);
    if (feof (myFile)) {
        return true;
    }

    fseek (myFile, -static_cast <long> (sizeof (char)), SEEK_CUR);
    return false;
}

}


Tape::Tape (const std::string& theTapeName) 
    : myImpl (std::make_shared <internal::TapeImpl> (theTapeName))
{}

Tape::Tape (const std::string& theTapeName, const std::string& theConfigFileName)
    : myImpl (std::make_shared <internal::TapeImpl> (theTapeName, theConfigFileName))
{}

bool Tape::MoveToNext() const
{
    return myImpl->MoveToNext();
}

bool Tape::MoveToPrev() const
{
    return myImpl->MoveToPrev();
}

void Tape::Rewind() const
{
    return myImpl->Rewind();
}

int Tape::Read() const
{
    return myImpl->Read();
}

void Tape::Write (int theNumber)
{
    return myImpl->Write (theNumber);
}

bool Tape::IsEnd() const
{
    return myImpl->IsEnd();
}