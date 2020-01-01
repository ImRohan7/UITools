#include "Mask.h"


namespace eae6320 {
	namespace UI {

		void Mask::setMaskedPercentage(float num)
		{
			if (!(num > 100) && !(num < 0))
			{
				m_maskedPercentage = num;
			}
		}

		Math::sVector Mask::getMaskPositionToRender() // returns pos for 
		{
			Math::sVector toReturn = m_Physics.position;
			float nP = 200 - m_maskedPercentage;
			// chnage z only
			toReturn.z = nP;
#if defined (EAE6320_PLATFORM_D3D)
			toReturn.z =  nP;
#endif // 


			return toReturn;
		}
	}
}