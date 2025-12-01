#ifndef ORC_PROXYREGISTRY_H
#define ORC_PROXYREGISTRY_H

#include <typeindex>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <proxy_data.h>
#include <data_query.h>
#include "macros.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_ProxyRegistry : public RefCounted {
    GDCLASS(ORC_ProxyRegistry, RefCounted)

private:
    // Static registry for C++ types (Meyers' Singleton)
    static std::unordered_map<StringName, std::type_index>& cpp_types();
    static TypeKey get_type_key(Ref<ORC_ProxyData> proxy_data);
    
    // Instance members
    std::unordered_map<int64_t, std::tuple<Ref<ORC_ProxyData>, int>> id_registry;
    std::vector<Ref<ORC_ProxyData>> all_data;
    
    std::unordered_map<StringName, uint64_t> flag_name_to_mask;
    uint8_t next_available_bit = 0;
    // TODO try to use Ref<> instead of raw pointer
    std::unordered_map<ORC_ProxyData*, uint64_t> data_flags;
    std::unordered_map<Ref<ORC_DataQuery>, std::vector<Ref<ORC_ProxyData>>, DataQueryHash> query_cache;
    
    uint64_t get_or_create_flag_mask(const StringName& flag_name);
    bool matches_query(Ref<ORC_ProxyData> proxy_data, uint64_t flags, const Ref<ORC_DataQuery>& query) const;
    bool update_query_cache_for_data(Ref<ORC_ProxyData> proxy_data, uint64_t old_flags, uint64_t new_flags);
    bool remove_from_query_cache(Ref<ORC_ProxyData> proxy_data);
    bool add_query_to_cache(const Ref<ORC_DataQuery>& query);
    bool fill_query_features(Ref<ORC_DataQuery> query, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values);

protected:
    static void _bind_methods();

public:
    // Static methods
    template<typename T>
    static void register_cpp_type(const StringName& class_name) {
        cpp_types().emplace(class_name, typeid(T));
    }
    
    static std::type_index get_cpp_type_index(const StringName& class_name);
    
    // Instance methods
    bool register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id = -1);
    bool unregister_data(Ref<ORC_ProxyData> proxy_data);
    Ref<ORC_ProxyData> get_by_unique_id(int64_t unique_id) const;
    bool increment_refcount(int64_t unique_id);
    bool decrement_refcount(int64_t unique_id);
    
    bool set_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value);
    TypedArray<ORC_ProxyData> get_by_query(Ref<ORC_DataQuery> query);
    
    Ref<ORC_DataQuery> create_query(const TypeKey& type_key, const TypedArray<StringName>& flag_names = TypedArray<StringName>(), const TypedArray<bool>& flag_values = TypedArray<bool>());

    void clear();

    String dump_registry() const;
};

}

#endif // ORC_PROXYREGISTRY_H
