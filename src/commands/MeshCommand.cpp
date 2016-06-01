#include "MeshCommand.h"
#include <imgui\IMGUI.h>
#include <math\math.h>

void AddCubeCommand::draw() {
	if (isActive()) {
		gui::Header("Add cube");
		gui::InputVec3("Pos", &_pos);
		gui::InputVec3("Size", &_scale);
		gui::beginGroup();
		if (gui::Button("Add")) {
			_gen->add_cube(_pos, _scale);
			for (int i = 0; i < 128; ++i) {
				_gen->texture_face(i, math::buildTexture(682, 260, 32, 32));
			}
			_mesh->clear();
			_gen->build(_mesh);
			setActive(false);
		}
		if (gui::Button("Cancel")) {
			setActive(false);
		}
		gui::endGroup();
	}
}