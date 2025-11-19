#include <query_filter_processor.h>
#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_QueryFilterProcessor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_query", "query"), &ORC_QueryFilterProcessor::set_query);
    ClassDB::bind_method(D_METHOD("get_query"), &ORC_QueryFilterProcessor::get_query);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "query", PROPERTY_HINT_RESOURCE_TYPE, "ORC_FeatureQuery"), "set_query", "get_query");
}

void ORC_QueryFilterProcessor::set_query(const Ref<ORC_FeatureQuery>& p_query) {
    query = p_query;
}

TypedArray<ORC_ProxyData> ORC_QueryFilterProcessor::process_impl(const TypedArray<ORC_ProxyData>& input) {
    TypedArray<ORC_ProxyData> output;
    
    if (!query.is_valid()) {
        ERR_PRINT("[ORC_QueryFilterProcessor WARNING] : Query is not valid, returning empty array");
        return output;
    }
    
    if (!scene_proxy.is_valid()) {
        ERR_PRINT("[ORC_QueryFilterProcessor WARNING] : Scene proxy is not valid, returning empty array");
        return output;
    }
    
    return scene_proxy->get_by_query(query);
}
