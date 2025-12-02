#ifndef ORC_PROXYOBJECT_H
#define ORC_PROXYOBJECT_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <macros.h>
#include <primary_data.h>

namespace godot {

class ORC_PrimaryData;

class ORC_ProxyObject : public RefCounted {
    GDCLASS(ORC_ProxyObject, RefCounted)

protected:
    static void _bind_methods();

public:
    Node* node = nullptr;
    Node* get_node() const { return node; }
    void set_node(Node* node) { this->node = node; }

    Ref<ORC_PrimaryData> primary_data;
    Ref<ORC_PrimaryData> get_primary_data() const { return primary_data; }
    void set_primary_data(const Ref<ORC_PrimaryData>& primary_data) { this->primary_data = primary_data; }
    
    bool is_active_ = true;
    bool is_active() const { return is_active_; }
    void set_active(bool active) { is_active_ = active; }

    DECLARE_GD_OVERRIDABLE_METHOD(void, update)
};

}

#endif
