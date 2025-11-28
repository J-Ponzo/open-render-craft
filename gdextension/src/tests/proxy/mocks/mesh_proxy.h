#ifndef ORCTEST_MESHPROXY_H
#define ORCTEST_MESHPROXY_H

#include "mock_proxy_object.h"

namespace godot {

class ORCTEST_MeshProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_MeshProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int mesh_update_count;
	static int get_mesh_update_count() { return mesh_update_count; }
	static void reset_mesh_update_count() { mesh_update_count = 0; }

	void update_impl();
};

}

#endif
