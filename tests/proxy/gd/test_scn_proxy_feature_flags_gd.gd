extends ORCTEST_ScnProxyFeatureFlags

func before() -> void:
	super.common_before()

func after() -> void:
	super.common_after()

func create_proxy_factory() -> ORC_ProxyFactory:
	return ORCTEST_ProxyFactory_GDMock.new()

func reset_proxy_update_counters() -> void:
	ORCTEST_ScnProxyTestsCommon.reset_proxy_update_counters_gd()

func create_camera_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_CameraData, flag_names, flag_values)

func create_mesh_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_MeshData, flag_names, flag_values)

func create_omni_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_OmniLightData, flag_names, flag_values)

func create_spot_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_SpotLightData, flag_names, flag_values)

func create_directional_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_DirectionalLightData, flag_names, flag_values)

func create_topology_query(flag_names : Array, flag_values : Array) -> ORC_DataQuery:
	return scn_proxy.create_query_gd(ORCTEST_TopologyData, flag_names, flag_values)

func test_camera_is_primary():
	await super.common_camera_is_primary()

func test_mesh_is_primary():
	await super.common_mesh_is_primary()

func test_omni_is_primary():
	await super.common_omni_is_primary()

func test_spot_is_primary():
	await super.common_spot_is_primary()

func test_directional_is_primary():
	await super.common_directional_is_primary()

func test_is_secondary():
	await super.common_is_secondary()

func test_is_light():
	await super.common_is_light()

func test_shadow():
	await super.common_shadow()

func test_update_shadow():
	await super.common_update_shadow()
