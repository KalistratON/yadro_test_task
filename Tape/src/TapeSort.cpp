#include "Tape.h"
#include "TapeSort.h"

#include <filesystem>
#include <memory>
#include <vector>
#include <iostream>


using TapeSizeTType = std::pair <Tape, size_t>;
using TapeSizeTVectorType = std::vector <TapeSizeTType>;

size_t GetNumberCountInTempTapes (const TapeSizeTVectorType& theTapes, size_t theBufferTapeInd)
{
    size_t aNumberCount = 0;
    for (size_t i = 0; i < theTapes.size(); ++i) {
        if (i == theBufferTapeInd) {
            continue;
        }
        aNumberCount += theTapes[i].second;
    }
    return aNumberCount;
}

void MergeTapes (TapeSizeTVectorType& theTapes, size_t theBufferTapeInd, bool theIsLessComporator = true)
{
    const size_t aTapesCount = theTapes.size();
    std::vector<int> aFrontElems (aTapesCount - 1, theIsLessComporator ? INT_MIN : INT_MAX);

    const auto& SetDefualtValue = [&](size_t theIndx) {
        aFrontElems[theIndx] = theIsLessComporator ? INT_MIN : INT_MAX;
    };

    for (size_t i = 0; i < aTapesCount; ++i) {
        if (i == theBufferTapeInd) {
            continue;
        }

        size_t anElemInd = i > theBufferTapeInd ? i - 1 : i;
        if (theTapes[i].second == 0) {
            SetDefualtValue (anElemInd);
            continue;
        }

        aFrontElems[anElemInd] = theTapes[i].first.Read();
    }

    size_t aNumberCount = GetNumberCountInTempTapes (theTapes, theBufferTapeInd);

    if (aNumberCount == 0) {
        return;
    }

    size_t aLeftNumberCount = aNumberCount;
    do {
        const auto& aTargetIt = theIsLessComporator ? std::max_element (aFrontElems.begin(), aFrontElems.end())
                                                    : std::min_element (aFrontElems.begin(), aFrontElems.end());

        size_t anElemInd = static_cast <size_t> (std::distance (aFrontElems.begin(), aTargetIt));
        size_t aLastUsedTapeInd = anElemInd;
        if (aLastUsedTapeInd >= theBufferTapeInd) {
            ++aLastUsedTapeInd;
        }

        int aTargetNumber = *aTargetIt;
        theTapes[theBufferTapeInd].first.Write (aTargetNumber);
        if (aLeftNumberCount != 1) {
            theTapes[theBufferTapeInd].first.MoveToNext();
        }

        if (--theTapes[aLastUsedTapeInd].second) {
            theTapes[aLastUsedTapeInd].first.MoveToPrev();
            aFrontElems[anElemInd] = theTapes[aLastUsedTapeInd].first.Read();
        } else {
            SetDefualtValue (anElemInd);
        }

    } while (--aLeftNumberCount);

    theTapes[theBufferTapeInd].second = aNumberCount;
}

std::string GetTempTapeName (size_t theInd)
{
    return std::filesystem::temp_directory_path().string() + "temporary_tape_" + std::to_string (theInd) + ".txt";
}

TapeSizeTVectorType GetTemporaryTapes (size_t theTemporaryTapeCount)
{
    TapeSizeTVectorType aTempTapes;
    aTempTapes.reserve (theTemporaryTapeCount);
    for (size_t i = 0; i < theTemporaryTapeCount; ++i) {
        const auto& aFileName = GetTempTapeName(i);
        aTempTapes.push_back (std::make_pair<Tape, size_t> (Tape (aFileName), 0));
    }
    return aTempTapes;
}

void WriteToTape (const std::vector<int>& theNumbers, size_t theWrittenElemCount, TapeSizeTType& theTape)
{
    for (size_t i = 0; i < theWrittenElemCount; ++i) {
        theTape.first.Write (theNumbers[i]);

        if (i < theWrittenElemCount - 1) {
            theTape.first.MoveToNext();
        }
    }

    theTape.second = theWrittenElemCount;
}

size_t FillBuffer (std::vector<int>& theBuffer, const Tape& theInTape, bool theIsLessComporator)
{
    size_t aWrittenElemCount = 0;
    while (aWrittenElemCount < theBuffer.size() && !theInTape.IsOut()) {
        theBuffer[aWrittenElemCount++] = theInTape.Read();
        theInTape.MoveToNext();
    }

    theIsLessComporator ? std::sort (theBuffer.begin(), theBuffer.begin() + aWrittenElemCount)
                        : std::sort (theBuffer.begin(), theBuffer.begin() + aWrittenElemCount, std::greater<int>());
    return aWrittenElemCount;
}

void FillTemporaryTapes (const Tape& theInTape, 
    TapeSizeTVectorType& theTempTapes, 
    size_t theBufferTapeInd1, 
    size_t theBufferTapeInd2, 
    std::vector<int>& theBuffer, 
    bool theIsLessComporator = true)
{
    for (size_t i = 0; i < theTempTapes.size(); ++i) {
        if (i == theBufferTapeInd1 || i == theBufferTapeInd2) {
            continue;
        }

        size_t aCurBufferSize = FillBuffer (theBuffer, theInTape, theIsLessComporator);
        WriteToTape (theBuffer, aCurBufferSize, theTempTapes[i]);

        if (theInTape.IsOut()) {
            return;
        }
    }
}

void FillTape (const TapeSizeTType& theSortedTape, Tape& theOutTape, bool IsReverse)
{
    size_t aNumberCount = theSortedTape.second;
    while (aNumberCount--) {
        theOutTape.Write (theSortedTape.first.Read());

        if (aNumberCount) {
            theOutTape.MoveToNext();
        }
        IsReverse ? theSortedTape.first.MoveToPrev() : theSortedTape.first.MoveToNext();
    }
}

void SortTape (const Tape& theInTape, Tape& theOutTape, size_t theRAMLimit, size_t theTempTapeLimit)
{
    size_t aRAMBufferSize = static_cast <size_t> (theRAMLimit / sizeof (int));
    std::vector<int> aBuffer (aRAMBufferSize);

    size_t aBufferTapesCount = theTempTapeLimit - 1;
    TapeSizeTVectorType aTempTapes = GetTemporaryTapes (aBufferTapesCount);

    int aBufferTapeIndx = 0;
    FillTemporaryTapes (theInTape, aTempTapes, aBufferTapeIndx, aBufferTapeIndx, aBuffer, !aBufferTapeIndx);
    while (!theInTape.IsOut()) {
        MergeTapes (aTempTapes, aBufferTapeIndx, !aBufferTapeIndx);

        FillTemporaryTapes (theInTape, aTempTapes, aBufferTapeIndx, !aBufferTapeIndx, aBuffer, aBufferTapeIndx);
        aBufferTapeIndx = !aBufferTapeIndx;
    }
    MergeTapes (aTempTapes, aBufferTapeIndx, !aBufferTapeIndx);

    if (aBufferTapeIndx) {
        aTempTapes[aBufferTapeIndx].first.Rewind();
    }

    FillTape (aTempTapes[aBufferTapeIndx], theOutTape, !aBufferTapeIndx);
}