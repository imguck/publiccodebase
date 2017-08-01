#include "server/zone/objects/area/GCWBaseActiveArea.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/ZoneClientSession.h"

void GCWBaseActiveAreaImplementation::init() {
	//ManagedReference<BuildingObject*> building = cast<BuildingObject*>(this->getParent(SceneObjectType::BUILDING).get());
	ManagedReference<SceneObject*> object = this->getRootParent();
	BuildingObject* building = cast<BuildingObject*>( object.get());
	info("Active Area has been created for the GCW Base", true);

}

void GCWBaseActiveAreaImplementation::notifyEnter(SceneObject* object) {
	if (object == NULL || !object->isPlayerCreature())
		return;

	CreatureObject* player = cast<CreatureObject*>(object);
	PlayerObject* ghost = player->getPlayerObject().get();
	player->sendSystemMessage("You have entered the Active Area for GCW Base");

	if (player == NULL || building == NULL )
		return;

	if (getRadius() < 100) {
		player->sendSystemMessage("You have been TEFed by the enemy base!");
		if (ghost != NULL){
			if (player->getFaction() != 0 && player->getFaction() != building->getFaction())
				ghost->updateLastPvpCombatActionTimestamp();
		}
	} 	
}
