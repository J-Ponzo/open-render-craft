extends ORC_ProxyFactory
class_name ORCTEST_ProxyFactory_GDMock

class ORCTEST_OmniLightProxy extends ORCTEST_MockProxyObject:
	static var omni_update_count : int = 0

	func update_override() -> void:
		super()
		ORCTEST_OmniLightProxy.omni_update_count += 1

class ORCTEST_OmniLightData extends ORC_PrimaryData:
	pass

class ORCTEST_SpotLightProxy extends ORCTEST_MockProxyObject:
	static var spot_update_count : int = 0

	func update_override() -> void:
		super()
		ORCTEST_SpotLightProxy.spot_update_count += 1

class ORCTEST_SpotLightData extends ORC_PrimaryData:
	pass

class ORCTEST_DirectionalLightProxy extends ORCTEST_MockProxyObject:
	static var directional_update_count : int = 0

	func update_override() -> void:
		super()
		ORCTEST_DirectionalLightProxy.directional_update_count += 1

class ORCTEST_DirectionalLightData extends ORC_PrimaryData:
	pass

func create_proxy_from_override(node : Node) -> ORC_ProxyObject:
	var proxy_object : ORC_ProxyObject = null
	if node is Camera3D:
		proxy_object = ORCTEST_CameraProxy.new()
	elif node is OmniLight3D:
		proxy_object = ORCTEST_OmniLightProxy.new()
	elif node is SpotLight3D:
		proxy_object = ORCTEST_SpotLightProxy.new()
	elif node is DirectionalLight3D:
		proxy_object = ORCTEST_DirectionalLightProxy.new()
	elif node is MeshInstance3D:
		proxy_object = ORCTEST_MeshProxy.new()
	return proxy_object
	
func create_data_from_override(node : Node, cache : ORC_ProxyRegistry) -> ORC_PrimaryData:
	var primary_data : ORC_PrimaryData = null
	if node is Camera3D:
		primary_data = create_and_register_primary(ORCTEST_CameraData, cache)
	elif node is OmniLight3D:
		primary_data = create_and_register_primary(ORCTEST_OmniLightData, cache)
	elif node is SpotLight3D:
		primary_data = create_and_register_primary(ORCTEST_SpotLightData, cache)
	elif node is DirectionalLight3D:
		primary_data = create_and_register_primary(ORCTEST_DirectionalLightData, cache)
	elif node is MeshInstance3D:
		primary_data = create_and_register_primary(ORCTEST_MeshData, cache)
		var topology_data = create_and_register_secondary(ORCTEST_TopologyData, cache, primary_data, node.mesh.get_rid().get_id())
		topology_data.mesh = node.mesh
		primary_data.topologyData = topology_data
		
	return primary_data;

func free_proxy_override(proxy_object : ORC_ProxyObject) -> bool:
		return true
		
func free_data_override(data : ORC_ProxyData, cache : ORC_ProxyRegistry) -> bool:
	var success = true
	if data is ORCTEST_TopologyData:
		var topology_data : ORCTEST_TopologyData = data
		success = success && destroy_and_unregister_data(data, cache, topology_data.mesh.get_rid().get_id())
	else:
		success = success && destroy_and_unregister_data(data, cache)
	return success

