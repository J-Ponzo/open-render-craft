extends ORCTEST_MockProxyObject
class_name ORCTEST_MeshProxy

static var mesh_update_count : int = 0

func update_override() -> void:
	super()
	ORCTEST_MeshProxy.mesh_update_count += 1
