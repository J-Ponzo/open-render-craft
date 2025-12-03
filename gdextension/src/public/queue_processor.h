#ifndef ORC_QUEUEPROCESSOR_H
#define ORC_QUEUEPROCESSOR_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <proxy_data.h>
#include <macros.h>

namespace godot {

class ORC_SceneProxyBase;

class ORC_QueueProcessor : public RefCounted {
    GDCLASS(ORC_QueueProcessor, RefCounted)
    friend class ORC_SceneProxyBase;

private:
    //TODO use ref<> ?
    ORC_SceneProxyBase* scene_proxy;

protected:
    static void _bind_methods();

public:
    ORC_QueueProcessor() : scene_proxy(nullptr) {}
    DECLARE_GD_OVERRIDABLE_METHOD(TypedArray<ORC_ProxyData>, process, const TypedArray<ORC_ProxyData>& input)
};

}

#endif // ORC_QUEUEPROCESSOR_H
