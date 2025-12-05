#ifndef ORC_PSO_FACTORY_H
#define ORC_PSO_FACTORY_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <unordered_map>
#include <macros.h>
#include <pso.h>
#include <proxy_data.h>
#include <rd_helper.h>

namespace godot {

class ORC_PSOFactory : public RefCounted {
    GDCLASS(ORC_PSOFactory, RefCounted)

protected:
    static void _bind_methods();

private:
    std::unordered_map<uint64_t, int64_t> mask_lookup; 
    std::unordered_map<int64_t, Ref<ORC_PSO>> pso_lookup;

public:
    ORC_PSOFactory();
    ~ORC_PSOFactory();

    Ref<ORC_PSO> get_or_create_pso_from_object(const Ref<ORC_ProxyData>& proxy_data);
    DECLARE_GD_OVERRIDABLE_METHOD(Ref<ORC_PSO>, create_pso_from_data, const Ref<ORC_ProxyData>&)
    void cleanup();
};

}

#endif
