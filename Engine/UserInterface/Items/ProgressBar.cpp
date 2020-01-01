#include "ProgressBar.h"



namespace eae6320 {
	namespace UI {
	
		Math::sVector ProgressBar::getBgPositionToRender() // returns pos for 
		{
			Math::sVector toReturn = m_Physics.position;
			
			// chnage z only
			toReturn.z = -(m_Percentage);
#if defined (EAE6320_PLATFORM_D3D)
			toReturn.z = 100 - (m_Percentage);
#endif // 


			return toReturn;
		}

		cResult ProgressBar::setPercentage(float num)
		{
			if (!(num > 100) && !(num < 0))
			{
				m_Percentage = num;
			}
			return eae6320::Results::Success;
		}
	}
}