#ifndef SLIDING_PUZZLE_H
#define SLIDING_PUZZLE_H

#include <string>

namespace SlidingPuzzle {
	void NewGame(std::string, int, int);
	void LeaveGame();
	void Move(int);
	bool On();
}

namespace SlidingPuzzle2048 {
	void NewGame();
	void LeaveGame();
	void Update();
	void Move(int);
	bool On();
}

namespace TowerOfHanoi {
	void NewGame();
	void LeaveGame();
	void Update();
	void Pop();
	void Move();
	void Push();
	bool On();
}
#endif
