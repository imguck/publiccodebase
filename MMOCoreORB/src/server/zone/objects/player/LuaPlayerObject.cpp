/*
 * LuaPlayerObject.cpp
 *
 *  Created on: 13/01/2012
 *      Author: victor
 */

#include "LuaPlayerObject.h"
#include "engine/engine.h"
#include "server/zone/managers/crafting/schematicmap/SchematicMap.h"
#include "server/zone/objects/tangible/deed/eventperk/EventPerkDeed.h"
#include "server/zone/objects/tangible/eventperk/Jukebox.h"
#include "server/zone/objects/tangible/eventperk/ShuttleBeacon.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/region/CityRegion.h"

const char LuaPlayerObject::className[] = "LuaPlayerObject";

Luna<LuaPlayerObject>::RegType LuaPlayerObject::Register[] = {
		{ "_setObject", &LuaPlayerObject::_setObject },
		{ "_getObject", &LuaSceneObject::_getObject },
		{ "getFactionStanding", &LuaPlayerObject::getFactionStanding },
		{ "increaseFactionStanding", &LuaPlayerObject::increaseFactionStanding },
		{ "decreaseFactionStanding", &LuaPlayerObject::decreaseFactionStanding },
		{ "setFactionStanding", &LuaPlayerObject::setFactionStanding },
		{ "addWaypoint", &LuaPlayerObject::addWaypoint },
		{ "removeWaypoint", &LuaPlayerObject::removeWaypoint },
		{ "removeWaypointBySpecialType", &LuaPlayerObject::removeWaypointBySpecialType },
		{ "getWaypointAt", &LuaPlayerObject::getWaypointAt },
		{ "updateWaypoint", &LuaPlayerObject::updateWaypoint },
		{ "addRewardedSchematic", &LuaPlayerObject::addRewardedSchematic },
		{ "removeRewardedSchematic", &LuaPlayerObject::removeRewardedSchematic },
		{ "hasSchematic", &LuaPlayerObject::hasSchematic },
		{ "addPermissionGroup", &LuaPlayerObject::addPermissionGroup },
		{ "removePermissionGroup", &LuaPlayerObject::removePermissionGroup },
		{ "hasPermissionGroup", &LuaPlayerObject::hasPermissionGroup },
		{ "awardBadge", &LuaPlayerObject::awardBadge },
		{ "hasBadge", &LuaPlayerObject::hasBadge },
		{ "addHologrindProfession", &LuaPlayerObject::addHologrindProfession },
		{ "addHolocronPOI", &LuaPlayerObject::addHolocronPOI },
		{ "addJediTime", &LuaPlayerObject::addJediTime },
		{ "getHologrindProfessions", &LuaPlayerObject::getHologrindProfessions },
		{ "getHolocronPOI", &LuaPlayerObject::getHolocronPOI },
		{ "getJediTime", &LuaPlayerObject::getJediTime },
		{ "getForcePower", &LuaPlayerObject::getForcePower },
		{ "getForcePowerMax", &LuaPlayerObject::getForcePowerMax },
		{ "setForcePower", &LuaPlayerObject::setForcePower },
		{ "isJedi", &LuaPlayerObject::isJedi },
		{ "isJediLight", &LuaPlayerObject::isJediLight },
		{ "isJediDark", &LuaPlayerObject::isJediDark },
		{ "setJediState", &LuaPlayerObject::setJediState },
		{ "getJediState", &LuaPlayerObject::getJediState },
		{ "isOnline", &LuaPlayerObject::isOnline },
		{ "setActiveQuestsBit", &LuaPlayerObject::setActiveQuestsBit },
		{ "clearActiveQuestsBit", &LuaPlayerObject::clearActiveQuestsBit },
		{ "hasActiveQuestBitSet", &LuaPlayerObject::hasActiveQuestBitSet },
		{ "hasCompletedQuestsBitSet", &LuaPlayerObject::hasCompletedQuestsBitSet },
		{ "setCompletedQuestsBit", &LuaPlayerObject::setCompletedQuestsBit },
		{ "clearCompletedQuestsBit", &LuaPlayerObject::clearCompletedQuestsBit },
		{ "hasAbility", &LuaPlayerObject::hasAbility},
		{ "addAbility", &LuaPlayerObject::addAbility},
		{ "getExperience", &LuaPlayerObject::getExperience },
		{ "addEventPerk", &LuaPlayerObject::addEventPerk},
		{ "getEventPerkCount", &LuaPlayerObject::getEventPerkCount},
		{ "hasEventPerk", &LuaPlayerObject::hasEventPerk},
		{ "getCharacterAgeInDays", &LuaPlayerObject::getCharacterAgeInDays},
		{ "hasGodMode", &LuaPlayerObject::hasGodMode},
		{ "isPrivileged", &LuaPlayerObject::isPrivileged},
		{ "closeSuiWindowType", &LuaPlayerObject::closeSuiWindowType},
		{ "getExperienceList", &LuaPlayerObject::getExperienceList},
		{ "getExperienceCap", &LuaPlayerObject::getExperienceCap},
		{ "activateQuest", &LuaPlayerObject::activateQuest },
		{ "canActivateQuest", &LuaPlayerObject::canActivateQuest },
		{ "getSuiBox", &LuaPlayerObject::getSuiBox },
		{ "addSuiBox", &LuaPlayerObject::addSuiBox },
		{ "removeSuiBox", &LuaPlayerObject::removeSuiBox },
		{ "findJediTrainer", &LuaPlayerObject::findJediTrainer },
		{ 0, 0 }
};


LuaPlayerObject::LuaPlayerObject(lua_State *L) : LuaIntangibleObject(L) {
#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<PlayerObject*>(_getRealSceneObject());

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	realObject = reinterpret_cast<PlayerObject*>(lua_touserdata(L, 1));
#endif
}

LuaPlayerObject::~LuaPlayerObject() {
}

int LuaPlayerObject::_setObject(lua_State* L) {
	LuaIntangibleObject::_setObject(L);

#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<PlayerObject*>(_getRealSceneObject());

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	realObject = (PlayerObject*)lua_touserdata(L, -1);
#endif

	return 0;
}

int LuaPlayerObject::getFactionStanding(lua_State* L) {
	//String faction = lua_tostring(L, -1);

	const char* str = lua_tostring(L, -1);

	String faction(str);

	float standing = realObject->getFactionStanding(faction);

	lua_pushnumber(L, standing);

	return 1;
}

int LuaPlayerObject::increaseFactionStanding(lua_State* L) {
	float val = lua_tonumber(L, -1);
	const char* str = lua_tostring(L, -2);

	realObject->increaseFactionStanding(str, val);

	return 0;
}

int LuaPlayerObject::decreaseFactionStanding(lua_State* L) {
	float val = lua_tonumber(L, -1);
	const char* str = lua_tostring(L, -2);

	realObject->decreaseFactionStanding(str, val);

	return 0;
}

int LuaPlayerObject::setFactionStanding(lua_State* L) {
	float val = lua_tonumber(L, -1);
	const char* str = lua_tostring(L, -2);

	realObject->setFactionStanding(str, val);

	return 0;
}

//addWaypoint(planet, name, desc, x, y, color, active, notifyClient, specialTypeID, persistence = 1)
int LuaPlayerObject::addWaypoint(lua_State* L) {
	int numberOfArguments = lua_gettop(L) - 1;

	String planet, customName, desc;
	float x, y;
	int color, persistence = 1, specialTypeID;
	bool active, notifyClient;

	if (numberOfArguments == 9) {
		planet = lua_tostring(L, -9);
		customName = lua_tostring(L, -8);
		desc = lua_tostring(L, -7);
		x = lua_tonumber(L, -6);
		y = lua_tonumber(L, -5);
		color = lua_tointeger(L, -4);
		active = lua_toboolean(L, -3);
		notifyClient = lua_toboolean(L, -2);
		specialTypeID = lua_tointeger(L, -1);
	} else {
		planet = lua_tostring(L, -10);
		customName = lua_tostring(L, -9);
		desc = lua_tostring(L, -8);
		x = lua_tonumber(L, -7);
		y = lua_tonumber(L, -6);
		color = lua_tointeger(L, -5);
		active = lua_toboolean(L, -4);
		notifyClient = lua_toboolean(L, -3);
		specialTypeID = lua_tointeger(L, -2);
		persistence = lua_tonumber(L, -1);
	}

	ManagedReference<WaypointObject*> waypoint = realObject->getZoneServer()->createObject(0xc456e788, persistence).castTo<WaypointObject*>();

	Locker locker(waypoint);

	waypoint->setPlanetCRC(planet.hashCode());
	waypoint->setPosition(x, 0, y);
	waypoint->setSpecialTypeID(specialTypeID);
	waypoint->setCustomObjectName(customName, false);
	waypoint->setColor(color);
	waypoint->setActive(active);

	if (!desc.isEmpty())
		waypoint->setDetailedDescription(desc);

	realObject->addWaypoint(waypoint, false, notifyClient);

	lua_pushinteger(L, waypoint->getObjectID());

	return 1;
}

int LuaPlayerObject::removeWaypoint(lua_State* L) {
	unsigned long long int waypointID = lua_tointeger(L, -2);
	bool notifyClient = lua_toboolean(L, -1);

	realObject->removeWaypoint(waypointID, notifyClient);

	return 0;
}

int LuaPlayerObject::removeWaypointBySpecialType(lua_State* L) {
	int specialTypeID = lua_tointeger(L, -1);

	realObject->removeWaypointBySpecialType(specialTypeID);

	return 0;
}

int LuaPlayerObject::getWaypointAt(lua_State* L) {
	float x = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -2);
	String planet = lua_tostring(L, -1);

	WaypointObject* waypoint = realObject->getWaypointAt(x, y, planet);

	if (waypoint != NULL)
		lua_pushlightuserdata(L, waypoint);
	else
		lua_pushnil(L);

	return 1;
}

int LuaPlayerObject::updateWaypoint(lua_State* L) {
	unsigned long long int waypointID = lua_tointeger(L, -1);

	realObject->updateWaypoint(waypointID);

	return 0;
}

int LuaPlayerObject::addRewardedSchematic(lua_State* L){
	String templateString = lua_tostring(L, -4);
	short type = lua_tointeger(L, -3);
	int quantity = lua_tointeger(L, -2);
	bool notifyClient = lua_toboolean(L, -1);

	DraftSchematic* schematic = SchematicMap::instance()->get(templateString.hashCode());

	if (schematic == NULL) {
		lua_pushboolean(L, false);
		return 1;
	}

	bool result = realObject->addRewardedSchematic(schematic, type, quantity, notifyClient);

	lua_pushboolean(L, result);

	return 1;
}

int LuaPlayerObject::hasSchematic(lua_State* L) {
	String templateString = lua_tostring(L, -1);
	DraftSchematic* schematic = SchematicMap::instance()->get(templateString.hashCode());

	lua_pushboolean(L, realObject->hasSchematic(schematic));

	return 1;
}

int LuaPlayerObject::removeRewardedSchematic(lua_State* L){
	String templateString = lua_tostring(L, -2);
	bool notifyClient = lua_toboolean(L, -1);

	DraftSchematic* schematic = SchematicMap::instance()->get(templateString.hashCode());

	if (schematic != NULL)
		realObject->removeRewardedSchematic(schematic, notifyClient);

	return 0;
}

int LuaPlayerObject::addPermissionGroup(lua_State* L){
	String permissionGroup = lua_tostring(L, -2);
	bool updateBuildings = lua_toboolean(L, -1);

	realObject->addPermissionGroup(permissionGroup, updateBuildings);

	return 0;
}

int LuaPlayerObject::removePermissionGroup(lua_State* L){
	String permissionGroup = lua_tostring(L, -2);
	bool updateBuildings = lua_toboolean(L, -1);

	realObject->removePermissionGroup(permissionGroup, updateBuildings);

	return 0;
}

int LuaPlayerObject::hasPermissionGroup(lua_State* L){
	String permissionGroup = lua_tostring(L, -1);

	lua_pushboolean(L, realObject->hasPermissionGroup(permissionGroup));

	return 1;
}

int LuaPlayerObject::awardBadge(lua_State* L){
	int badgeId = lua_tointeger(L, -1);

	realObject->awardBadge(badgeId);

	return 0;
}

int LuaPlayerObject::hasBadge(lua_State* L){
	int badgeId = lua_tointeger(L, -1);

	lua_pushboolean(L, realObject->hasBadge(badgeId));

	return 1;
}

int LuaPlayerObject::addHologrindProfession(lua_State* L){
	byte profession = lua_tointeger(L, -1);

	realObject->addHologrindProfession(profession);

	return 0;
}

int LuaPlayerObject::addHolocronPOI(lua_State* L){
	int POI = lua_tonumber(L, -1);

	realObject->addHolocronPOI(POI);

	return 0;
}

int LuaPlayerObject::addJediTime(lua_State* L){
	int days = lua_tonumber(L, -1);

	realObject->addJediTime(days);

	return 0;
}

int LuaPlayerObject::getHologrindProfessions(lua_State* L) {
	Vector<byte>* professions = realObject->getHologrindProfessions();

	lua_newtable(L);

	for (int i = 0; i < professions->size(); i++) {
		lua_pushnumber(L, professions->get(i));

		lua_rawseti(L, -2, i + 1);
	}

	return 1;
}

int LuaPlayerObject::getHolocronPOI(lua_State* L) {
	lua_pushnumber(L, realObject->getHolocronPOI());

	return 1;
}

int LuaPlayerObject::getJediTime(lua_State* L) {
	lua_pushnumber(L, realObject->getJediTime());

	return 1;
}

int LuaPlayerObject::getForcePower(lua_State* L) {
	lua_pushinteger(L, realObject->getForcePower());

	return 1;
}

int LuaPlayerObject::getForcePowerMax(lua_State* L) {
	lua_pushinteger(L, realObject->getForcePowerMax());

	return 1;
}

int LuaPlayerObject::setForcePower(lua_State* L) {
	int forcePower = lua_tointeger(L, -1);

	realObject->setForcePower(forcePower, true);

	return 0;
}

int LuaPlayerObject::isJedi(lua_State* L) {
	lua_pushboolean(L, realObject->isJedi());

	return 1;
}

int LuaPlayerObject::isJediLight(lua_State* L) {
	lua_pushboolean(L, realObject->isJediLight());

	return 1;
}

int LuaPlayerObject::isJediDark(lua_State* L) {
	lua_pushboolean(L, realObject->isJediDark());

	return 1;
}

int LuaPlayerObject::setJediState(lua_State* L) {
	int jediState = lua_tointeger(L, -1);

	realObject->setJediState(jediState);

	return 0;
}
int LuaPlayerObject::getJediState(lua_State* L) {
	lua_pushinteger(L, realObject->getJediState());

	return 1;
}

int LuaPlayerObject::isOnline(lua_State* L) {
	lua_pushboolean(L, realObject->isOnline());

	return 1;
}

int LuaPlayerObject::setActiveQuestsBit(lua_State* L) {
	int quest = lua_tointeger(L, -2);
	byte value = lua_tointeger(L, -1);

	realObject->setActiveQuestsBit(quest, value, true);

	return 0;
}

int LuaPlayerObject::clearActiveQuestsBit(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	realObject->clearActiveQuestsBit(quest, true);

	return 0;
}

int LuaPlayerObject::hasActiveQuestBitSet(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	lua_pushboolean(L, realObject->hasActiveQuestBitSet(quest));

	return 1;
}

int LuaPlayerObject::hasCompletedQuestsBitSet(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	lua_pushboolean(L, realObject->hasCompletedQuestsBitSet(quest));

	return 1;
}

int LuaPlayerObject::setCompletedQuestsBit(lua_State* L) {
	int quest = lua_tointeger(L, -2);
	byte value = lua_tointeger(L, -1);

	realObject->setCompletedQuestsBit(quest, value, true);

	return 0;
}

int LuaPlayerObject::clearCompletedQuestsBit(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	realObject->clearCompletedQuestsBit(quest, true);

	return 0;
}

int LuaPlayerObject::activateQuest(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	realObject->activateQuest(quest);

	return 0;
}

int LuaPlayerObject::canActivateQuest(lua_State* L) {
	int quest = lua_tointeger(L, -1);

	lua_pushboolean(L, realObject->canActivateQuest(quest));

	return 1;
}


int LuaPlayerObject::hasAbility(lua_State* L) {
	String value = lua_tostring(L, -1);

	bool check = realObject->hasAbility(value);

	lua_pushboolean(L, check);

	return 1;

}

int LuaPlayerObject::addAbility(lua_State* L) {
	String value = lua_tostring(L, -1);

	SkillManager* skillManager = SkillManager::instance();

	if (!realObject->hasAbility(value))
		skillManager->addAbility(realObject, value);

	return 1;

}

int LuaPlayerObject::getExperience(lua_State* L) {
	String type = lua_tostring(L, -1);

	lua_pushinteger(L, realObject->getExperience(type));

	return 1;
}

int LuaPlayerObject::getEventPerkCount(lua_State* L) {
	lua_pushinteger(L, realObject->getEventPerkCount());

	return 1;
}

int LuaPlayerObject::hasEventPerk(lua_State* L) {
	String templateString = lua_tostring(L, -1);

	lua_pushboolean(L, realObject->hasEventPerk(templateString));

	return 1;
}

int LuaPlayerObject::addEventPerk(lua_State* L) {
	SceneObject* item = (SceneObject*) lua_touserdata(L, -1);

	if (item == NULL) {
		return 0;
	}

	Locker locker(item);

	ManagedReference<CreatureObject*> creature = dynamic_cast<CreatureObject*>(realObject->getParent().get().get());

	if (creature != NULL) {
		if (item->isEventPerkDeed()) {
			EventPerkDeed* deed = cast<EventPerkDeed*>(item);
			deed->setOwner(creature);
		} else if (item->isEventPerkItem()) {
			if (item->getServerObjectCRC() == 0x46BD798B) { // Jukebox
				Jukebox* jbox = cast<Jukebox*>(item);

				if (jbox != NULL)
					jbox->setOwner(creature);
			} else if (item->getServerObjectCRC() == 0x255F612C) { // Shuttle Beacon
				ShuttleBeacon* beacon = cast<ShuttleBeacon*>(item);

				if (beacon != NULL)
					beacon->setOwner(creature);
			}
		}
	}

	realObject->addEventPerk(item);

	return 0;
}

int LuaPlayerObject::getCharacterAgeInDays(lua_State* L) {
	lua_pushinteger(L, realObject->getCharacterAgeInDays());

	return 1;
}

int LuaPlayerObject::hasGodMode(lua_State* L) {
	lua_pushboolean(L, realObject->hasGodMode());

	return 1;
}

int LuaPlayerObject::isPrivileged(lua_State* L) {
	lua_pushboolean(L, realObject->isPrivileged());

	return 1;
}

int LuaPlayerObject::closeSuiWindowType(lua_State* L) {
	int type = lua_tointeger(L, -1);
	unsigned suiType = (unsigned)type;

	realObject->closeSuiWindowType( suiType );

	return 0;
}

int LuaPlayerObject::getExperienceList(lua_State* L) {
	DeltaVectorMap<String, int>* expList = realObject->getExperienceList();

	lua_newtable(L);

	for (int i = 0; i < expList->size(); i++) {
		const auto& value = expList->getKeyAt(i);

		lua_pushstring(L, value.toCharArray());
		lua_rawseti(L, -2, i + 1);
	}

	return 1;
}

int LuaPlayerObject::getExperienceCap(lua_State* L) {
	String type = lua_tostring(L, -1);
	lua_pushinteger(L, realObject->getXpCap(type));

	return 1;
}

int LuaPlayerObject::getSuiBox(lua_State* L) {
	uint32 pageId = lua_tointeger(L, -1);
	Reference<SuiBox*> object = realObject->getSuiBox(pageId);

	if (object == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushlightuserdata(L, object.get());
		object->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
	}

	return 1;
}

int LuaPlayerObject::addSuiBox(lua_State* L) {
	Reference<SuiBox*> box = (SuiBox*) lua_touserdata(L, -1);

	if (box == NULL)
		return 0;

	realObject->addSuiBox(box);

	return 0;
}

int LuaPlayerObject::removeSuiBox(lua_State* L) {
	uint32 pageId = lua_tointeger(L, -1);
	realObject->removeSuiBox(pageId, true);

	return 1;
}

int LuaPlayerObject::findJediTrainer(lua_State* L) {

	ZoneServer* zServ = realObject->getZoneServer();

	if (zServ == NULL) {
		return 0;
	}

	Vector<ManagedReference<SceneObject*> > trainers;
	Vector<String> trainerTypes;

	// Map categories defined here.
	trainerTypes.add("trainer_brawler");
	trainerTypes.add("trainer_artisan");
	trainerTypes.add("trainer_scout");
	trainerTypes.add("trainer_marksman");
	trainerTypes.add("trainer_entertainer");
	trainerTypes.add("trainer_medic");

	// Trainer number. Pick a random trainer, there are at least 600 in the galaxy.
	for (int i=0; i < zServ->getZoneCount(); ++i) {
		Zone* zone = zServ->getZone(i);
		for (int j=0; j < trainerTypes.size(); ++j) {
			SortedVector<ManagedReference<SceneObject*> > objectList = zone->getPlanetaryObjectList(trainerTypes.get(j));
			trainers.addAll(objectList);
		}
	}

	bool found = false;
	Vector3 coords;
	String zoneName = "";
	int size = trainers.size();

	if (size <= 0) {
		realObject->error("Size of trainers was less than or equal to zero in LuaPlayerObject::findJediTrainer()");
		return 0;
	}

	while (!found) {
		SceneObject* trainer = trainers.get(System::random(size - 1));

		if (trainer == NULL) {
			continue;
		}

		CreatureObject* trainerCreo = trainer->asCreatureObject();

		if (trainerCreo == NULL) {
			continue;
		}

		if (!(trainerCreo->getOptionsBitmask() & OptionBitmask::CONVERSE)) {
			continue;
		}

		ManagedReference<CityRegion*> city = trainerCreo->getCityRegion().get();

		// Make sure it's not a player-city trainer.
		if (city != NULL && !city->isClientRegion()){
			continue;
		}

		zoneName = trainerCreo->getZone()->getZoneName();
		coords = trainerCreo->getWorldPosition();
		found = true;

	}

	realObject->setTrainerCoordinates(coords);
	realObject->setTrainerZoneName(zoneName); // For the waypoint.

	return 0;
}
