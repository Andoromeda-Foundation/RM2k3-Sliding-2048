#ifndef SLIDING_PUZZLE_H
#define SLIDING_PUZZLE_H

#include "rect.h"
#include <string>
namespace SlidingPuzzle {
	void NewGame(std::string, int, int);
	void LeaveGame();
	void Move(int);
	bool On();
}

#endif
