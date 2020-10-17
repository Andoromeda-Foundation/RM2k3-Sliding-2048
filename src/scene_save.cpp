/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <sstream>

#ifdef EMSCRIPTEN
#  include <emscripten.h>
#endif

#include <lcf/data.h>
#include "dynrpg.h"
#include "filefinder.h"
#include "game_actor.h"
#include "game_map.h"
#include "game_party.h"
#include "game_switches.h"
#include "game_variables.h"
#include "game_party.h"
#include "game_actors.h"
#include "game_system.h"
#include "game_targets.h"
#include "game_screen.h"
#include "game_pictures.h"
#include <lcf/lsd/reader.h>
#include "output.h"
#include "player.h"
#include "scene_save.h"
#include "version.h"

Scene_Save::Scene_Save() :
	Scene_File(ToString(lcf::Data::terms.save_game_message)) {
	Scene::type = Scene::Save;
}

void Scene_Save::Start() {
	Scene_File::Start();

	for (int i = 0; i < 15; i++) {
		file_windows[i]->SetHasSave(true);
		file_windows[i]->Refresh();
	}
}

void Scene_Save::Action(int index) {
	Save(*tree, index + 1);

	Scene::Pop();
}

std::string Scene_Save::GetSaveFilename(const FileFinder::DirectoryTree& tree, int slot_id) {
	const auto save_file = fmt::format("Save{:02d}.lsd", slot_id);

	Output::Debug("Saving to {}", save_file);

	std::string filename = FileFinder::FindDefault(tree, save_file);

	if (filename.empty()) {
		filename = FileFinder::MakePath(tree.directory_path, save_file);
	}
	return filename;
}

void Scene_Save::Save(const FileFinder::DirectoryTree& tree, int slot_id, bool prepare_save) {
	const auto filename = GetSaveFilename(tree, slot_id);
	auto save_stream = FileFinder::OpenOutputStream(filename);
	Save(save_stream, slot_id, prepare_save);
}

void Scene_Save::Save(std::ostream& os, int slot_id, bool prepare_save) {

	lcf::rpg::Save save;
	auto& title = save.title;
	// TODO: Maybe find a better place to setup the save file?

	int size = (int)Main_Data::game_party->GetActors().size();
	Game_Actor* actor;

	if (size > 3) {
		actor = Main_Data::game_party->GetActors()[3];
		title.face4_id = actor->GetFaceIndex();
		title.face4_name = ToString(actor->GetFaceName());
	}
	if (size > 2) {
		actor = Main_Data::game_party->GetActors()[2];
		title.face3_id = actor->GetFaceIndex();
		title.face3_name = ToString(actor->GetFaceName());
	}
	if (size > 1) {
		actor = Main_Data::game_party->GetActors()[1];
		title.face2_id = actor->GetFaceIndex();
		title.face2_name = ToString(actor->GetFaceName());
	}
	if (size > 0) {
		actor = Main_Data::game_party->GetActors()[0];
		title.face1_id = actor->GetFaceIndex();
		title.face1_name = ToString(actor->GetFaceName());
		title.hero_hp = actor->GetHp();
		title.hero_level = actor->GetLevel();
		title.hero_name = ToString(actor->GetName());
	}

	Main_Data::game_system->SetSaveSlot(slot_id);
	save.party_location = Main_Data::game_player->GetSaveData();
	Game_Map::PrepareSave(save);

	if (prepare_save) {
		lcf::LSD_Reader::PrepareSave(save, PLAYER_SAVEGAME_VERSION);
	}

	save.targets = Main_Data::game_targets->GetSaveData();
	save.system = Main_Data::game_system->GetSaveData();
	save.system.switches = Main_Data::game_switches->GetData();
	save.system.variables = Main_Data::game_variables->GetData();
	save.inventory = Main_Data::game_party->GetSaveData();
	save.actors = Main_Data::game_actors->GetSaveData();

	save.screen = Main_Data::game_screen->GetSaveData();
	save.pictures = Main_Data::game_pictures->GetSaveData();

	save.system.scene = Scene::instance ? Scene::rpgRtSceneFromSceneType(Scene::instance->type) : -1;

	// 2k RPG_RT always stores SaveMapEvent with map_id == 0.
	if (Player::IsRPG2k()) {
		for (auto& sme: save.map_info.events) {
			sme.map_id = 0;
		}
	}
	lcf::LSD_Reader::Save(os, save, Player::IsRPG2k3(), Player::encoding);

	DynRpg::Save(slot_id);

#ifdef EMSCRIPTEN
	// Save changed file system
	EM_ASM({
		FS.syncfs(function(err) {
		});
	});
#endif

}

bool Scene_Save::IsSlotValid(int) {
	return true;
}
