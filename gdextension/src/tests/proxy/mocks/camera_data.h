#ifndef ORCTEST_CAMERADATA_H
#define ORCTEST_CAMERADATA_H

#include <primary_data.h>

namespace godot {

class ORCTEST_CameraData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_CameraData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

}

#endif
