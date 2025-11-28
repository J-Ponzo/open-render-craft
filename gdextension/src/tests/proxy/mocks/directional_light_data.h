#ifndef ORCTEST_DIRECTIONALLIGHTDATA_H
#define ORCTEST_DIRECTIONALLIGHTDATA_H

#include <primary_data.h>

namespace godot {

class ORCTEST_DirectionalLightData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_DirectionalLightData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

}

#endif
