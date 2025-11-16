extends ORCTEST_MockProxyObject
class_name ORCTEST_MeshProxy

static var mesh_update_count : int = 0

var cast_shadow_last_frame : bool = false

func update_override() -> void:
	super()
	ORCTEST_MeshProxy.mesh_update_count += 1

	var cast_shadow : bool = node.cast_shadow != GeometryInstance3D.SHADOW_CASTING_SETTING_OFF
	if cast_shadow_last_frame != cast_shadow:
		primary_data.set_flag("SHADOWS", cast_shadow)
		cast_shadow_last_frame = cast_shadow

