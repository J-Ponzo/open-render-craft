#include <algorithm>
static const char* ERR_PR_UNREGISTER_FLAG_SOURCES_EMPTY = "[ORC] unregister_flag_sources_internal called with empty sources: explicit sources required";
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
static const char* ERR_PR_UNREGISTER_FLAG_SOURCES_PROXY_NOT_FOUND = "[ORC] unregister_flag_sources_internal: proxy_ref not found in cascade_sources";
static const char* ERR_PR_INVALID_FLAG_SRC = "[ORC] register_flag_sources_internal: invalid source in sources array";
static const char* ERR_PR_FLAG_SRC_ALREADY_REGISTERED = "[ORC] register_flag_sources_internal: source already registered for this proxy data";
static const char* ERR_PR_DUPLICATE_FLAG_SRC_IN_ARGS = "[ORC] register_flag_sources_internal: duplicate source in sources array";
static const char* ERR_PR_NO_CYCLES_RULE_BROKEN = "[ORC] Inconsistent flag cascade: No cycles rule is broken";
static const char* ERR_PR_TYPE_UNICITY_RULE_BROKEN = "[ORC] Inconsistent flag cascade: Type unicity rule is broken";

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
        id_lookup[unique_id] = std::make_tuple(proxy_data, 1);
    }

    data_pool.push_back(proxy_data);

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

    unregister_cascade_relations(proxy_data);
    remove_from_query_cache(proxy_data);
    data_flags.erase(proxy_data);
    
    data_pool.erase(std::remove(data_pool.begin(), data_pool.end(), proxy_data), data_pool.end());

    return true;
}

Ref<ORC_ProxyData> ORC_ProxyRegistry::get_by_unique_id(int64_t unique_id) const {
    auto it = id_lookup.find(unique_id);
    return (it != id_lookup.end()) ? std::get<0>(it->second) : Ref<ORC_ProxyData>();
}

bool ORC_ProxyRegistry::increment_refcount(int64_t unique_id) {
    auto it = id_lookup.find(unique_id);
    if (it == id_lookup.end()) ERR_FAIL_V_MSG(false, ERR_PR_UNIQUE_ID_NOT_FOUND);
    
    std::get<1>(it->second)++;
    std::get<0>(it->second)->shared = true;
    return true;
}

bool ORC_ProxyRegistry::decrement_refcount(int64_t unique_id) {
    auto it = id_lookup.find(unique_id);
    if (it == id_lookup.end()) ERR_FAIL_V_MSG(false, ERR_PR_UNIQUE_ID_NOT_FOUND);
    
    Ref<ORC_ProxyData>& proxy_data = std::get<0>(it->second);
    int& refcount = std::get<1>(it->second);
    refcount--;
    
    if (refcount == 1) proxy_data->shared = false;
    else if (refcount == 0) {
        unregister_data(proxy_data);
        id_lookup.erase(it);
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
    
    for (const auto& proxy_data : data_pool) {
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
    auto it = flag_mask_lookup.find(flag_name);
    
    if (it != flag_mask_lookup.end()) {
        return it->second;
    }

    if (next_available_bit >= 64) ERR_FAIL_V_MSG(0, vformat(ERR_PR_MAX_FLAGS_REACHED, String(flag_name)));
    
    uint64_t flag_mask = 1ULL << next_available_bit;
    flag_mask_lookup[flag_name] = flag_mask;
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
    
    propagate_flag_to_targets(proxy_data, flag_name, value);
    
    return true;
}

void ORC_ProxyRegistry::propagate_flag_to_targets(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value) {
    Ref<ORC_ProxyData> proxy_ref;
    proxy_ref.reference_ptr(proxy_data);
    
    auto it = cascade_targets.find(proxy_ref);
    if (it == cascade_targets.end()) return;
    
    for (const auto& target : it->second) {
        set_flag_internal(target.ptr(), flag_name, value);
    }
}

// Warning: In Shipping builds, this function does not perform error detection.
void ORC_ProxyRegistry::register_flag_sources_internal(ORC_ProxyData* proxy_data, const TypedArray<ORC_ProxyData>& sources) {
    DEV_ASSERT(proxy_data != nullptr && "Cannot register flag sources on null proxy_data.");
#ifdef DEBUG_ENABLED
    for (int i = 0; i < sources.size(); i++) {
        Ref<ORC_ProxyData> source = sources[i];
        if (!source.is_valid()) ERR_FAIL_MSG(ERR_PR_INVALID_FLAG_SRC);
        if (std::find(cascade_sources[proxy_data].begin(), cascade_sources[proxy_data].end(), source) != cascade_sources[proxy_data].end()) ERR_FAIL_MSG(ERR_PR_FLAG_SRC_ALREADY_REGISTERED);
        if (sources.count(source) > 1) ERR_FAIL_MSG(ERR_PR_DUPLICATE_FLAG_SRC_IN_ARGS);
    }
#endif

    Ref<ORC_ProxyData> proxy_ref;
    proxy_ref.reference_ptr(proxy_data);
    
    for (int i = 0; i < sources.size(); i++) {
        Ref<ORC_ProxyData> source = sources[i];
        cascade_targets[source].push_back(proxy_ref);
        cascade_sources[proxy_ref].push_back(source);
    }

#ifdef DEBUG_ENABLED
    std::unordered_set<const ORC_ProxyData*> in_path;
    if (has_cycle(proxy_ref, in_path)) ERR_FAIL_MSG(ERR_PR_NO_CYCLES_RULE_BROKEN);
    if (has_type_duplicate(proxy_ref)) ERR_FAIL_MSG(ERR_PR_TYPE_UNICITY_RULE_BROKEN);
#endif

    for (int i = 0; i < sources.size(); i++) {
        Ref<ORC_ProxyData> source = sources[i];
        uint64_t source_flags = data_flags[source];
        for (const auto& pair : flag_mask_lookup) {
            if ((source_flags & pair.second) != 0) {
                set_flag_internal(proxy_data, pair.first, true);
            }
        }
    }
}

void ORC_ProxyRegistry::unregister_flag_sources_internal(ORC_ProxyData* proxy_data, const TypedArray<ORC_ProxyData>& sources) {
    DEV_ASSERT(proxy_data != nullptr && "Cannot unregister flag sources on null proxy_data.");
    if (sources.is_empty()) ERR_FAIL_MSG(ERR_PR_UNREGISTER_FLAG_SOURCES_EMPTY);

    Ref<ORC_ProxyData> proxy_ref;
    proxy_ref.reference_ptr(proxy_data);
    auto sources_it = cascade_sources.find(proxy_ref);
    if (sources_it == cascade_sources.end()) ERR_FAIL_MSG(ERR_PR_UNREGISTER_FLAG_SOURCES_PROXY_NOT_FOUND);

    auto& current_sources = sources_it->second;
    for (int i = 0; i < sources.size(); i++) {
        Ref<ORC_ProxyData> source = sources[i];
        DEV_ASSERT(source.is_valid() && "unregister_flag_sources_internal: invalid source in sources array");
        if (!source.is_valid()) continue;

        auto targets_it = cascade_targets.find(source);
        if (targets_it != cascade_targets.end()) {
            auto& targets = targets_it->second;
            targets.erase(std::remove(targets.begin(), targets.end(), proxy_ref), targets.end());
            if (targets.empty()) cascade_targets.erase(targets_it);
        }

        current_sources.erase(std::remove(current_sources.begin(), current_sources.end(), source), current_sources.end());
    }
    if (current_sources.empty()) cascade_sources.erase(sources_it);
}

void ORC_ProxyRegistry::unregister_cascade_relations(const Ref<ORC_ProxyData>& proxy_data) {
    auto sources_it = cascade_sources.find(proxy_data);
    if (sources_it != cascade_sources.end()) {
        for (const auto& source : sources_it->second) {
            auto targets_it = cascade_targets.find(source);
            if (targets_it != cascade_targets.end()) {
                auto& targets = targets_it->second;
                targets.erase(std::remove(targets.begin(), targets.end(), proxy_data), targets.end());
                if (targets.empty()) cascade_targets.erase(targets_it);
            }
        }
        cascade_sources.erase(sources_it);
    }
    
    auto targets_it = cascade_targets.find(proxy_data);
    if (targets_it != cascade_targets.end()) {
        for (const auto& target : targets_it->second) {
            auto sources_it2 = cascade_sources.find(target);
            if (sources_it2 != cascade_sources.end()) {
                auto& sources = sources_it2->second;
                sources.erase(std::remove(sources.begin(), sources.end(), proxy_data), sources.end());
                if (sources.empty()) cascade_sources.erase(sources_it2);
            }
        }
        cascade_targets.erase(targets_it);
    }
}

bool ORC_ProxyRegistry::has_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name) {
    DEV_ASSERT(proxy_data != nullptr && "Cannot check flag on null proxy_data.");
    
    auto mask_it = flag_mask_lookup.find(flag_name);
    if (mask_it == flag_mask_lookup.end()) return false;
    
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
	id_lookup.clear();
	data_pool.clear();
	data_flags.clear();
	query_cache.clear();
	flag_mask_lookup.clear();
	cascade_sources.clear();
	cascade_targets.clear();
	next_available_bit = 0;
}

Ref<ORC_ProxyRegistryDump> ORC_ProxyRegistry::dump_registry() const {
    Ref<ORC_ProxyRegistryDump> dump;
    dump.instantiate();
    
    dump->id_lookup = id_lookup;
    dump->data_pool = data_pool;
    dump->flag_mask_lookup = flag_mask_lookup;
    dump->next_available_bit = next_available_bit;
    dump->data_flags = data_flags;
    dump->query_cache = query_cache;
    dump->cascade_sources = cascade_sources;
    dump->cascade_targets = cascade_targets;
    
    return dump;
}

TypedArray<StringName> ORC_ProxyRegistry::get_flags_internal(ORC_ProxyData* proxy_data) const {
    DEV_ASSERT(proxy_data != nullptr && "Cannot get flags from null proxy_data.");
    
    TypedArray<StringName> result;
    
    auto flags_it = data_flags.find(proxy_data);
    if (flags_it == data_flags.end()) return result;
    
    uint64_t flags = flags_it->second;
    
    for (const auto& pair : flag_mask_lookup) {
        if ((flags & pair.second) != 0) {
            result.push_back(pair.first);
        }
    }
    
    return result;
}

#ifdef DEBUG_ENABLED
bool ORC_ProxyRegistry::has_cycle(const Ref<ORC_ProxyData>& node, std::unordered_set<const ORC_ProxyData*>& in_path) const {
    if (!node.is_valid()) return false;
    if (in_path.count(node.ptr()) > 0) return true;
    
    in_path.insert(node.ptr());
    
    auto sources_it = cascade_sources.find(node);
    if (sources_it != cascade_sources.end()) {
        for (const auto& source : sources_it->second) {
            if (has_cycle(source, in_path)) return true;
        }
    }
    
    in_path.erase(node.ptr());
    return false;
}

bool ORC_ProxyRegistry::has_type_duplicate(const Ref<ORC_ProxyData>& node) const {
    std::vector<Ref<ORC_ProxyData>> graph_instances = gather_cascade_graph_instances(node);
    std::unordered_map<TypeKey, Ref<ORC_ProxyData>, TypeKeyHash> type_to_instance;
    
    for (const auto& instance : graph_instances) {
        TypeKey instance_type = instance->get_type_key();
        auto it = type_to_instance.find(instance_type);
        if (it != type_to_instance.end() && it->second.ptr() != instance.ptr()) return true;
        type_to_instance[instance_type] = instance;
    }
    
    return false;
}

std::vector<Ref<ORC_ProxyData>> ORC_ProxyRegistry::gather_cascade_graph_instances(const Ref<ORC_ProxyData>& start) const {
    std::vector<Ref<ORC_ProxyData>> result;
    std::vector<Ref<ORC_ProxyData>> to_visit;
    std::unordered_set<const ORC_ProxyData*> visited;
    
    to_visit.push_back(start);
    
    while (!to_visit.empty()) {
        Ref<ORC_ProxyData> current = to_visit.back();
        to_visit.pop_back();
        
        if (!current.is_valid()) continue;
        if (visited.count(current.ptr()) > 0) continue;
        
        visited.insert(current.ptr());
        result.push_back(current);
        
        auto sources_it = cascade_sources.find(current);
        if (sources_it != cascade_sources.end()) {
            for (const auto& source : sources_it->second) {
                if (source.is_valid() && visited.count(source.ptr()) == 0) {
                    to_visit.push_back(source);
                }
            }
        }
        
        auto targets_it = cascade_targets.find(current);
        if (targets_it != cascade_targets.end()) {
            for (const auto& target : targets_it->second) {
                if (target.is_valid() && visited.count(target.ptr()) == 0) {
                    to_visit.push_back(target);
                }
            }
        }
    }
    
    return result;
}
#endif

} // namespace godot
