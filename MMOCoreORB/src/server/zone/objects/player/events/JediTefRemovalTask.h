
#ifndef JediTEFREMOVALTASK_H_
#define JediTEFREMOVALTASK_H_

#include "server/zone/objects/player/PlayerObject.h"
#include "templates/params/creature/CreatureFlag.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
//#include "server/zone/objects/creature/CreatureObject.h"

namespace server {
namespace zone {
namespace objects {
namespace player {
namespace events {

class JediTefRemovalTask: public Task {
	ManagedWeakReference<CreatureObject*> creature;

public:
	JediTefRemovalTask(CreatureObject* creo) {
		creature = creo;
	}

	void run() {
		ManagedReference<CreatureObject*> player = creature.get();

		if (player == NULL)
			return;

		ManagedReference<PlayerObject*> ghost = player->getPlayerObject().get();

		if (ghost == NULL) {
			return;
		}

		Locker locker(player);

		if (ghost->hasJediTef()) {
			this->reschedule(llabs(ghost->getLastJediCombatActionTimestamp().miliDifference()));
		} else {
			ghost->updateInRangeBuildingPermissions();
			WeaponObject* weap = player->getWeapon();
			if (ghost->isJedi() && weap->isJediWeapon()){
				return;
			}
			if (ghost->hasPvpTef()){
				if (player->getFaction() == 0){
					ghost->schedulePvpTefRemovalTask(true);
				}
				return;
			}
			player->broadcastPvpStatusBitmask();
			}
	}
};

}
}
}
}
}

using namespace server::zone::objects::player::events;

#endif /* JediTEFREMOVALTASK_H_ */
