#ifndef ORC_PROXYREGISTRYDUMP_H
#define ORC_PROXYREGISTRYDUMP_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <proxy_data.h>
#include <data_query.h>
#include "macros.h"

#include <vector>
#include <unordered_map>
#include <tuple>

namespace godot {

class ORC_ProxyRegistryDump : public RefCounted {
    GDCLASS(ORC_ProxyRegistryDump, RefCounted)

private:
    static void _bind_methods();

public:
    std::unordered_map<int64_t, std::tuple<Ref<ORC_ProxyData>, int>> id_registry;
    std::vector<Ref<ORC_ProxyData>> all_data;
    std::unordered_map<StringName, uint64_t> flag_name_to_mask;
    uint8_t next_available_bit;
    std::unordered_map<Ref<ORC_ProxyData>, uint64_t, ProxyDataHash> data_flags;
    std::unordered_map<Ref<ORC_DataQuery>, std::vector<Ref<ORC_ProxyData>>, DataQueryHash> query_cache;

    ORC_ProxyRegistryDump();
    ~ORC_ProxyRegistryDump();

    String to_string() const;
};

}

#endif // ORC_PROXYREGISTRYDUMP_H
