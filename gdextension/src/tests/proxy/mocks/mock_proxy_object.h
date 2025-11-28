#ifndef ORCTEST_MOCKPROXYOBJECT_H
#define ORCTEST_MOCKPROXYOBJECT_H

#include <proxy_object.h>

namespace godot {

class ORCTEST_MockProxyObject_CPP : public ORC_ProxyObject {
	GDCLASS(ORCTEST_MockProxyObject_CPP, ORC_ProxyObject)

protected:
	static void _bind_methods();

public:
	static int update_count;
	static int get_update_count() { return update_count; }
	static void reset_update_count() { update_count = 0; }

	void update_impl();
};

}

#endif
