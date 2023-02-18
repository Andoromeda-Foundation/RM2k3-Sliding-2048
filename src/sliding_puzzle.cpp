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

	Game_Pictures::ShowParams getShowParams(int id, int pos) {
		Game_Pictures::ShowParams z = {};
		z.name = name;
		z.fixed_to_map = true;
		z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = pos / h, j = pos % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int id) {
		Game_Pictures::MoveParams z = {};
		//z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = id / h, j = id % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		z.duration = 1;
		return z;
	}

	void NewGame(string _name, int _w, int _h) {
		name = _name; w = _w; h = _h; w0 = 320 / w, h0 = 240 / h;
		int id = 0;
		Game_Pictures::ShowParams params = {};
		params.name = name;
		params.fixed_to_map = true;
		i0 = rand() % (w*h);

		for (int i=0;i<w;++i)
			for (int j=0;j<h;++j)
				p.push_back(id++);
		std::random_shuffle(p.begin(), p.end());

		id = 0;
		for (int i=0;i<w;++i) {
			for (int j=0;j<h;++j) {
				if (p[id] == i0) {
					p0 = id;
				} else {
					Main_Data::game_pictures->Show(p[id]+1, getShowParams(p[id], id));
				}
				++id;
			}
		}
	}

	void LeaveGame() {
		for (int i=0;i<w*h;++i) {
			Main_Data::game_pictures->Erase(i+1);
		}
		w = 0; h = 0; p.clear();
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
	}

	void Move(int d) {
		if (d == 0) {
			if ((p0+1) % h == 0) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0+1]; swap(p[p0], p[p0+1]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0+=1;
			}
		} else if (d == 2) {
			if (p0 % h == 0) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0-1]; swap(p[p0], p[p0-1]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0-=1;
			}
		} else if (d == 1) {
			if (p0 < h) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0-h]; swap(p[p0], p[p0-h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0-=h;
			}
		} else if (d == 3) {
			if (p0 + h >= w*h) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			} else {
				int t = p[p0+h]; swap(p[p0], p[p0+h]);
				Main_Data::game_pictures->Move(t+1, getMoveParams(p0));
				p0+=h;
			}
		}
		if (ok()) {
			lcf::rpg::Sound sound;
			sound.name = "Key";
			sound.volume = 100;
			sound.tempo = 100;
			sound.balance = 50;

			Main_Data::game_system->SePlay(sound);
			//Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		}
	}

	bool On() {
		return w && h;
	}
}
