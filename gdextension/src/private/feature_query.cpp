#include <feature_query.h>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void ORC_FeatureQuery::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_mask", "mask"), &ORC_FeatureQuery::set_mask);
    ClassDB::bind_method(D_METHOD("get_mask"), &ORC_FeatureQuery::get_mask);
    
    ClassDB::bind_method(D_METHOD("set_value", "value"), &ORC_FeatureQuery::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &ORC_FeatureQuery::get_value);
    
    ClassDB::bind_method(D_METHOD("clear"), &ORC_FeatureQuery::clear);
    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "mask"), "set_mask", "get_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
}

void ORC_FeatureQuery::set_mask(uint64_t p_mask) {
    mask = p_mask;
}

uint64_t ORC_FeatureQuery::get_mask() const {
    return mask;
}

void ORC_FeatureQuery::set_value(uint64_t p_value) {
    value = p_value;
}

uint64_t ORC_FeatureQuery::get_value() const {
    return value;
}

void ORC_FeatureQuery::clear() {
    mask = 0;
    value = 0;
}

}
