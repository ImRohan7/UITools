#pragma once

#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <External/Lua/Includes.h>

namespace eae6320
{
	namespace Assets
	{

		class ItemGeometryBuilder: public cbBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual cResult Build(const std::vector<std::string>& i_arguments) override;

			// Implementation
			//===============

			// Lua functions
			cResult LoadUIGeometryDataFromLua(const char* const i_path); // load file
			cResult LoadTableValues(lua_State& io_luaState); // load table values

			cResult LoadType(lua_State& io_luaState);
			cResult LoadDimensions(lua_State& io_luaState);
			cResult LoadPosition(lua_State& io_luaState);
			cResult LoadColors(lua_State& io_luaState);
			cResult LoadFourColorValues(lua_State& io_state,const std::string iTname, float* ar);

			

		};
	}
}

