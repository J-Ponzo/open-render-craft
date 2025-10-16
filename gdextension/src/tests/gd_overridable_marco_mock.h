#ifndef ORCTEST_GDOVERRIDABLEMACROMOCK_H
#define ORCTEST_GDOVERRIDABLEMACROMOCK_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "macros.h"

namespace godot {

class ORCTEST_GDOverridableMacro_Mock : public RefCounted {
	GDCLASS(ORCTEST_GDOverridableMacro_Mock, RefCounted)

protected:
	static void _bind_methods();

public:
	int internal_state = 0;
	int get_internal_state() { return internal_state; }
	void set_internal_state(int internal_state) { this->internal_state = internal_state; }

	DECLARE_GD_OVERRIDABLE_METHOD(void, proc_0_args)
	DECLARE_GD_OVERRIDABLE_METHOD(void, proc_1_args, int)
	DECLARE_GD_OVERRIDABLE_METHOD(void, proc_2_args, int, int)
	DECLARE_GD_OVERRIDABLE_METHOD(void, proc_3_args, int, int, int)
	DECLARE_GD_OVERRIDABLE_METHOD(void, proc_4_args, int, int, int, int)
	DECLARE_GD_OVERRIDABLE_METHOD(int, func_0_args)
	DECLARE_GD_OVERRIDABLE_METHOD(int, func_1_args, int)
	DECLARE_GD_OVERRIDABLE_METHOD(int, func_2_args, int, int)
	DECLARE_GD_OVERRIDABLE_METHOD(int, func_3_args, int, int, int)
	DECLARE_GD_OVERRIDABLE_METHOD(int, func_4_args, int, int, int, int)
};

}

#endif