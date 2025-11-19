class_name ORC_ImplFactory

static func create_impl(impl_def : ORC_AImpl_Def) -> Object:
	if impl_def == null:
		return null
	
	var instance : Object = null
	
	if impl_def is ORC_CPPImpl_Def:
		var casted_impl_def : ORC_CPPImpl_Def = impl_def
		instance = ClassDB.instantiate(casted_impl_def.cpp_class_name)
	elif impl_def is ORC_GDImpl_Def:
		var casted_impl_def : ORC_GDImpl_Def = impl_def
		instance = casted_impl_def.gd_script.new()

	if instance != null:
		_apply_parameters(instance, impl_def.parameters)
	
	return instance

static func _apply_parameters(instance : Object, parameters : Dictionary) -> void:
	for key in parameters.keys():
		instance.set(key, parameters[key])
	
