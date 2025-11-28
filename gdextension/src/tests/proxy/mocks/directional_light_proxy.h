#ifndef ORCTEST_DIRECTIONALLIGHTPROXY_H
#define ORCTEST_DIRECTIONALLIGHTPROXY_H

#include "mock_proxy_object.h"

namespace godot {

class ORCTEST_DirectionalLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_DirectionalLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int directional_update_count;
	static int get_directional_update_count() { return directional_update_count; }
	static void reset_directional_update_count() { directional_update_count = 0; }

	void update_impl();
};

}

#endif
