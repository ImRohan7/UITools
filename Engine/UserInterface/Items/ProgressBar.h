#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Graphics/Effects.h>
#include <Engine/Results/Results.h>
#include <Engine/Results/cResult.h>
#include "../Geometry/UIGeometry.h"
#include <iostream>

namespace eae6320 {
	namespace UI {

		class ProgressBar
		{

		public:

			cResult setPercentage(float i_f);
			float getPercentage() const { return m_Percentage; }

			Math::sVector getBgPositionToRender(); // returns pos for 

		public:
			Physics::sRigidBodyState m_Physics; // Holds physics values

			// Two objects for effect and geometry as we need
			// one for main bar and one to show bg
			// Efefcts data
			Graphics::Effects* m_Effect_Bar;
			Graphics::Effects* m_Effect_Background;
			// Geometry data
			UIGeometry* m_Geometry_Bar;
			UIGeometry* m_Geometry_Background;

			std::string m_Tag; 			// An identifier for Panel

			// Constructor
			ProgressBar(float per)
			{
				m_Tag = "default";
				m_Percentage = per;
				m_Effect_Bar = nullptr;
				m_Effect_Background = nullptr;
				m_Geometry_Bar = nullptr;
				m_Geometry_Background = nullptr;
			}

		private:
			float m_Percentage; // the filling percentage 0-100 not more than 100

		};
	}
}

