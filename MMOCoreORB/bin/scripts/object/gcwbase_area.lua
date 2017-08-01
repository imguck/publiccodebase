gcwbase_area = SharedObjectTemplate:new {
	clientTemplateFileName = "",
	planetMapCategory = "",
	planetMapSubCategory = "",
	autoRegisterWithPlanetMap = 1,

	zoneComponent = "ZoneComponent",
	objectMenuComponent = "ObjectMenuComponent",
	containerComponent = "ContainerComponent",
	gameObjectType = 33554445
}
	
ObjectTemplates:addTemplate(gcwbase_area, "object/gcwbase_area.iff")
