#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include "base_classes.h"

#include <vector>

class CPanel
{
      protected:
	std::vector<IControl *> _controls;

      public:
	~CPanel() { _controls.clear(); }

	void addControl(IControl *control) { _controls.push_back(control); }

	void addControls(std::vector<IControl *> *controls)
	{
		_controls.insert(_controls.end(), controls->begin(),
				 controls->end());
	}

	void layout(glm::ivec2 grid, glm::ivec2 pos, glm::ivec2 shape,
		    bool read_hor = true)
	{
		const int size = _controls.size();
		const int grid_size = grid.x * grid.y;

		if (size > grid_size) {
			std::cerr << "Too many controls for grid!" << std::endl;
			exit(1);
			return;
		}

		int used_rows, used_cols;
		if (read_hor) {
			used_rows = (size + grid.x - 1) / grid.x;
			used_cols = std::min(size, grid.x);
		} else {
			used_cols = (size + grid.y - 1) / grid.y;
			used_rows = std::min(size, grid.y);
		}

		glm::vec2 base_offset{(grid.x - used_cols) / 2.0f,
				      (grid.y - used_rows) / 2.0f};

		glm::vec2 inner_space = {static_cast<float>(shape.x) / grid.x,
					 static_cast<float>(shape.y) / grid.y};

		for (int i = 0; i < size; ++i) {
			glm::ivec2 grid_loc;

			if (read_hor)
				grid_loc = {i % grid.x, i / grid.x};
			else
				grid_loc = {i / grid.y, i % grid.y};

			// Compute how many items are in this "row" or "column"
			int items_in_this_line;
			if (read_hor) {
				int row = grid_loc.y;
				int items_remaining = size - row * grid.x;
				items_in_this_line =
				    std::min(items_remaining, grid.x);
			} else {
				int col = grid_loc.x;
				int items_remaining = size - col * grid.y;
				items_in_this_line =
				    std::min(items_remaining, grid.y);
			}

			// Per-line offset: centers last row/column if not full
			glm::vec2 line_offset = base_offset;
			if (read_hor) {
				if (items_in_this_line < grid.x &&
				    grid_loc.y == used_rows - 1)
					line_offset.x +=
					    (grid.x - items_in_this_line) /
					    2.0f;
			} else {
				if (items_in_this_line < grid.y &&
				    grid_loc.x == used_cols - 1)
					line_offset.y +=
					    (grid.y - items_in_this_line) /
					    2.0f;
			}

			// Compute final coordinate
			glm::vec2 centered =
			    glm::vec2(grid_loc.x + line_offset.x + 0.5f,
				      grid_loc.y + line_offset.y + 0.5f);

			glm::ivec2 grid_point =
			    pos + glm::ivec2(centered * inner_space);
			_controls[i]->setPosition(grid_point);
		}

		/*const int diff = (grid.x * grid.y) - size;

		glm::ivec2 space{shape.x / (grid.x + 1),
				 shape.y / (grid.y + 1)};

		for (int i = 0; i < size; ++i) {
			glm::ivec2 grid_loc =
			    read_hor
				? glm::ivec2({i % grid.x + 1, i / grid.x + 1})
				: glm::ivec2({i / grid.y + 1, i % grid.y + 1});

			if (i >= grid_size - diff - 1) {
				// ???
			}

			glm::ivec2 grid_point = pos + grid_loc * space;
			_controls[i]->setPosition(grid_point);
		}*/
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
