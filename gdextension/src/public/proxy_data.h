#ifndef ORC_PROXYDATA_H
#define ORC_PROXYDATA_H

#include <godot_cpp/classes/ref_counted.hpp>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_ProxyData : public RefCounted {
    GDCLASS(ORC_ProxyData, RefCounted)

protected:
    static void _bind_methods();

public:

};

}

#endif
