#pragma once
#include "Geometry/UIGeometry.h"
#include "Engine/Graphics/Effects.h"
#include <Engine/Results/Results.h>
#include "Engine/Graphics/ConstantBufferFormats.h"
#include "Items/Panel.h"
#include "Items/Widget.h"

namespace eae6320 {
	namespace UI {
		// the size for bucket..How many objects you want to render
		// Keep in mind progress bar takes two geometry and effect objects
		static const size_t s_size = 15; 

		// rendering pair
		struct UIRenderingData
		{

		public:

			Graphics::Effects* m_effectData;	// The Effect
			UIGeometry* m_geometryData; // my UIGeometry
			Graphics::ConstantBufferFormats::sDrawCall m_draw_call;
		};

		// then bucket object
		class UIRenderer
		{

		public:
			
			void storeDataToRender(const Widget& i_widget);
			void BindAndDraw(eae6320::Graphics::cConstantBuffer &iBf);
			void clean();

		public:
			UIRenderingData m_data[s_size];
		};
	}
}