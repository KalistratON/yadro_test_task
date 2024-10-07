#pragma once

#include <cstdio>
#include <string>


namespace internal {

class TapeImpl
{
public:
    explicit TapeImpl (const std::string& theTapeName);
    TapeImpl (const std::string& theTapeName, const std::string& theConfigFileName);
    ~TapeImpl();

    bool MoveToNext() const;
    bool MoveToPrev() const;
    void Rewind() const;

    int Read() const;
    void Write (int theNumber);

    bool IsOut() const;

private:
    int myLatencies[4];
    FILE* myFile;
};

}

