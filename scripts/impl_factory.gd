class_name ORC_ImplFactory

static func create_impl(impl_def : ORC_AImpl_Def) -> Object:
	if impl_def == null:
		return null
	
	if impl_def is ORC_CPPImpl_Def:
		var casted_impl_def : ORC_CPPImpl_Def = impl_def
		return ClassDB.instantiate(casted_impl_def.cpp_class_name)
	elif impl_def is ORC_GDImpl_Def:
		var casted_impl_def : ORC_GDImpl_Def = impl_def
		return casted_impl_def.gd_script.new()

	return null
	
