--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{

	shaders =
	{
		{ path = "Shaders/Vertex/vertexInputLayout_3dObject.shader", arguments = { "vertex" } },
	},

	geometry = 
	{
		"Geometry/geometry1.lua",
		"Geometry/geometry2.lua",
		"Geometry/geometry3.lua",
		"Geometry/geometry4.lua",
		"Geometry/panel.lua",
		"Geometry/panelSimple.lua",
		"Geometry/panelTilted.lua",
		-- "Geometry/geometry4Big.lua",
		"Geometry/plane.lua",	   
		"Geometry/ring.lua",	   
		"Geometry/game/bui.lua",	   
		"Geometry/game/monster.lua",	   
		"Geometry/game/blood.lua",	   
	},							   

	effects =
	{
		"Effects/Effect1.lua",
		"Effects/Effect2.lua",
		"Effects/Effect3.lua",
		"Effects/UIeffect.lua",
		"Effects/UIPanelBG.lua",
		"Effects/cheffect.lua",
		"Effects/building.lua",
	},

	ui_item =
	{
		"Geometry/UI/UIPanel.lua",
		"Geometry/UI/UIProgressBar.lua",
		"Geometry/UI/UIMask.lua",
		"Geometry/UI/chVerticle.lua",
		"Geometry/UI/chHorizontal.lua",
	},

	emitters = 
	{
		"Particles/sample.emi",
	}
--	panel = 
	--{}
}
