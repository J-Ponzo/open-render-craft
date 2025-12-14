#include "test_utils.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

void ORC_TestUtils::_bind_methods() {
    ClassDB::bind_static_method("ORC_TestUtils", D_METHOD("is_running_test"), &ORC_TestUtils::is_running_test);
}

bool ORC_TestUtils::is_running_test() {
    return FileAccess::file_exists("res://.gdunit_is_running_test");
}

}
