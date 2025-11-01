#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include "base_classes.h"

#include <vector>

class CPanel
{
      protected:
	std::vector<IControl *> _controls;

	glm::ivec2 _grid, _pos, _shape;
	glm::vec2 _spacing, _border;

	int _spaces, _elements;
	bool _spanH;

	void _calcSpacing();

	int _calcLineItems(int idx) const;

	glm::ivec2 _calcElementPosition(int idx) const;

      public:
	~CPanel() { _controls.clear(); }

	void addControl(IControl *control) { _controls.push_back(control); }

	void addControls(std::vector<IControl *> *controls)
	{
		_controls.insert(_controls.end(), controls->begin(),
				 controls->end());
	}

	void layout(glm::ivec2 grid, glm::ivec2 pos, glm::ivec2 shape,
		    bool spanH = true);

	void processInput(Input *ip);

	void update();

	void draw(Renderer *ren, float intp);
};

#endif
