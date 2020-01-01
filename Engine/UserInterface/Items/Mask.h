#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Graphics/Effects.h>
#include "../Geometry/UIGeometry.h"

namespace eae6320 {
	namespace UI {

		class Mask
		{
		public:

			// Holds physics values
			Physics::sRigidBodyState m_Physics;

			// Efefcts data
			Graphics::Effects* m_Effect;
			// Geometry data
			UIGeometry* m_Geometry;

			// An identifier for Panel
			std::string m_Tag;

			// user
			void setMaskedPercentage(float num);
			float getPercentage() const { return m_maskedPercentage; }


			// engine
			Math::sVector getMaskPositionToRender();

		public:
			Mask(float per) : m_Tag("test"), m_maskedPercentage(per),m_Effect(nullptr), m_Geometry(nullptr)
			{}

		private:
			float m_maskedPercentage;
		};
	}
}