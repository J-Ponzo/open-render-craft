#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "gd_overridable_marco_mock.h"
#include "macros.h"

using namespace godot;

void ORCTEST_GDOverridableMacro_Mock::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, proc_0_args)
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, proc_1_args, "arg1")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, proc_2_args, "arg1", "arg2")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, proc_3_args, "arg1", "arg2", "arg3")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, proc_4_args, "arg1", "arg2", "arg3", "arg4")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, func_0_args)
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, func_1_args, "arg1")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, func_2_args, "arg1", "arg2")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, func_3_args, "arg1", "arg2", "arg3")
	BIND_GD_OVERRIDABLE_METHOD(ORCTEST_GDOverridableMacro_Mock, func_4_args, "arg1", "arg2", "arg3", "arg4")
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORCTEST_GDOverridableMacro_Mock, void, proc_0_args)
void ORCTEST_GDOverridableMacro_Mock::proc_0_args() {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.proc_0_args");
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORCTEST_GDOverridableMacro_Mock, void, proc_1_args, int, arg1)
void ORCTEST_GDOverridableMacro_Mock::proc_1_args(int arg1) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.proc_1_args ", arg1);
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORCTEST_GDOverridableMacro_Mock, void, proc_2_args, int, arg1, int, arg2)
void ORCTEST_GDOverridableMacro_Mock::proc_2_args(int arg1, int arg2) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.proc_1_args ", arg1, arg2);
}

DEFINE_GD_OVERRIDABLE_METHOD_3_ARGS(ORCTEST_GDOverridableMacro_Mock, void, proc_3_args, int, arg1, int, arg2, int, arg3)
void ORCTEST_GDOverridableMacro_Mock::proc_3_args(int arg1, int arg2, int arg3) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.proc_1_args ", arg1, arg2, arg3);
}

DEFINE_GD_OVERRIDABLE_METHOD_4_ARGS(ORCTEST_GDOverridableMacro_Mock, void, proc_4_args, int, arg1, int, arg2, int, arg3, int, arg4)
void ORCTEST_GDOverridableMacro_Mock::proc_4_args(int arg1, int arg2, int arg3, int arg4) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.proc_1_args ", arg1, arg2, arg3, arg4);
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORCTEST_GDOverridableMacro_Mock, int, func_0_args)
int ORCTEST_GDOverridableMacro_Mock::func_0_args() {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.func_0_args");
    return 0;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORCTEST_GDOverridableMacro_Mock, int, func_1_args, int, arg1)
int ORCTEST_GDOverridableMacro_Mock::func_1_args(int arg1) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.func_1_args ", arg1);
    return arg1;
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORCTEST_GDOverridableMacro_Mock, int, func_2_args, int, arg1, int, arg2)
int ORCTEST_GDOverridableMacro_Mock::func_2_args(int arg1, int arg2) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.func_1_args ", arg1, arg2);
    return arg1 + arg2;
}

DEFINE_GD_OVERRIDABLE_METHOD_3_ARGS(ORCTEST_GDOverridableMacro_Mock, int, func_3_args, int, arg1, int, arg2, int, arg3)
int ORCTEST_GDOverridableMacro_Mock::func_3_args(int arg1, int arg2, int arg3) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.func_1_args ", arg1, arg2, arg3);
    return arg1 + arg2 + arg3;
}

DEFINE_GD_OVERRIDABLE_METHOD_4_ARGS(ORCTEST_GDOverridableMacro_Mock, int, func_4_args, int, arg1, int, arg2, int, arg3, int, arg4)
int ORCTEST_GDOverridableMacro_Mock::func_4_args(int arg1, int arg2, int arg3, int arg4) {
	UtilityFunctions::print("ORCTEST_GDOverridableMacro_Mock.func_1_args ", arg1, arg2, arg3, arg4);
    return arg1 + arg2 + arg3 + arg4;
}