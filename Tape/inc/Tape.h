#pragma once

#include "Macro.h"

#include <memory>
#include <string>


namespace internal {
class TapeImpl;
}

class Tape {

public:
    using ImplType = std::shared_ptr<internal::TapeImpl>;

    _EXPORT Tape (const std::string& theTapeName);
    _EXPORT Tape (const std::string& theTapeName, const std::string& theConfigFileName);

    _EXPORT bool MoveToNext() const;
    _EXPORT bool MoveToPrev() const;
    _EXPORT void Rewind() const;

    _EXPORT int Read() const;
    _EXPORT void Write (int theNumber);

    _EXPORT bool IsOut() const;

private:
    ImplType myImpl;
};