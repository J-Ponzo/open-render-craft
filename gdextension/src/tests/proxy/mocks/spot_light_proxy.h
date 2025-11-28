#ifndef ORCTEST_SPOTLIGHTPROXY_H
#define ORCTEST_SPOTLIGHTPROXY_H

#include "mock_proxy_object.h"

namespace godot {

class ORCTEST_SpotLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_SpotLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int spot_update_count;
	static int get_spot_update_count() { return spot_update_count; }
	static void reset_spot_update_count() { spot_update_count = 0; }

	void update_impl();
};

}

#endif
