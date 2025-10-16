extends ORCTEST_GDOverridableMacro_Mock
class_name ORCTEST_GDOverridableMacro_GDDerivedMock

func proc_0_args_override() -> void:
	super_proc_0_args()
	internal_state *= -1
	print("ORCTEST_GDOverridableMacro_GDDerivedMock.proc_0_args")
	
func proc_1_args_override(arg1 : int) -> void:
	super_proc_1_args(arg1)
	internal_state *= -1
	print("ORCTEST_GDOverridableMacro_GDDerivedMock.proc_1_args ", arg1)
	
func proc_2_args_override(arg1 : int, arg2 : int) -> void:
	super_proc_2_args(arg1, arg2)
	internal_state *= -1
	print("ORCTEST_GDOverridableMacro_GDDerivedMock.proc_2_args ", arg1, " ", arg2)
	
func proc_3_args_override(arg1 : int, arg2 : int, arg3 : int) -> void:
	super_proc_3_args(arg1, arg2, arg3)
	internal_state *= -1
	print("ORCTEST_GDOverridableMacro_GDDerivedMock.proc_3_args ", arg1, " ", arg2, " ", arg3)
	
func proc_4_args_override(arg1 : int, arg2 : int, arg3 : int, arg4 : int) -> void:
	super_proc_4_args(arg1, arg2, arg3, arg4)
	internal_state *= -1
	print("ORCTEST_GDOverridableMacro_GDDerivedMock.proc_4_args ", arg1, " ", arg2, " ", arg3, " ", arg4)

func func_0_args_override() -> int:
	var result : int = -super_func_0_args()
	print(result, " ORCTEST_GDOverridableMacro_GDDerivedMock.func_0_args")
	return result
	
func func_1_args_override(arg1 : int) -> int:
	var result : int = -super_func_1_args(arg1)
	print(result, " ORCTEST_GDOverridableMacro_GDDerivedMock.func_1_args ", arg1)
	return result
	
func func_2_args_override(arg1 : int, arg2 : int) -> int:
	var result : int = -super_func_2_args(arg1, arg2)
	print(result, " ORCTEST_GDOverridableMacro_GDDerivedMock.func_2_args ", arg1, " ", arg2)
	return result
	
func func_3_args_override(arg1 : int, arg2 : int, arg3 : int) -> int:
	var result : int = -super_func_3_args(arg1, arg2, arg3)
	print(result, " ORCTEST_GDOverridableMacro_GDDerivedMock.func_3_args ", arg1, " ", arg2, " ", arg3)
	return result
	
func func_4_args_override(arg1 : int, arg2 : int, arg3 : int, arg4 : int) -> int:
	var result : int = -super_func_4_args(arg1, arg2, arg3, arg4)
	print(result, " ORCTEST_GDOverridableMacro_GDDerivedMock.func_4_args ", arg1, " ", arg2, " ", arg3, " ", arg4)
	return result
