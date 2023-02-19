#include "sliding_puzzle.h"
#include "main_data.h"
#include "game_pictures.h"
#include "game_system.h"
#include "input.h"
#include "output.h"
#include "rect.h"
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
		p.clear();
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
		return !p.empty();
	}
}

namespace SlidingPuzzle2048 {
	using namespace std;
	vector<int> id; vector<int> lv;

	int w, h, wh, w0, h0;

	Game_Pictures::ShowParams getShowParams(int pos, int level) {
		Game_Pictures::ShowParams z = {};
		z.name = "2048";
		z.fixed_to_map = true;
		z.myRect = {0,(level-1)*32,32,32};
		int i = pos / 4, j = pos % 4;
		z.position_x = i*32+16;
		z.position_y = j*32+16;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int pos) {
		Game_Pictures::MoveParams z = {};
		//z.myRect = {p[id]/h*w0,p[id]%h*h0,w0-1,h0-1};
		int i = pos / h, j = pos % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		z.duration = 1;
		return z;
	}

	void NewGame() {
		w = h = 4; w0 = h0 = 32; wh = w*h; id.resize(wh); lv.resize(wh);
		int r1 = rand() % wh, r2 = rand() % (wh-1);
		if (r2 >= r1) ++r2;
		id[r1] = 1; lv[r1] = 1;
		id[r2] = 2; lv[r2] = 1;

		int ii = 0;
		for (int i=0;i<w;++i) {
			for (int j=0;j<h;++j) {
				if (id[ii]) {
					Main_Data::game_pictures->Show(id[ii], getShowParams(ii, 1));
				}
				++ii;
			}
		}
	}

	void LeaveGame() {
		for (int i=0;i<w*h;++i) {
			if (id[i]) {
				Main_Data::game_pictures->Erase(id[i]);
			}
		}
		id.clear(); lv.clear();
		//Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));

			lcf::rpg::Sound sound;
			sound.name = "Key";
			sound.volume = 100;
			sound.tempo = 100;
			sound.balance = 50;

			Main_Data::game_system->SePlay(sound);
	}

	bool isInside(int x, int y) {
		return 0 <= x && x < w && 0 <= y && y < h;
	}

	bool canMove(int dx, int dy) {
		if (dx < 0 || dy < 0) {
			for (int x=0;x<w;++x) {
				for (int y=0;y<h;++y) {
					int i = x*h+y; if (!id[i]) continue;
					int xx = x + dx, yy = y + dy;
					if (!isInside(xx, yy)) continue;
					// Output::Debug("id: {} {} {}", xx, yy, xx*h+yy);
					if (id[xx*h+yy] == 0) return 1;
				}
			}
		} else {
			for (int x=w-1;x>=0;--x) {
				for (int y=h-1;y>=0;--y) {
					int i = x*h+y; if (!id[i]) continue;
					int xx = x + dx, yy = y + dy;
					if (!isInside(xx, yy)) continue;
					if (id[xx*h+yy] == 0) return 1;
				}
			}
		}
		return 0;
	}

	void Move(int dx, int dy) {
		if (!canMove(dx, dy)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			return;
		}

		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
		if (dx < 0 || dy < 0) {
			for (int x=0;x<w;++x) {
				for (int y=0;y<h;++y) {
					int i = x*h+y; if (!id[i]) continue;
					int xx = x + dx, yy = y + dy, ii = xx*h+yy;
					while (isInside(xx, yy) && !id[ii]) {
						swap(id[i], id[ii]); swap(lv[i], lv[ii]);
						i = ii; xx += dx; yy += dy; ii = xx*h+yy;
					}
					if (i != x*h+y) Main_Data::game_pictures->Move(id[i], getMoveParams(i));
				}
			}
		} else {
			for (int x=w-1;x>=0;--x) {
				for (int y=h-1;y>=0;--y) {
					int i = x*h+y; if (!id[i]) continue;
					int xx = x + dx, yy = y + dy, ii = xx*h+yy;
					while (isInside(xx, yy) && !id[ii]) {
						swap(id[i], id[ii]); swap(lv[i], lv[ii]);
						i = ii; xx += dx; yy += dy; ii = xx*h+yy;
					}
					if (i != x*h+y) Main_Data::game_pictures->Move(id[i], getMoveParams(i));
				}
			}
		}
	}

	void Update() {
		if (Input::IsTriggered(Input::CANCEL)) {
			LeaveGame();
			//window->FinishMessageProcessing();
			//SetPause(false);
		} else if (Input::IsTriggered(Input::LEFT)) {
			Move(-1, 0);
		} else if (Input::IsTriggered(Input::RIGHT)) {
			Move(1, 0);
		} else if (Input::IsTriggered(Input::UP)) {
			Move(0, -1);
		} else if (Input::IsTriggered(Input::DOWN)) {
			Move(0, 1);
		}
	}

	bool On() {
		return !id.empty();
	}
}
