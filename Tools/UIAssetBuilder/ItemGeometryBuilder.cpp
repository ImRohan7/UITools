// Includes
//=========

#include "ItemGeometryBuilder.h"

#include "Engine/Asserts/Asserts.h"
#include <Engine/Platform/Platform.h>
#include "Engine/Graphics/Graphics.h"
#include <iostream>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <array>
#include "Tools/AssetBuildLibrary/Functions.h"
#include "Engine/Time/Time.h"
#include "Engine/Math/sVector.h"

// Helper function declaration
// ---------------------------

namespace
{
	// the vectors to receive data
	std::vector<std::array<float, 3>> _vectVertices; // pos

	// local values extracting from lua file 
	std::string _type; // the item type
	float h, w; // height width x and y
	float colorMain[4];
	float colorBG[4];

	//=======
	void createGeometryData();
}

namespace {
	void createGeometryData()
	{
		// get 4 points
		float x1, x2, y1, y2, z;
		x1 = 0.0f;
		x2 = w/100;
		y1 = 0.0f;
		y2 = h/100;
		z = 0.0f;

		// Push vertices
		std::array<float, 3> p = { x1, y1, z };
		_vectVertices.push_back(p);
		p = { x1, y2, z };
		_vectVertices.push_back(p);
		p = { x2, y1, z };
		_vectVertices.push_back(p);
		p = { x2, y2, z };
		_vectVertices.push_back(p);
		
		//std::cerr << "\nY : " << _vectVertices.at(1)[1];// vertexData[i].x;
	}
}

namespace eae6320 {

	eae6320::cResult eae6320::Assets::ItemGeometryBuilder::Build(const std::vector<std::string>& i_arguments)
	{
		auto result = eae6320::Results::Success;
		std::string p = m_path_target;
		size_t s = p.size();
		p.erase(s-4, 4);
		p.append("_bg.lua");
		std::cerr << "\nPath: " << p;
		// Crate a directory if doen't exist
		if (!(result = eae6320::Platform::CreateDirectoryIfItDoesntExist(m_path_target)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_target, "Unable to create directory!");
			return result;
		}
		else
		{
			//  read from lua file
			if (!(result = LoadUIGeometryDataFromLua(m_path_source)))
			{
				eae6320::Assets::OutputWarningMessageWithFileInfo(m_path_source, "Error loading data from file.");
				return result;
			}
			else
			{
				// create Geometry and color vetices
				createGeometryData();

				// create data
				const unsigned int verticesCount = (unsigned int)_vectVertices.size();
				eae6320::Graphics::VertexFormats::sUIObject* vertexData = new eae6320::Graphics::VertexFormats::sUIObject[verticesCount];

				for (unsigned int i = 0; i < verticesCount; i++)
				{
					std::array<float, 3> a = _vectVertices.at(i);
					vertexData[i].x = a[0];
					vertexData[i].y = a[1];
					vertexData[i].z = a[2];

					vertexData[i].r = (uint8_t)(colorMain[0] * 255.0);
					vertexData[i].g = (uint8_t)(colorMain[1] * 255.0);
					vertexData[i].b = (uint8_t)(colorMain[2] * 255.0);
					vertexData[i].a = (uint8_t)(colorMain[3] * 255.0);

				}
				//
				uint8_t* indexData = new uint8_t[6];
				indexData[0] = 0;
				indexData[1] = 2;
				indexData[2] = 1;
				indexData[3] = 1;
				indexData[4] = 3;
				indexData[5] = 2;
				
				// swap winding for direct 3d
#if defined ( EAE6320_PLATFORM_D3D )
				for (uint8_t i = 0; i < 6; i++)
				{
					if (i % 3 == 0)
					{
						std::swap(indexData[i + 1], indexData[i + 2]);
					}
				}
#endif // ( EAE6320_PLATFORM_D3D )

				// Write to binary file Based on type
				////////////////////////
				FILE* myFile;
				myFile = fopen(m_path_target, "wb");	// Open file
				
				if (_type == "mask") // set z for mask
				{
					//std::cerr << "\nMask";
					for (unsigned int i = 0; i < 2; i++)
					{
						vertexData[i].z = -1.0f;
					}
				}

				fwrite(vertexData, sizeof(Graphics::VertexFormats::sUIObject), verticesCount, myFile);
				fwrite(indexData, sizeof(*indexData), (uint8_t)6, myFile);

				// write background bar data if type is progress bar
				if (_type == "progress_bar")
				{
					std::string p = m_path_target;
					size_t s = p.size();
					p.erase(s - 4, 4);
					p.append("_bg.lua");
					FILE* mFile;
					mFile = fopen(p.data(), "wb");

					for (unsigned int i = 0; i < verticesCount; i++)
					{
						vertexData[i].r = (uint8_t)(colorBG[0] * 255.0);
						vertexData[i].g = (uint8_t)(colorBG[1] * 255.0);
						vertexData[i].b = (uint8_t)(colorBG[2] * 255.0);
						vertexData[i].a = (uint8_t)(colorBG[3] * 255.0);
					}
					for (unsigned int i = 0; i < 2; i++)
					{
						vertexData[i].z = -1.0f;
					}
					// for tilted panel
					
					// write to file
					fwrite(vertexData, sizeof(Graphics::VertexFormats::sUIObject), verticesCount, mFile);
					fwrite(indexData, sizeof(*indexData), (uint8_t)6, mFile);
				}
				//-----------------------------------------------------------

				fclose(myFile); // close

			}
			return result;
		}
	}

	

	// 3
	// Type and COLOR
	cResult Assets::ItemGeometryBuilder::LoadType(lua_State& io_luaState)
	{

		auto result = eae6320::Results::Success;
		
		// Type
		lua_pushstring(&io_luaState, "Type");
		lua_gettable(&io_luaState, -2);

		if (lua_type(&io_luaState, -1) != LUA_TSTRING) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting String but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		_type = lua_tostring(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		if (_type != "panel" && _type != "progress_bar" && _type != "mask") // check for type
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid Type found in ui item. Got : " + lua_type(&io_luaState, -1));
			return result;
		}
		//--------------------------------------------------------

		
		return result;
	}
	// 3
	cResult Assets::ItemGeometryBuilder::LoadDimensions(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// Dimensions
		lua_pushstring(&io_luaState, "Dimensions");
		lua_gettable(&io_luaState, -2);

		eae6320::cScopeGuard scopeGuard_popAssetTable([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});


		// Height
		lua_pushstring(&io_luaState, "height");
		lua_gettable(&io_luaState, -2);

		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		h = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------

		// Width
		lua_pushstring(&io_luaState, "width");
		lua_gettable(&io_luaState, -2);

		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		w = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------
		
		return result;
	}

	// Not using anyomre
	cResult Assets::ItemGeometryBuilder::LoadPosition(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// Dimensions
		lua_pushstring(&io_luaState, "position");
		lua_gettable(&io_luaState, -2);

		eae6320::cScopeGuard scopeGuard_popAssetTable([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});


		// Height
		lua_pushstring(&io_luaState, "x");
		lua_gettable(&io_luaState, -2);

		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

	//	x = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------
		
		// Width
		lua_pushstring(&io_luaState, "y");
		lua_gettable(&io_luaState, -2);

		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

	//	y = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------

		return result;
	}
	
	// 4
	// R G B A
	cResult Assets::ItemGeometryBuilder::LoadFourColorValues(lua_State& io_luaState,const std::string iTname, float* ar)
	{
		auto result = eae6320::Results::Success;
	//	char * a = iTname.data;
		// color 
		lua_pushstring(&io_luaState, iTname.data());
		lua_gettable(&io_luaState, -2);

		eae6320::cScopeGuard scopeGuard_popAssetTable([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});


		// r
		lua_pushstring(&io_luaState, "r");
		lua_gettable(&io_luaState, -2);
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		ar[0] = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		

		// g
		lua_pushstring(&io_luaState, "g");
		lua_gettable(&io_luaState, -2);
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		ar[1] = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------

		// b
		lua_pushstring(&io_luaState, "b");
		lua_gettable(&io_luaState, -2);
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		ar[2] = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------

		// a
		lua_pushstring(&io_luaState, "a");
		lua_gettable(&io_luaState, -2);
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER) // number check
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Invalid values in Ui Itrm file. Expecting number but getting :" + lua_type(&io_luaState, -1));
			return result;
		}

		ar[3] = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//--------------------------------------------------------

		return result;
	}
	// 3
	cResult Assets::ItemGeometryBuilder::LoadColors(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;
		if (!(result = LoadFourColorValues(io_luaState, "color" , colorMain)))
		{
			return result;
		}
		if (_type == "progress_bar")
		{
			if (!(result = LoadFourColorValues(io_luaState, "bg_color" ,colorBG)))
			{
				return result;
			}
		}
		return result;
	}

	// 2
	eae6320::cResult eae6320::Assets::ItemGeometryBuilder::LoadTableValues(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// load vertices

		if (!(result = LoadType(io_luaState)))
		{
			return result;
		}
		if (!(result = LoadDimensions(io_luaState)))
		{
			return result;
		}
		/*if (!(result = LoadPosition(io_luaState)))
		{
			return result;
		}*/

		if (!(result = LoadColors(io_luaState)))
		{
			return result;
		}

		return result;
	}


	// 1
	eae6320::cResult eae6320::Assets::ItemGeometryBuilder::LoadUIGeometryDataFromLua(const char* const i_path)
	{

		std::cerr << "\nPath: " << m_path_source;
		auto result = eae6320::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		eae6320::cScopeGuard scopeGuard_onExit([&luaState]
			{
				if (luaState)
				{
					// If I haven't made any mistakes
					// there shouldn't be anything on the stack
					// regardless of any errors
					EAE6320_ASSERT(lua_gettop(luaState) == 0);

					lua_close(luaState);
					luaState = nullptr;
				}
			});
		{	// Create a new state
			luaState = luaL_newstate();
			if (!luaState)
			{
				result = eae6320::Results::OutOfMemory;
				std::cerr << "Failed to create a new Lua state" << std::endl;
				return result;
			}
		}

		// Load the asset file as a "chunk",
		// meaning there will be a callable function at the top of the stack
		const auto stackTopBeforeLoad = lua_gettop(luaState);
		{
			const auto luaResult = luaL_loadfile(luaState, i_path);
			if (luaResult != LUA_OK)
			{
				result = eae6320::Results::Failure;
				std::cerr << lua_tostring(luaState, -1) << std::endl;
				// Pop the error message
				lua_pop(luaState, 1);
				return result;
			}
		}
		// Execute the "chunk", which should load the asset
		// into a table at the top of the stack
		{
			constexpr int argumentCount = 0;
			constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
			constexpr int noMessageHandler = 0;
			const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
			if (luaResult == LUA_OK)
			{
				// A well-behaved asset file will only return a single value
				const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
				if (returnedValueCount == 1)
				{
					// A correct asset file _must_ return a table
					if (!lua_istable(luaState, -1))
					{
						result = eae6320::Results::InvalidFile;
						std::cerr << "Asset files must return a table (instead of a " <<
							luaL_typename(luaState, -1) << ")" << std::endl;
						// Pop the returned non-table value
						lua_pop(luaState, 1);
						return result;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a single table (instead of " <<
						returnedValueCount << " values)" << std::endl;
					// Pop every value that was returned
					lua_pop(luaState, returnedValueCount);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << lua_tostring(luaState, -1) << std::endl;
				// Pop the error message
				lua_pop(luaState, 1);
				return result;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		eae6320::cScopeGuard scopeGuard_popAssetTable([luaState]
			{
				lua_pop(luaState, 1);
			});
		result = LoadTableValues(*luaState);
		return result;
	}
}

