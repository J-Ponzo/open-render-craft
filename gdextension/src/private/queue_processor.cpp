#include <queue_processor.h>
#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_QueueProcessor::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORC_QueueProcessor, process);
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_QueueProcessor, TypedArray<ORC_ProxyData>, process, const TypedArray<ORC_ProxyData>&, input)
TypedArray<ORC_ProxyData> ORC_QueueProcessor::process_impl(const TypedArray<ORC_ProxyData>& input) {
    return input;
}