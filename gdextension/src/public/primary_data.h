#ifndef ORC_PRIMARYDATA_H
#define ORC_PRIMARYDATA_H

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <proxy_data.h>
#include <proxy_object.h>
#include <secondary_data.h>

namespace godot {

class ORC_ProxyObject;
class ORC_SecondaryData;

class ORC_PrimaryData : public ORC_ProxyData {
    GDCLASS(ORC_PrimaryData, ORC_ProxyData)

protected:
    static void _bind_methods();

public:
    Ref<ORC_ProxyObject> proxy_object;
    Ref<ORC_ProxyObject> get_proxy_object() const { return proxy_object; }
    void set_proxy_object(const Ref<ORC_ProxyObject>& proxy_object) { this->proxy_object = proxy_object; }

    TypedArray<ORC_SecondaryData> secondary_data_array;
    TypedArray<ORC_SecondaryData> get_secondary_data() const { return secondary_data_array; }
    void set_secondary_data(const TypedArray<ORC_SecondaryData>& secondary_data_array) { this->secondary_data_array = secondary_data_array; }
};

}

#endif
