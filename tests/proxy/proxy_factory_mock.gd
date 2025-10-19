extends ORC_ProxyFactory
class_name ORCTEST_ProxyFactory_GDMock

class MeshProxy extends ORC_ProxyObject:
	pass
	
class MeshData extends ORC_PrimaryData:
	pass
	
class SurfaceData extends ORC_SecondaryData:
	pass
	
class TopologyData extends ORC_SecondaryData:
	pass
	
class CameraProxy extends ORC_ProxyObject:
	pass

class CameraData extends ORC_PrimaryData:
	pass

class OmniLightProxy extends ORC_ProxyObject:
	pass

class OmniLightData extends ORC_PrimaryData:
	pass
	
class SpotLightProxy extends ORC_ProxyObject:
	pass

class SpotLightData extends ORC_PrimaryData:
	pass
	
class DirectionalLightProxy extends ORC_ProxyObject:
	pass

class DirectionalLightData extends ORC_PrimaryData:
	pass

func create_from_override(node : Node, cache : ORC_ProxyCache) -> ORC_ProxyObject:
	var proxy_object : ORC_ProxyObject = null
	if node is Camera3D:
		proxy_object = CameraProxy.new()
	elif node is OmniLight3D:
		proxy_object = OmniLightProxy.new()
	elif node is SpotLight3D:
		proxy_object = SpotLightProxy.new()
	elif node is DirectionalLight3D:
		proxy_object = DirectionalLightProxy.new()
	elif node is MeshInstance3D:
		proxy_object = MeshProxy.new()			
	
	return proxy_object
