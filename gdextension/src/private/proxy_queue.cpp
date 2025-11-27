#include <proxy_queue.h>
#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_ProxyQueue::_bind_methods() {
}

ORC_ProxyQueue::ORC_ProxyQueue() {
}

void ORC_ProxyQueue::set_scene_proxy(ORC_SceneProxyBase* scene_proxy) {
    this->scene_proxy = scene_proxy;
}

void ORC_ProxyQueue::add_processor(const Ref<ORC_QueueProcessor>& processor) {
    if (!processor.is_valid()) {
        ERR_FAIL_MSG("[ORC_ProxyQueue ERROR] : Cannot add null processor");
    }
    processors.append(processor);
}

void ORC_ProxyQueue::clear_processors() {
    processors.clear();
}

void ORC_ProxyQueue::execute() {
	TypedArray<ORC_ProxyData> current_data = scene_proxy->get_by_query(init_query);
    
    for (int i = 0; i < processors.size(); i++) {
        Ref<ORC_QueueProcessor> processor = processors[i];
        if (!processor.is_valid()) {
            ERR_PRINT("[ORC_ProxyQueue WARNING] : Skipping invalid processor at index " + String::num_int64(i));
            continue;
        }
        
        current_data = processor->process(current_data);
    }
    
    cached_result = current_data;
}