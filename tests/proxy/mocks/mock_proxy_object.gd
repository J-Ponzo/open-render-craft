extends ORC_ProxyObject
class_name ORCTEST_MockProxyObject

static var all_update_count : int = 0

func update_override() -> void:
	self.super_update()
	ORCTEST_MockProxyObject.all_update_count += 1
