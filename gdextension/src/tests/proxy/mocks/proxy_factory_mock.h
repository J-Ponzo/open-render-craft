#ifndef ORCTEST_PROXYFACTORYMOCK_H
#define ORCTEST_PROXYFACTORYMOCK_H

#include <proxy_factory.h>

namespace godot {

class ORCTEST_ProxyFactory_CPPMock : public ORC_ProxyFactory {
	GDCLASS(ORCTEST_ProxyFactory_CPPMock, ORC_ProxyFactory)

protected:
	static void _bind_methods();

public:
	Ref<ORC_ProxyObject> create_proxy_from_impl(Node* node);
	Ref<ORC_PrimaryData> create_data_from_impl(Node* node, Ref<ORC_ProxyRegistry> registry);
	bool free_proxy_impl(Ref<ORC_ProxyObject> proxy_object);
	bool free_data_impl(Ref<ORC_ProxyData> data, Ref<ORC_ProxyRegistry> registry);

	static void reset_all_counters();
};

}

#endif
