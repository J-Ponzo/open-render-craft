#include <processor_pipeline.h>
#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_ProcessorPipeline::_bind_methods() {
}

void ORC_ProcessorPipeline::add_processor(const Ref<ORC_QueueProcessor>& processor) {
    if (!processor.is_valid()) {
        ERR_FAIL_MSG("[ORC_ProcessorPipeline ERROR] : Cannot add null processor");
    }
    processors.append(processor);
}

void ORC_ProcessorPipeline::clear_processors() {
    processors.clear();
}

void ORC_ProcessorPipeline::execute(const TypedArray<ORC_ProxyData>& input) {
    TypedArray<ORC_ProxyData> current_data = input;
    
    for (int i = 0; i < processors.size(); i++) {
        Ref<ORC_QueueProcessor> processor = processors[i];
        if (!processor.is_valid()) {
            ERR_PRINT("[ORC_ProcessorPipeline WARNING] : Skipping invalid processor at index " + String::num_int64(i));
            continue;
        }
        
        current_data = processor->process(current_data);
    }
    
    cached_result = current_data;
}
