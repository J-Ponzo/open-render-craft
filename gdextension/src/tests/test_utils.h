#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class ORC_TestUtils : public Object {
    GDCLASS(ORC_TestUtils, Object);

protected:
    static void _bind_methods();

public:
    ORC_TestUtils() = default;
    ~ORC_TestUtils() = default;

    static bool is_running_test();
};

}
