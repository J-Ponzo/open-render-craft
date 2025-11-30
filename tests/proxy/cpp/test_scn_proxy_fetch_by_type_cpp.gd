extends ORCTEST_ScnProxyFetchByType

func before() -> void:
	super.common_before()

func after() -> void:
	super.common_after()

func create_proxy_factory() -> ORC_ProxyFactory:
	return ORCTEST_ProxyFactory_CPPMock.new()

func create_mesh_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_MeshData_CPP")

func create_topology_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_TopologyData_CPP")

func create_camera_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_CameraData_CPP")

func create_omni_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_OmniLightData_CPP")

func create_spot_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_SpotLightData_CPP")

func create_directional_query() -> ORC_DataQuery:
	return scn_proxy.create_query_cpp("ORCTEST_DirectionalLightData_CPP")

func test_get_all_mesh():
	await super.common_get_all_mesh()

func test_get_all_topology():
	await super.common_get_all_topology()

func test_get_all_camera():
	await super.common_get_all_camera()

func test_get_all_omni():
	await super.common_get_all_omni()

func test_get_all_spot():
	await super.common_get_all_spot()

func test_get_all_directional():
	await super.common_get_all_directional()
