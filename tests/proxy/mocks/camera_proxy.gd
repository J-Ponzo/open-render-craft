extends ORCTEST_MockProxyObject
class_name ORCTEST_CameraProxy

static var cam_update_count : int = 0

func update_override() -> void:
	super()
	ORCTEST_CameraProxy.cam_update_count += 1
