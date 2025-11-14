#include <feature_query.h>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void ORC_FeatureQuery::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_mask", "mask"), &ORC_FeatureQuery::set_mask);
    ClassDB::bind_method(D_METHOD("get_mask"), &ORC_FeatureQuery::get_mask);
    
    ClassDB::bind_method(D_METHOD("set_value", "value"), &ORC_FeatureQuery::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &ORC_FeatureQuery::get_value);
    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "mask"), "set_mask", "get_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
}

}
