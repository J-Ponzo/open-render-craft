#ifndef ORCTEST_OMNILIGHTPROXY_H
#define ORCTEST_OMNILIGHTPROXY_H

#include "mock_proxy_object.h"

namespace godot {

class ORCTEST_OmniLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_OmniLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int omni_update_count;
	static int get_omni_update_count() { return omni_update_count; }
	static void reset_omni_update_count() { omni_update_count = 0; }

	void update_impl();
};

}

#endif
