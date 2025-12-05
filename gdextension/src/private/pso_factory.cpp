#include <pso_factory.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

static const char* ERR_INVALID_PROXY_DATA = "[ORC] Invalid proxy_data reference";
static const char* ERR_CREATE_PSO_NOT_IMPLEMENTED = "[ORC] ORC_PSOFactory::create_pso_from_data_impl() not implemented";

ORC_PSOFactory::ORC_PSOFactory() {
}

ORC_PSOFactory::~ORC_PSOFactory() {
}

void ORC_PSOFactory::_bind_methods() {
    ClassDB::bind_method(D_METHOD("cleanup"), &ORC_PSOFactory::cleanup);
    ClassDB::bind_method(D_METHOD("get_or_create_pso_from_object", "proxy_data"), &ORC_PSOFactory::get_or_create_pso_from_object);
    BIND_GD_OVERRIDABLE_METHOD(ORC_PSOFactory, create_pso_from_data)
}

Ref<ORC_PSO> ORC_PSOFactory::get_or_create_pso_from_object(const Ref<ORC_ProxyData>& proxy_data) {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(Ref<ORC_PSO>(), ERR_INVALID_PROXY_DATA);
    
    uint64_t instance_id = proxy_data->get_instance_id();
    
    auto it = mask_lookup.find(instance_id);
    if (it == mask_lookup.end()) {
        Ref<ORC_PSO> pso = create_pso_from_data(proxy_data);
        if (pso.is_valid()) {
            int64_t mask = instance_id;
            mask_lookup[instance_id] = mask;
            pso_lookup[mask] = pso;
        }
    }
    
    int64_t mask = mask_lookup[instance_id];
    return pso_lookup[mask];
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_PSOFactory, Ref<ORC_PSO>, create_pso_from_data, const Ref<ORC_ProxyData>&, proxy_data)
Ref<ORC_PSO> ORC_PSOFactory::create_pso_from_data_impl(const Ref<ORC_ProxyData>& proxy_data) {
    ERR_FAIL_V_MSG(Ref<ORC_PSO>(), ERR_CREATE_PSO_NOT_IMPLEMENTED);
}

void ORC_PSOFactory::cleanup() {
    for (auto& pair : pso_lookup) {
        Ref<ORC_PSO>& pso = pair.second;
        if (pso.is_valid()) {
            ORC_RDHelper::get_rd()->free_rid(pso->pipeline);
            ORC_RDHelper::get_rd()->free_rid(pso->shader_program);
        }
    }
    pso_lookup.clear();
    mask_lookup.clear();
}
