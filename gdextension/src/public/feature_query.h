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

    friend class ORC_ProxyRegistry;

private:
    uint64_t mask;
    uint64_t value;

protected:
    static void _bind_methods();

public:
    ORC_FeatureQuery() : mask(0), value(0) {}
    
    void set_mask(uint64_t p_mask);
    uint64_t get_mask() const;
    
    void set_value(uint64_t p_value);
    uint64_t get_value() const;
    
    void clear();
    
    bool operator==(const ORC_FeatureQuery& other) const {
        return mask == other.mask && value == other.value;
    }
    
    size_t hash() const {
        return std::hash<uint64_t>{}(mask) ^ (std::hash<uint64_t>{}(value) << 1);
    }
};

struct FeatureQueryHash {
    size_t operator()(const Ref<ORC_FeatureQuery>& query) const {
        return query.is_valid() ? query->hash() : 0;
    }
};

struct FeatureQueryEqual {
    bool operator()(const Ref<ORC_FeatureQuery>& a, const Ref<ORC_FeatureQuery>& b) const {
        if (!a.is_valid() || !b.is_valid()) return a.ptr() == b.ptr();
        return *a == *b;
    }
};

}

#endif // ORC_FEATUREQUERY_H
