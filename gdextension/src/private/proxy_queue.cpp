#include <proxy_queue.h>
#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_ProxyQueue::_bind_methods() {
}

ORC_ProxyQueue::ORC_ProxyQueue() {
    pipeline.instantiate();
}

void ORC_ProxyQueue::set_parent(const Ref<ORC_ProxyQueue>& parent) {
    parent_queue = parent;
}

void ORC_ProxyQueue::execute() {
	TypedArray<ORC_ProxyData> input_data;
	
	if (parent_queue.is_valid()) {
		input_data = parent_queue->get_cached_data();
	}
	
	if (!pipeline.is_valid()) {
		ERR_FAIL_MSG("[ORC_ProxyQueue ERROR] : Pipeline is not valid");
	}
	
	pipeline->execute(input_data);
}

TypedArray<ORC_ProxyData> ORC_ProxyQueue::get_cached_data() const {
	if (!pipeline.is_valid()) {
		ERR_FAIL_V_MSG(TypedArray<ORC_ProxyData>(), "[ORC_ProxyQueue ERROR] : Pipeline is not valid");
	}
	return pipeline->get_cached_data();
}