#ifndef ORC_PROXYCACHE_H
#define ORC_PROXYCACHE_H

#include <typeindex>
#include <vector>
#include <unordered_map>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <proxy_data.h>
#include "macros.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

// TODO : could be better if not exposed to GDScript ? 
class ORC_API ORC_ProxyCache : public RefCounted {
    GDCLASS(ORC_ProxyCache, RefCounted)

protected:
    static void _bind_methods();

public:
    std::unordered_map<std::type_index, std::vector<Ref<ORC_ProxyData>>> type_cache;
    std::unordered_map<int64_t, std::tuple<Ref<ORC_ProxyData>, int>> id_cache;

    bool register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id = -1);
    bool unregister_data(Ref<ORC_ProxyData> proxy_data);
    std::vector<Ref<ORC_ProxyData>> get_by_type(std::type_index type) const;
    Ref<ORC_ProxyData> get_by_unique_id(int64_t unique_id) const;
    bool increment_refcount(int64_t unique_id);
    bool decrement_refcount(int64_t unique_id);
};

}

#endif // ORC_PROXYCACHE_H
