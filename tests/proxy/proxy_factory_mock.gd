extends ORC_ProxyFactory
class_name ORCTEST_ProxyFactory_GDMock

class MeshProxy extends ORC_ProxyObject:
	pass
	
class MeshData extends ORC_PrimaryData:
	var topologyData : TopologyData
	
class TopologyData extends ORC_SecondaryData:
	var mesh : Mesh
	
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

func create_proxy_from_override(node : Node) -> ORC_ProxyObject:
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
	
func create_data_from_override(node : Node, cache : ORC_ProxyCache) -> ORC_PrimaryData:
	var primary_data : ORC_PrimaryData = null
	if node is Camera3D:
		primary_data = create_and_register_primary(CameraData, cache)
	elif node is OmniLight3D:
		primary_data = create_and_register_primary(OmniLightData, cache)
	elif node is SpotLight3D:
		primary_data = create_and_register_primary(SpotLightData, cache)
	elif node is DirectionalLight3D:
		primary_data = create_and_register_primary(DirectionalLightData, cache)
	elif node is MeshInstance3D:
		primary_data = create_and_register_primary(MeshData, cache)
		var topology_data = create_and_register_secondary(TopologyData, cache, primary_data, node.mesh.get_rid().get_id())
		topology_data.mesh = node.mesh
		primary_data.topologyData = topology_data
		
	return primary_data;

func free_proxy_override(proxy_object : ORC_ProxyObject) -> bool:
		proxy_object.unreference()
		return true
		
func free_data_override(data : ORC_ProxyData, cache : ORC_ProxyCache) -> bool:
	var success = true
	if data is TopologyData:
		var topology_data : TopologyData = data
		success &= destroy_and_unregister_data(data, cache, topology_data.mesh.get_rid().get_id())
	else:
		success &= destroy_and_unregister_data(data, cache)
	return success
