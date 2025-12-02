#ifndef ORC_SECONDARYDATA_H
#define ORC_SECONDARYDATA_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <proxy_data.h>
#include <primary_data.h>

namespace godot {

class ORC_PrimaryData;

class ORC_SecondaryData : public ORC_ProxyData {
    GDCLASS(ORC_SecondaryData, ORC_ProxyData)

protected:
    static void _bind_methods();

public:
    TypedArray<ORC_PrimaryData> primary_data_array;
    TypedArray<ORC_PrimaryData> get_primary_data_array() const { return primary_data_array; }
    void set_primary_data_array(const TypedArray<ORC_PrimaryData>& primary_data_array) { this->primary_data_array = primary_data_array; }
};

}

#endif
