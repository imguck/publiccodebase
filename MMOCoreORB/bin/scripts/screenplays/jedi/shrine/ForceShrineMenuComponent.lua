ForceShrineMenuComponent = {}
local ObjectManager = require("managers.object.object_manager")
local Logger = require("utils.logger")

function ForceShrineMenuComponent:fillObjectMenuResponse(pSceneObject, pMenuResponse, pPlayer)
   local menuResponse = LuaObjectMenuResponse(pMenuResponse)

	if (CreatureObject(pPlayer):hasSkill("force_title_jedi_novice")) then
		menuResponse:addRadialMenuItem(120, 3, "@jedi_trials:meditate") -- Meditate
	end

	if (CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_02")) then
		menuResponse:addRadialMenuItem(121, 3, "@force_rank:recover_jedi_items") -- Recover Jedi Items
	end
end
function ForceShrineMenuComponent:handleObjectMenuSelect(pObject, pPlayer, selectedID)
	if (pPlayer == nil or pObject == nil) then
		return 0
	end

	if (selectedID == 120 and CreatureObject(pPlayer):hasSkill("force_title_jedi_novice")) then
		if (CreatureObject(pPlayer):getPosture() ~= CROUCHED) then
			CreatureObject(pPlayer):sendSystemMessage("@jedi_trials:show_respect") -- Must respect
		else
			self:doMeditate(pObject, pPlayer)
		end
	elseif (selectedID == 121 and CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_02")) then
		self:recoverRobe(pPlayer)
	end

	return 0
end

function ForceShrineMenuComponent:doMeditate(pObject, pPlayer)
	local isJediInitiate = CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_01")
	local isJediPadawan = CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_02")
	local isJediKnight = CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_03")
	local currentTrial = JediTrials:getCurrentTrial(pPlayer)
	local knightAvailable = CreatureObject(pPlayer):villageKnightPrereqsMet("")
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (not isJediPadawan) then
		-- Unlock Padawan
		if (isJediInitiate) then
			CreatureObject(pPlayer):sendSystemMessage("You now qualify to learn the ways of the Jedi! Use /findmytrainer to locate your Jedi trainer")
			awardSkill(pPlayer, "force_title_jedi_rank_02")
            CreatureObject(pPlayer):playEffect("clienteffect/trap_electric_01.cef", "")
			CreatureObject(pPlayer):playMusicMessage("sound/music_become_jedi.snd")
			broadcastGalaxy("\\#00ff00IMPERIAL COMMUNICATION FROM THE REGIONAL GOVERNOR: Lord Vader has detected a vergence in the Force. Be on the lookout for any suspicious persons displaying unique or odd abilities. Lord Vader authorizes all citizens to use deadly force to eliminate this threat from the Empire.")
            PlayerObject(pGhost):setJediState(2)
            PlayerObject(pGhost):findJediTrainer()
            self:recoverRobe(pPlayer)

        end
    end
end


function ForceShrineMenuComponent:recoverRobe(pPlayer)
	local pInventory = SceneObject(pPlayer):getSlottedObject("inventory")
	local isJediMaster = CreatureObject(pPlayer):hasSkill("")

	if (pInventory == nil) then
		print("inventory is nil")
		return
	end

	if not (SceneObject(pInventory):isContainerFullRecursive()) then
		local pInventory = CreatureObject(pPlayer):getSlottedObject("inventory")
		if (CreatureObject(pPlayer):hasSkill("jedi_light_side_master_master")) then
			local pItem = giveItem(pInventory, "object/tangible/wearables/robe/robe_jedi_light_s01.iff", -1)
			CreatureObject(pPlayer):sendSystemMessage("@force_rank:items_recovered")
		elseif (CreatureObject(pPlayer):hasSkill("jedi_dark_side_master_master")) then
			local pItem = giveItem(pInventory, "object/tangible/wearables/robe/robe_jedi_dark_s01.iff", -1)
			CreatureObject(pPlayer):sendSystemMessage("@force_rank:items_recovered")
		else
			local pItem = giveItem(pInventory, "object/tangible/wearables/robe/robe_jedi_padawan.iff", -1)
			CreatureObject(pPlayer):sendSystemMessage("@force_rank:items_recovered")
		end
	else
		CreatureObject(pPlayer):sendSystemMessage("@jedi_spam:inventory_full_jedi_robe")
	end
end