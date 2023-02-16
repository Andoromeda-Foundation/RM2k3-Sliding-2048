#include "sliding_puzzle.h"
#include "main_data.h"
#include "game_pictures.h"
#include "game_system.h"
#include "output.h"
#include <vector>
#include <cstdlib>


namespace SlidingPuzzle {
	using namespace std;
	vector<int> p; int w, h, w0, h0, i0, p0;
	string name;

	bool ok() {
		for (int i=0;i<w*h;++i) {
			if (p[i] != i) return 0;
		}
		return 1;
	}

	Game_Pictures::ShowParams getShowParams(int id) {
		Game_Pictures::ShowParams z = {};
		z.name = name;
		z.fixed_to_map = true;
		z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = id / h, j = id % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int id) {
		Game_Pictures::MoveParams z = {};
		z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = id / h, j = id % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		return z;
	}





	void NewGame(string _name, int _w, int _h) {
		name = _name; w = _w; h = _h; w0 = 320 / w, h0 = 240 / h;
		int id = 0;
		Game_Pictures::ShowParams params = {};
		params.name = name;
		params.fixed_to_map = true;
		i0 = rand() % (w*h);

		for (int i=0;i<w;++i) {
			for (int j=0;j<h;++j) {
				p.push_back(id);
				if (id == i0) {
					p0 = id;
				} else {
					Main_Data::game_pictures->Show(id+1, getShowParams(id));
				}
				++id;
			}
		}
	}

	void Move(int d) {
		Output::Debug("Move: {} {} {}", d, p0, h);
		if (d == 3) {
			if (p0 < h) {
				// Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0-h]; swap(p[p0], p[p0-h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0-=h;
			}
		} else if (d == 1) {
			if (p0 + h >= w*h) {
				// Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0+h]; swap(p[p0], p[p0+h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0+=h;
			}
		}
	}

	bool On() {
		return w && h;
	}
}
