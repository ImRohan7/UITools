#pragma once

#include "Panel.h"
#include "ProgressBar.h"
#include "Mask.h"
#include <vector>
#include <iostream>

namespace eae6320 {
	namespace UI {

		class Widget
		{

		public:
			std::vector<Panel*> panels;
			std::vector<ProgressBar*> pBars;
			std::vector<Mask*> masks;

		};
	}
}