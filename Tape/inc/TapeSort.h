#pragma once

#include "Macro.h"


class Tape;

_EXPORT void SortTape (const Tape& theInTape, Tape& theOutTape, size_t theRAMLimit, size_t theTempTapeLimit);