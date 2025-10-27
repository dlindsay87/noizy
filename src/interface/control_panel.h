#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include "base_control.h"

#include <vector>

class CPanel
{
      protected:
	std::vector<IControl *> _controls;

      public:
	~CPanel() { _controls.clear(); }

	void addControl(IControl *control) { _controls.push_back(control); }

	void layout(glm::ivec2 grid, glm::ivec2 pos, glm::ivec2 shape)
	{
		const int size = _controls.size();

		int space_w = shape.x / (grid.x + 1);
		int space_h = shape.y / (grid.y + 1);

		if (size % (grid.x * grid.y) == 0 &&
		    (grid.x * grid.y) <= size) {
			for (int j = 0; j < grid.y; ++j) {
				for (int i = 0; i < grid.x; ++i) {

					glm::ivec2 grid_point{
					    pos.x + (i + 1) * space_w,
					    pos.y + (j + 1) * space_h};

					_controls[j * grid.x + i]->setPosition(
					    grid_point);
				}
			}
		}
	}

	void processInput(Input *ip)
	{
		for (size_t i = 0; i < _controls.size(); ++i) {
			_controls[i]->processInput(ip);
		}
	}

	void update()
	{
		for (size_t i = 0; i < _controls.size(); ++i) {
			_controls[i]->update();
		}
	}

	void draw(Renderer *ren, float intp)
	{
		for (size_t i = 0; i < _controls.size(); ++i) {
			_controls[i]->draw(ren, intp);
		}
	}
};

#endif
