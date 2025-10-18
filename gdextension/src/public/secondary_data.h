#ifndef ORC_SECONDARYDATA_H
#define ORC_SECONDARYDATA_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <proxy_data.h>
#include <primary_data.h>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_PrimaryData;

class ORC_API ORC_SecondaryData : public ORC_ProxyData {
    GDCLASS(ORC_SecondaryData, ORC_ProxyData)

protected:
    static void _bind_methods();

public:
    Ref<ORC_PrimaryData> primary_data;
    Ref<ORC_PrimaryData> get_primary_data() { return primary_data; }
    void set_primary_data(Ref<ORC_PrimaryData> primary_data) { this->primary_data = primary_data; }
};

}

#endif
