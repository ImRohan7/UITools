#pragma once#pragma once

#include <Engine/Assets/ReferenceCountedAssets.h>

#include "Engine/Graphics/cConstantBuffer.h"
#include "Engine/Graphics/cVertexFormat.h"
#include "Engine/Graphics/VertexFormats.h"
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>

namespace eae6320
{
	namespace UI {
		
		enum ItemType :uint8_t
		{
			Undefined = 0,

			sPanel = 1 << 0, // simple panel

			sProgressBar = 1 << 1, // progress bar

			sProgressbar_background = 1 << 2, // progress bar bg

			sMask = 1 << 3

		};

		struct UIGeometry {
			
		public:

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(UIGeometry);

			
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();


		public:


#if defined( EAE6320_PLATFORM_D3D )
			// Direct3D

			Graphics::cVertexFormat::Handle m_vertexFormat;
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			ID3D11Buffer* m_indexBuffer = nullptr;

#elif defined( EAE6320_PLATFORM_GL )
			// OpenGL
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			GLuint m_indexBufferId = 0;
#endif
			// the number of indices that are passed in index buffer
			unsigned int m_indexCount;

			// Assets
			//--------
			using Handle = Assets::cHandle<UIGeometry>;
			static Assets::cManager<UIGeometry> s_managerG;


			///////////// 
			// FUNCTIONS
			// =========================

			// Handle create
			static cResult Load(const std::string& i_path, UIGeometry*& o_geometry, ItemType i_type);

			// Factory Create
			static cResult CreateUIGeometryObject(UIGeometry*& i_outvar);

			// Factory Delete
			static void Release_Object(UIGeometry*& i_obj)
			{
				if (i_obj)
				{
					i_obj->DecrementReferenceCount();
					i_obj = nullptr;
				}
				else
				{
					EAE6320_ASSERTF(false, "Unable to delete the Geometry Object! Passed object is null !")
				}
			}

			// Initialize
			eae6320::cResult InitializeGeometryData(Graphics::VertexFormats::sUIObject i_vertexData[], 
				uint8_t i_indexData[], unsigned int i_indexCount, unsigned int i_vertexCount);

			// Draw
			void Draw_Geometry();

			// Clean
			eae6320::cResult Clean_GeometryData(); // pf specific

			EAE6320_ASSETS_DECLAREREFERENCECOUNT();


		private:

			// Constructor
			UIGeometry() : m_indexCount(0)
			{

			}

			// Destructor
			~UIGeometry()
			{
				EAE6320_ASSERT(m_referenceCount == 0);
				const auto result = Clean_GeometryData();
				EAE6320_ASSERT(result);
			}

		};

	}
}





