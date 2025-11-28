#ifndef ORCTEST_TOPOLOGYDATA_H
#define ORCTEST_TOPOLOGYDATA_H

#include <godot_cpp/classes/mesh.hpp>
#include <secondary_data.h>

namespace godot {

class ORCTEST_TopologyData_CPP : public ORC_SecondaryData {
	GDCLASS(ORCTEST_TopologyData_CPP, ORC_SecondaryData)

protected:
	static void _bind_methods();

public:
	Ref<Mesh> mesh;
	Ref<Mesh> get_mesh() const { return mesh; }
	void set_mesh(const Ref<Mesh> &mesh) { this->mesh = mesh; }
};

}

#endif
