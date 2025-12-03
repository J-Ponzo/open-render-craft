#ifndef ORC_DATAQUERY_H
#define ORC_DATAQUERY_H

#include <typeindex>
#include <godot_cpp/classes/ref_counted.hpp>
#include "proxy_data.h"

namespace godot {

//TODO : do not expose to GDScript and make an actual struct instead ?
class ORC_DataQuery : public RefCounted {
    GDCLASS(ORC_DataQuery, RefCounted)

protected:
    static void _bind_methods();

public:
    TypeKey type_key;
    uint64_t mask;
    uint64_t value;

    ORC_DataQuery() : type_key(std::type_index(typeid(void))), mask(0), value(0) {}
};

struct DataQueryHash {
    size_t operator()(const Ref<ORC_DataQuery>& query) const {
        if (!query.is_valid()) return 0;
        
        size_t h1 = TypeKeyHash{}(query->type_key);
        size_t h2 = std::hash<uint64_t>{}(query->mask);
        size_t h3 = std::hash<uint64_t>{}(query->value);
        
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

}

#endif // ORC_DATAQUERY_H
