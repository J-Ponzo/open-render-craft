#ifndef ORC_PROXYREGISTRY_H
#define ORC_PROXYREGISTRY_H

#include <typeindex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <variant>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <proxy_data.h>
#include <data_query.h>
#include <proxy_registry_dump.h>
#include "macros.h"

namespace godot {

class ORC_ProxyRegistry : public RefCounted {
    GDCLASS(ORC_ProxyRegistry, RefCounted)

    friend class ORC_SceneProxyBase;
    friend class ORC_ProxyData;

private:
    static std::unordered_map<StringName, std::type_index>& cpp_types();
    std::unordered_map<StringName, uint64_t> flag_mask_lookup;
    uint8_t next_available_bit = 0;

    std::unordered_map<int64_t, std::tuple<Ref<ORC_ProxyData>, int>> id_lookup;
    std::vector<Ref<ORC_ProxyData>> data_pool;
    std::unordered_map<Ref<ORC_ProxyData>, uint64_t, ProxyDataHash> data_flags;
    std::unordered_map<Ref<ORC_DataQuery>, std::vector<Ref<ORC_ProxyData>>, DataQueryHash> query_cache;
    std::unordered_map<Ref<ORC_ProxyData>, std::vector<Ref<ORC_ProxyData>>, ProxyDataHash> cascade_sources;
    std::unordered_map<Ref<ORC_ProxyData>, std::vector<Ref<ORC_ProxyData>>, ProxyDataHash> cascade_targets;
    
    uint64_t get_or_create_flag_mask(const StringName& flag_name);
    void propagate_flag_to_targets(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value);
    void unregister_cascade_relations(const Ref<ORC_ProxyData>& proxy_data);
    bool matches_query(const Ref<ORC_ProxyData>& proxy_data, uint64_t flags, const Ref<ORC_DataQuery>& query) const;
    bool update_query_cache_for_data(const Ref<ORC_ProxyData>& proxy_data, uint64_t old_flags, uint64_t new_flags);
    bool remove_from_query_cache(const Ref<ORC_ProxyData>& proxy_data);
    bool add_query_to_cache(const Ref<ORC_DataQuery>& query);
    bool fill_query_features(const Ref<ORC_DataQuery>& query, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values);

    bool set_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value, bool is_propagation);
    bool has_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name);
    void register_flag_sources_internal(ORC_ProxyData* proxy_data, const TypedArray<ORC_ProxyData>& sources);
    void unregister_flag_sources_internal(ORC_ProxyData* proxy_data, const TypedArray<ORC_ProxyData>& sources);
    TypedArray<ORC_ProxyData> get_by_query_internal(const Ref<ORC_DataQuery>& query);
    Ref<ORC_DataQuery> create_query_internal(const TypeKey& type_key, const TypedArray<StringName>& flag_names = TypedArray<StringName>(), const TypedArray<bool>& flag_values = TypedArray<bool>());

protected:
    static void _bind_methods();

public:
    template<typename T>
    static void register_cpp_type(const StringName& class_name) {
        cpp_types().emplace(class_name, typeid(T));
    }
    static std::type_index get_cpp_type_index(const StringName& class_name);
    
    bool register_data(const Ref<ORC_ProxyData>& proxy_data, int64_t unique_id = -1);
    bool unregister_data(const Ref<ORC_ProxyData>& proxy_data);
    Ref<ORC_ProxyData> get_by_unique_id(int64_t unique_id) const;
    bool increment_refcount(int64_t unique_id);
    bool decrement_refcount(int64_t unique_id);
    void clear();
    Ref<ORC_ProxyRegistryDump> dump_registry() const;
    
    int64_t get_flags_mask_internal(ORC_ProxyData* proxy_data) const;

#ifdef DEBUG_ENABLED
private:
    std::unordered_map<StringName, TypeKey> flag_type_lookup;

    std::vector<Ref<ORC_ProxyData>> gather_cascade_instances(const Ref<ORC_ProxyData>& start, bool upstream) const;
    bool has_cycle(const Ref<ORC_ProxyData>& node, std::unordered_set<const ORC_ProxyData*>& visited) const;    
    bool has_type_duplicate(const Ref<ORC_ProxyData>& node) const;
#endif
};

}

#endif // ORC_PROXYREGISTRY_H
