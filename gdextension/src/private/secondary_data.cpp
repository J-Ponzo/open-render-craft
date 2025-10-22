#include <godot_cpp/core/class_db.hpp>

#include <secondary_data.h>

using namespace godot;

void ORC_SecondaryData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_primary_data_array"), &ORC_SecondaryData::get_primary_data_array);
    ClassDB::bind_method(D_METHOD("set_primary_data_array", "primary_data_array"), &ORC_SecondaryData::set_primary_data_array);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "primary_data_array", PROPERTY_HINT_RESOURCE_TYPE, "ORC_PrimaryData"), "set_primary_data_array", "get_primary_data_array");
}
