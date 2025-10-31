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

	void _calcSpacing()
	{
		int used_cols = _spanH ? std::min(_elements, _grid.x)
				       : (_elements + _grid.y - 1) / _grid.y;
		int used_rows = _spanH ? (_elements + _grid.x - 1) / _grid.x
				       : std::min(_elements, _grid.y);

		_border = {(_grid.x - used_cols) / 2.0f,
			   (_grid.y - used_rows) / 2.0f};

		_spacing = glm::vec2(_shape) / glm::vec2(_grid);
	}

	int _calcLineItems(int idx) const
	{
		if (_spanH) {
			int row = idx / _grid.x;
			return std::min(_grid.x, _elements - row * _grid.x);
		} else {
			int col = idx / _grid.y;
			return std::min(_grid.y, _elements - col * _grid.y);
		}
	}

	glm::ivec2 _calcElementPosition(int idx) const
	{
		glm::ivec2 grid_loc =
		    _spanH ? glm::ivec2{idx % _grid.x, idx / _grid.x}
			   : glm::ivec2{idx / _grid.y, idx % _grid.y};

		int items_this_line = _calcLineItems(idx);
		glm::vec2 offset = _border;

		if (_spanH && (grid_loc.y == (_elements - 1) / _grid.x) &&
		    items_this_line < _grid.x) {
			offset.x += (_grid.x - items_this_line) / 2.0f;
		}

		else if (!_spanH && (grid_loc.x == (_elements - 1) / _grid.y) &&
			 items_this_line < _grid.y) {
			offset.y += (_grid.y - items_this_line) / 2.0f;
		}

		glm::vec2 center = glm::vec2(grid_loc) + offset + 0.5f;

		return _pos + glm::ivec2(center * _spacing);
	}

      public:
	~CPanel() { _controls.clear(); }

	void addControl(IControl *control) { _controls.push_back(control); }

	void addControls(std::vector<IControl *> *controls)
	{
		_controls.insert(_controls.end(), controls->begin(),
				 controls->end());
	}

	void layout(glm::ivec2 grid, glm::ivec2 pos, glm::ivec2 shape,
		    bool spanH = true)
	{
		_grid = grid;
		_pos = pos;
		_shape = shape;

		_spaces = grid.x * grid.y;
		_elements = _controls.size();

		_spanH = spanH;

		if (_elements > _spaces) {
			std::cerr << "Too many controls for grid!" << std::endl;
			exit(1);
			return;
		}

		_calcSpacing();

		for (int i = 0; i < _elements; ++i) {
			_controls[i]->setPosition(_calcElementPosition(i));
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
