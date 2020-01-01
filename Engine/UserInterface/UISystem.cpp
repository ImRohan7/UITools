#include "UISystem.h"
#include "Engine/Graphics/cShader.h"
#include "Items/Widget.h"
#include "Engine/Math/cMatrix_transformation.h"

// Main hub for Drawing

namespace {
	// Draw Call
	eae6320::Graphics::cConstantBuffer s_constantBuffer_drawCall(eae6320::Graphics::ConstantBufferTypes::DrawCall);

	size_t size = eae6320::UI::s_size; // the size of the bucket object
}

namespace eae6320 {
	namespace UI {

		// Draw
		void UIRenderer::BindAndDraw(eae6320::Graphics::cConstantBuffer &cb)
		{
			
			// do for every object
			for (unsigned int i = 0; i < size; i++)
			{
				auto &data = m_data[i];
				if (data.m_effectData != NULL)
				{
					cb.Update(&data.m_draw_call);
					// bind, draw and drawcall
					data.m_effectData->bind_Effect();
					data.m_geometryData->Draw_Geometry();
				}
			}
		}

		// Clean
		void UIRenderer::clean()
		{
			// do for every object
			for (unsigned int i = 0; i < size; i++)
			{
				auto &data = m_data[i];
				if (data.m_effectData != NULL)
				{
					data.m_geometryData->DecrementReferenceCount();
					data.m_geometryData = nullptr;
					data.m_effectData->DecrementReferenceCount();
					data.m_effectData = nullptr;
				}
			}
		}

		// store data to render
		void UIRenderer::storeDataToRender(const Widget& i_widget)	// make sure to map positions (0-100)
		{
			// find panel and pbars, mask and store it
			unsigned int size = 0;
			for (auto a: i_widget.panels)
			{
				auto& data = m_data[size];
				data.m_geometryData = a->m_Geometry; // Geometry 
				data.m_geometryData->IncrementReferenceCount();

				data.m_effectData = a->m_Effect;	// Effect
				data.m_effectData->IncrementReferenceCount();

				// pos mapping
				Math::sVector pos = (a->m_Physics.position)/100;
				pos.z = 0.0;

				auto& dCall = data.m_draw_call; // Drawcall
				// Calculate localToWorld Matrix
				dCall.g_transform_localToWorld = eae6320::Math::cMatrix_transformation::CreateLocalToWorldTransform(
					a->m_Physics.orientation, pos);
				
				size++;
			}

			// mask
			for (auto a : i_widget.masks)
			{
				auto& data = m_data[size];
				data.m_geometryData = a->m_Geometry; // Geometry 
				data.m_geometryData->IncrementReferenceCount();

				data.m_effectData = a->m_Effect;	// Effect
				data.m_effectData->IncrementReferenceCount();

				// pos mapping
				Math::sVector pos;
				pos = a->getMaskPositionToRender() / 100;

				auto& dCall = data.m_draw_call; // Drawcall
				// Calculate localToWorld Matrix
				dCall.g_transform_localToWorld = eae6320::Math::cMatrix_transformation::CreateLocalToWorldTransform(
					a->m_Physics.orientation, pos);

				size++;
			}

			// Pbar
			for (auto a : i_widget.pBars)
			{
				// Bar
				auto& data = m_data[size];
				data.m_geometryData = a->m_Geometry_Bar; // Geometry  
				data.m_geometryData->IncrementReferenceCount();

				data.m_effectData = a->m_Effect_Bar;	// Effect
				data.m_effectData->IncrementReferenceCount();

				// pos mapping
				Math::sVector pos = (a->m_Physics.position) / 100;
				pos.z = 0;

				auto& dCall = data.m_draw_call; // Drawcall
				// Calculate localToWorld Matrix
				dCall.g_transform_localToWorld = eae6320::Math::cMatrix_transformation::CreateLocalToWorldTransform(
					a->m_Physics.orientation, pos);

				size++;

				// BG
				auto& mdata = m_data[size];
				mdata.m_geometryData = a->m_Geometry_Background; // Geometry  
				mdata.m_geometryData->IncrementReferenceCount();
				
				mdata.m_effectData = a->m_Effect_Background;	// Effect
				mdata.m_effectData->IncrementReferenceCount();

				auto & ndCall = mdata.m_draw_call; // Drawcall
				
				// pos mapping
				 pos = a->getBgPositionToRender() / 100;
				
										  // Calculate localToWorld Matrix
				 ndCall.g_transform_localToWorld = eae6320::Math::cMatrix_transformation::CreateLocalToWorldTransform(
					a->m_Physics.orientation, pos);

			}
			
		}


	}
}