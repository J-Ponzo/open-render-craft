#include <proxy_registry.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <primary_data.h>
#include <secondary_data.h>

namespace godot {

static const char* ERR_PR_CPP_TYPE_NOT_REGISTERED = "[ORC] C++ type '%s' not registered. Call ORC_ProxyRegistry::register_cpp_type<YourType>(\"%s\") in your module initialization.";
static const char* ERR_PR_MAX_FLAGS_REACHED = "[ORC] Maximum number of flags (64) reached. Cannot create new flag: %s";
static const char* ERR_PR_NULL_PROXY_DATA = "[ORC] Cannot set flag on null proxy_data.";
static const char* ERR_PR_INVALID_PROXY_DATA = "[ORC] Proxy data is not valid.";
static const char* ERR_PR_INVALID_QUERY = "[ORC] Query is not valid.";
static const char* ERR_PR_FLAG_ARRAY_SIZE_MISMATCH = "[ORC] flag_names and flag_values arrays must have the same size.";
static const char* ERR_PR_UNIQUE_ID_NOT_FOUND = "[ORC] Unique ID not found in registry.";
static const char* ERR_PR_QUERY_ALREADY_CACHED = "[ORC] Query is already in cache.";

std::unordered_map<StringName, std::type_index>& ORC_ProxyRegistry::cpp_types() {
    static std::unordered_map<StringName, std::type_index> registry;
    return registry;
}

std::type_index ORC_ProxyRegistry::get_cpp_type_index(const StringName& class_name) {
    auto& registry = cpp_types();
    auto it = registry.find(class_name);
    if (it == registry.end()) ERR_FAIL_V_MSG(typeid(void), vformat(ERR_PR_CPP_TYPE_NOT_REGISTERED, class_name, class_name));
    return it->second;
}

void ORC_ProxyRegistry::_bind_methods() {
}

bool ORC_ProxyRegistry::register_data(const Ref<ORC_ProxyData>& proxy_data, int64_t unique_id) {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_PROXY_DATA);

    proxy_data->registry = this;

    if (unique_id != -1) {
        id_registry[unique_id] = std::make_tuple(proxy_data, 1);
    }

    all_data.push_back(proxy_data);

    uint64_t flags = 0;
    auto flags_it = data_flags.find(proxy_data);
    if (flags_it != data_flags.end()) {
        flags = flags_it->second;
    }
    
    for (auto& cache_entry : query_cache) {
        const Ref<ORC_DataQuery>& query = cache_entry.first;
        DEV_ASSERT(query.is_valid() && "Invalid query found in cache - this should never happen");
        
        if (matches_query(proxy_data, flags, query)) {
            cache_entry.second.push_back(proxy_data);
        }
    }

    return true;
}

bool ORC_ProxyRegistry::unregister_data(const Ref<ORC_ProxyData>& proxy_data) {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_PROXY_DATA);

    proxy_data->registry = nullptr;

    remove_from_query_cache(proxy_data);
    data_flags.erase(proxy_data);
    
    all_data.erase(std::remove(all_data.begin(), all_data.end(), proxy_data), all_data.end());

    return true;
}

Ref<ORC_ProxyData> ORC_ProxyRegistry::get_by_unique_id(int64_t unique_id) const {
    auto it = id_registry.find(unique_id);
    return (it != id_registry.end()) ? std::get<0>(it->second) : Ref<ORC_ProxyData>();
}

bool ORC_ProxyRegistry::increment_refcount(int64_t unique_id) {
    auto it = id_registry.find(unique_id);
    if (it == id_registry.end()) ERR_FAIL_V_MSG(false, ERR_PR_UNIQUE_ID_NOT_FOUND);
    
    std::get<1>(it->second)++;
    return true;
}

bool ORC_ProxyRegistry::decrement_refcount(int64_t unique_id) {
    auto it = id_registry.find(unique_id);
    if (it == id_registry.end()) ERR_FAIL_V_MSG(false, ERR_PR_UNIQUE_ID_NOT_FOUND);
    
    std::get<1>(it->second)--;
    if (std::get<1>(it->second) == 0) {
        unregister_data(std::get<0>(it->second));
        id_registry.erase(it);
    }
    return true;
}

bool ORC_ProxyRegistry::matches_query(const Ref<ORC_ProxyData>& proxy_data, uint64_t flags, const Ref<ORC_DataQuery>& query) const {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_PROXY_DATA);
    if (!query.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_QUERY);
    
    if (!(proxy_data->get_type_key() == query->type_key)) return false;
    
    return (flags & query->mask) == (query->value & query->mask);
}

bool ORC_ProxyRegistry::update_query_cache_for_data(const Ref<ORC_ProxyData>& proxy_data, uint64_t old_flags, uint64_t new_flags) {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_PROXY_DATA);
    
    for (auto& cache_entry : query_cache) {
        const Ref<ORC_DataQuery>& query = cache_entry.first;
        DEV_ASSERT(query.is_valid() && "Invalid query found in cache - this should never happen");
        
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        
        bool old_match = matches_query(proxy_data, old_flags, query);
        bool new_match = matches_query(proxy_data, new_flags, query);
        
        if (old_match && !new_match) {
            data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
        } else if (!old_match && new_match) {
            data_list.push_back(proxy_data);
        }
    }
    return true;
}

bool ORC_ProxyRegistry::remove_from_query_cache(const Ref<ORC_ProxyData>& proxy_data) {
    if (!proxy_data.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_PROXY_DATA);
    
    for (auto& cache_entry : query_cache) {
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
    }
    return true;
}

bool ORC_ProxyRegistry::add_query_to_cache(const Ref<ORC_DataQuery>& query) {
    if (!query.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_QUERY);
    if (query_cache.find(query) != query_cache.end()) ERR_FAIL_V_MSG(false, ERR_PR_QUERY_ALREADY_CACHED);
    
    std::vector<Ref<ORC_ProxyData>> matching_data;
    
    for (const auto& proxy_data : all_data) {
        if (!proxy_data.is_valid()) continue;
        
        uint64_t flags = 0;
        auto flags_it = data_flags.find(proxy_data);
        if (flags_it != data_flags.end()) {
            flags = flags_it->second;
        }
        
        if (matches_query(proxy_data, flags, query)) {
            matching_data.push_back(proxy_data);
        }
    }
    
    query_cache[query] = matching_data;
    return true;
}

uint64_t ORC_ProxyRegistry::get_or_create_flag_mask(const StringName& flag_name) {
    auto it = flag_name_to_mask.find(flag_name);
    
    if (it != flag_name_to_mask.end()) {
        return it->second;
    }
    
    if (next_available_bit >= 64) ERR_FAIL_V_MSG(0, vformat(ERR_PR_MAX_FLAGS_REACHED, String(flag_name)));
    
    uint64_t flag_mask = 1ULL << next_available_bit;
    flag_name_to_mask[flag_name] = flag_mask;
    next_available_bit++;
    
    return flag_mask;
}

bool ORC_ProxyRegistry::set_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value) {
    DEV_ASSERT(proxy_data != nullptr && "Cannot set flag on null proxy_data.");
    
    uint64_t flag_mask = get_or_create_flag_mask(flag_name);
    
    uint64_t old_flags = data_flags[proxy_data];
    uint64_t new_flags = value ? (old_flags | flag_mask) : (old_flags & ~flag_mask);
    
    if (old_flags == new_flags) return false;

    data_flags[proxy_data] = new_flags;
    
    Ref<ORC_ProxyData> proxy_ref;
    proxy_ref.reference_ptr(proxy_data);
    update_query_cache_for_data(proxy_ref, old_flags, new_flags);
    
    return true;
}

bool ORC_ProxyRegistry::has_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name) {
    DEV_ASSERT(proxy_data != nullptr && "Cannot check flag on null proxy_data.");
    
    auto mask_it = flag_name_to_mask.find(flag_name);
    if (mask_it == flag_name_to_mask.end()) return false;
    
    uint64_t flag_mask = mask_it->second;
    
    auto flags_it = data_flags.find(proxy_data);
    if (flags_it == data_flags.end()) return false;
    
    return (flags_it->second & flag_mask) != 0;
}

TypedArray<ORC_ProxyData> ORC_ProxyRegistry::get_by_query_internal(const Ref<ORC_DataQuery>& query) {
    TypedArray<ORC_ProxyData> result;

    if (!query.is_valid()) ERR_FAIL_V_MSG(result, ERR_PR_INVALID_QUERY);

    auto it = query_cache.find(query);
    if (it == query_cache.end()) {
        add_query_to_cache(query);
        it = query_cache.find(query);
    }
    
    if (it != query_cache.end()) {
        for (const auto& data : it->second) {
            result.append(data);
        }
    }
    
    return result;
}

bool ORC_ProxyRegistry::fill_query_features(const Ref<ORC_DataQuery>& query, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values) {
    if (!query.is_valid()) ERR_FAIL_V_MSG(false, ERR_PR_INVALID_QUERY);
    if (flag_names.size() != flag_values.size()) ERR_FAIL_V_MSG(false, ERR_PR_FLAG_ARRAY_SIZE_MISMATCH);
    
    uint64_t mask = 0;
    uint64_t value = 0;
    
    for (int i = 0; i < flag_names.size(); i++) {
        StringName flag_name = flag_names[i];
        bool flag_value = flag_values[i];
        
        uint64_t flag_mask = get_or_create_flag_mask(flag_name);
        mask |= flag_mask;
        
        if (flag_value) {
            value |= flag_mask;
        }
    }
    
    query->mask = mask;
    query->value = value;
    
    return true;
}

Ref<ORC_DataQuery> ORC_ProxyRegistry::create_query_internal(const TypeKey& type_key, const TypedArray<StringName> &flag_names, const TypedArray<bool> &flag_values)
{
    Ref<ORC_DataQuery> query;
    query.instantiate();
    fill_query_features(query, flag_names, flag_values);
    query->type_key = type_key;
    
    return query;
}

void ORC_ProxyRegistry::clear() {
	id_registry.clear();
	all_data.clear();
	data_flags.clear();
	query_cache.clear();
	flag_name_to_mask.clear();
	next_available_bit = 0;
}

Ref<ORC_ProxyRegistryDump> ORC_ProxyRegistry::dump_registry() const {
    Ref<ORC_ProxyRegistryDump> dump;
    dump.instantiate();
    
    dump->id_registry = id_registry;
    dump->all_data = all_data;
    dump->flag_name_to_mask = flag_name_to_mask;
    dump->next_available_bit = next_available_bit;
    dump->data_flags = data_flags;
    dump->query_cache = query_cache;
    
    return dump;
}

} // namespace godot
