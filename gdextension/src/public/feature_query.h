#ifndef ORC_FEATUREQUERY_H
#define ORC_FEATUREQUERY_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <cstdint>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_FeatureQuery : public RefCounted {
    GDCLASS(ORC_FeatureQuery, RefCounted)

protected:
    static void _bind_methods();

public:
    uint64_t mask;
    uint64_t value;
    
    void set_mask(uint64_t p_mask) { mask = p_mask; }
    uint64_t get_mask() const { return mask; }
    
    void set_value(uint64_t p_value) { value = p_value; }
    uint64_t get_value() const { return value; }
};

}

#endif // ORC_FEATUREQUERY_H
