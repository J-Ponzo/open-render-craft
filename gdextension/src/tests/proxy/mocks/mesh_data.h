#ifndef ORCTEST_MESHDATA_H
#define ORCTEST_MESHDATA_H

#include <primary_data.h>
#include <secondary_data.h>

namespace godot {

class ORCTEST_MeshData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_MeshData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods();

public:
	Ref<ORC_SecondaryData> topology_data;
	Ref<ORC_SecondaryData> get_topology_data() const { return topology_data; }
	void set_topology_data(const Ref<ORC_SecondaryData> &topology_data) { this->topology_data = topology_data; }
};

}

#endif
