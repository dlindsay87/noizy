#ifndef __PANEL_H__
#define __PANEL_H__

#include "knob.h"
#include "spin_box.h"

#include <vector>

class Panel
{
      protected:
	std::vector<IKnob<float> *> _fKnobs;
	std::vector<IKnob<int> *> _iKnobs;
	std::vector<SpinBox *> _spinners;

      public:
	Panel() {}

	void init(glm::ivec2 pos, glm::ivec2 shape) {}

	void addKnob_f(const char *label, float value, glm::ivec2 pos,
		       DisplaySize radius, glm::vec2 rotLimits,
		       glm::vec2 valLimits, float step)
	{
		_fKnobs.push_back(new IKnob<float>(label, value, pos, radius,
						   rotLimits, valLimits, step));
	}

	void addKnob_i(const char *label, float value, glm::ivec2 pos,
		       DisplaySize radius, glm::vec2 rotLimits,
		       glm::ivec2 valLimits, float step)
	{
		_iKnobs.push_back(new IKnob<int>(label, value, pos, radius,
						 rotLimits, valLimits, step));
	}
};

#endif
