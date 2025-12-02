#ifndef ORCTEST_PROXYFACTORYMOCK_H
#define ORCTEST_PROXYFACTORYMOCK_H

#include <proxy_factory.h>

namespace godot {

class ORCTEST_ProxyFactory_CPPMock : public ORC_ProxyFactory {
	GDCLASS(ORCTEST_ProxyFactory_CPPMock, ORC_ProxyFactory)

protected:
	static void _bind_methods();

public:
	Ref<ORC_ProxyObject> create_proxy_from_impl(Node* node) override;
	Ref<ORC_PrimaryData> create_data_from_impl(Node* node, const Ref<ORC_ProxyRegistry>& registry) override;
	bool free_proxy_impl(const Ref<ORC_ProxyObject>& proxy_object) override;
	bool free_data_impl(const Ref<ORC_ProxyData>& data, const Ref<ORC_ProxyRegistry>& registry) override;

	static void reset_all_counters();
};

}

#endif
