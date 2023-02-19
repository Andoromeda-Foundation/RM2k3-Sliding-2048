#include "sliding_puzzle.h"
#include "main_data.h"
#include "game_pictures.h"
#include "game_system.h"
#include "input.h"
#include "output.h"
#include "rect.h"
#include <vector>
#include <set>
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

	Game_Pictures::ShowParams getShowParams(int pos, int lv) {
		Game_Pictures::ShowParams z = {};
		z.name = "2048";
		z.fixed_to_map = true;
		z.myRect = {0,(lv-1)*32,32,32};
		int i = pos / 4, j = pos % 4;
		z.position_x = i*32+16;
		z.position_y = j*32+16;
		return z;
	}

	Game_Pictures::MoveParams getMoveParams(int pos) {
		Game_Pictures::MoveParams z = {};
		int i = pos / h, j = pos % h;
		z.position_x = i*w0+w0/2;
		z.position_y = j*h0+h0/2;
		z.duration = 1;
		return z;
	}

	void NewTile() {
		set<int> s; for (int i=1;i<=wh;++i) s.insert(i);
		vector<int> t; for (int i=0;i<wh;++i) if (!id[i]) t.push_back(i); else s.erase(id[i]);
		if (t.empty()) return;
		int r = rand() % t.size();
		int i = t[r];
		id[i] = *s.begin();
		lv[i] = 1;
		Main_Data::game_pictures->Show(id[i], getShowParams(i, lv[i]));
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
					Main_Data::game_pictures->Show(id[ii], getShowParams(ii, lv[ii]));
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

	void Move(int dx, int dy) {
		bool ok = false;
		int x0 = 0, x1 = w, xd = 1, y0 = 0, y1 = h, yd = 1;
		if (dx < 0) x0 = w-1, x1 = -1, xd = -1;
		if (dy < 0) y0 = h-1, y1 = -1, yd = -1;

		for (int x=x0;x!=x1;x+=xd) {
			for (int y=y0;y!=y1;y+=yd) {
				int i = x*h+y; if (!id[i]) continue;
				int i0 = i, lv0 = lv[i];
				int xx = x + dx, yy = y + dy, ii = xx*h+yy;
				while (isInside(xx, yy) && (!id[ii] || lv[ii] == lv[i])) {
					if (lv[i] == lv[ii]) {
						Main_Data::game_pictures->Erase(id[ii]);
						id[ii] = id[i]; lv[ii] += 1; id[i] = lv[i] = 0;
						i = ii; break;
					} else {
						swap(id[i], id[ii]); swap(lv[i], lv[ii]);
						i = ii; xx += dx; yy += dy; ii = xx*h+yy;
					}
				}
				if (i != i0) {
					if (lv[i] != lv0) Main_Data::game_pictures->Show(id[i], getShowParams(i0, lv[i]));
					Main_Data::game_pictures->Move(id[i], getMoveParams(i));
					ok = true;
				}
			}
		}

		if (ok) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			NewTile();
		} else {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
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
