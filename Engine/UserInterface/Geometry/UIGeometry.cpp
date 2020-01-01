// Includes
//---------------------------

#include "UIGeometry.h"
#include <iostream>
#include <Engine/Platform/Platform.h>
#include <array>

#include <Engine/Logging/Logging.h>
#include "Engine/Time/time.h"

// static data initialization

eae6320::Assets::cManager<eae6320::UI::UIGeometry> eae6320::UI::UIGeometry::s_managerG;


// Factory fun
eae6320::cResult eae6320::UI::UIGeometry::CreateUIGeometryObject(UIGeometry*& i_outGeoData)
{
	auto result = Results::Success;
	UIGeometry* newData = nullptr;

	newData = new UIGeometry();
	i_outGeoData = newData;
	if (!newData)
	{
		result = Results::OutOfMemory;
		EAE6320_ASSERTF(false, "Unable to allocate memory for Geometry Data! ");
		Logging::OutputError("Failed to allocate memory for Geometry Object!");
		return result;
	}
	return result;
}

// load and initialize geometry from lua files here??? Yes
eae6320::cResult eae6320::UI::UIGeometry::Load(const std::string& i_path, UIGeometry*& o_geometry, UI::ItemType i_type)
{
	auto result = Results::Success;
	
	if (!(result = CreateUIGeometryObject(o_geometry)))
	{
		EAE6320_ASSERTF(false, "Can't provide memory for geometry data.");
		return result;
	}
	else
	{	// Extract data from a binary file and initialize geometry

		Platform::sDataFromFile dataFromFile;

		// Load the binary data
		{
			std::string errorTxt;
			if (!(result = Platform::LoadBinaryFile(i_path.data() , dataFromFile, &errorTxt)))
			{
				EAE6320_ASSERTF(false, errorTxt.c_str());
				Logging::OutputError("Failed to load Geometry Data(Binary) from file %s: %s", i_path.c_str(), errorTxt.c_str());
				return result;
			}

			auto offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
			const auto finalOffset = offset + dataFromFile.size;


			// Load by checking type
			//==================================
			 Graphics::VertexFormats::sUIObject* _vertexArray = new Graphics::VertexFormats::sUIObject();
			 uint8_t* _indexArray = new uint8_t();
			
			switch (i_type)
			{

			case eae6320::UI::Undefined:
				EAE6320_ASSERTF(false, "Undefined item type passed");
				result = eae6320::Results::Failure;
				Logging::OutputError("Failed to load UI Geometry Data(Binary). Undefined ItemTypes found while loading UIgeometry");
				return result;
				break;
			case eae6320::UI::sPanel:
			case eae6320::UI::sProgressBar:
			case eae6320::UI::sProgressbar_background:
			case eae6320::UI::sMask:

				// Vertex Array
				 _vertexArray = reinterpret_cast<Graphics::VertexFormats::sUIObject*>(offset);
				offset += (sizeof(Graphics::VertexFormats::sUIObject) * 4);

				// Index Array
				_indexArray = reinterpret_cast<uint8_t*>(offset);
			
				break;
			
			default:
				EAE6320_ASSERTF(false, "Undefined item type passed");
				result = eae6320::Results::Failure;
				Logging::OutputError("Failed to load UI Geometry Data(Binary). Undefined ItemTypes found while loading UIgeometry");
				return result;
				break;
			}
			

			if (!(result = o_geometry->InitializeGeometryData(_vertexArray, _indexArray, 6, 4)))
			{
				EAE6320_ASSERTF(false, "Error Initializing geometry.");
				return result;
			}
		}

	}

	return result;
}

