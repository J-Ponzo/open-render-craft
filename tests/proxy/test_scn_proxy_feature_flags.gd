extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

var nb_mesh_data = 3
var nb_cam_data = 1
var nb_omni_data = 1
var nb_spot_data = 1
var nb_directional_data = 1
var nb_topology_data = 2	# cube shared
var nb_cast_shadow_at_start = 2

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	ORCTEST_ScnProxyTestsCommon.reset_proxy_update_counters()
	
	scn_proxy.setup(scn_instance)
	scn_proxy.pre_render()

func after() -> void:
	scn_proxy.cleanup()
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_camera_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_CameraData,
		["IS_PRIMARY"],
		[true]
	)
	scn_proxy.create_queue("queue_test_camera_is_primary", query, [])
	scn_proxy.pre_render()

	var actual_primary_count = scn_proxy.fetch_queue_data("queue_test_camera_is_primary").size()
	var expected_primary_count = nb_cam_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_mesh_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_MeshData,
		["IS_PRIMARY"],
		[true]
	)
	scn_proxy.create_queue("queue_test_mesh_is_primary", query, [])
	scn_proxy.pre_render()

	var actual_primary_count = scn_proxy.fetch_queue_data("queue_test_mesh_is_primary").size()
	var expected_primary_count = nb_mesh_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_omni_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightData,
		["IS_PRIMARY"],
		[true]
	)
	scn_proxy.create_queue("queue_test_omni_is_primary", query, [])
	scn_proxy.pre_render()

	var actual_primary_count = scn_proxy.fetch_queue_data("queue_test_omni_is_primary").size()
	var expected_primary_count = nb_omni_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_spot_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightData,
		["IS_PRIMARY"],
		[true]
	)
	scn_proxy.create_queue("queue_test_spot_is_primary", query, [])
	scn_proxy.pre_render()

	var actual_primary_count = scn_proxy.fetch_queue_data("queue_test_spot_is_primary").size()
	var expected_primary_count = nb_spot_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_directional_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightData,
		["IS_PRIMARY"],
		[true]
	)
	scn_proxy.create_queue("queue_test_directional_is_primary", query, [])
	scn_proxy.pre_render()

	var actual_primary_count = scn_proxy.fetch_queue_data("queue_test_directional_is_primary").size()
	var expected_primary_count = nb_directional_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_is_secondary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_TopologyData,
		["IS_PRIMARY"],
		[false]
	)
	scn_proxy.create_queue("queue_test_is_secondary", query, [])
	scn_proxy.pre_render()

	var actual_secondary_count = scn_proxy.fetch_queue_data("queue_test_is_secondary").size()
	var expected_secondary_count = nb_topology_data
	assert_int(actual_secondary_count).is_equal(expected_secondary_count)

func test_is_light_sum():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query_omni : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightData,
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_omni_test_is_light", query_omni, [])

	var query_spot : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightData,
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_spot_test_is_light", query_spot, [])

	var query_directional : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightData,
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_directional_test_is_light", query_directional, [])

	scn_proxy.pre_render()

	var actual_light_count = scn_proxy.fetch_queue_data("queue_omni_test_is_light").size() + scn_proxy.fetch_queue_data("queue_spot_test_is_light").size() + scn_proxy.fetch_queue_data("queue_directional_test_is_light").size()
	var expected_light_count = nb_omni_data + nb_spot_data + nb_directional_data
	assert_int(actual_light_count).is_equal(expected_light_count)

func test_is_light():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query_omni : ORC_DataQuery = scn_proxy.create_query_gd(
		null,
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_test_is_light", query_omni, [])

	scn_proxy.pre_render()

	var actual_light_count = scn_proxy.fetch_queue_data("queue_test_is_light").size()
	var expected_light_count = nb_omni_data + nb_spot_data + nb_directional_data
	assert_int(actual_light_count).is_equal(expected_light_count)

func test_shadow():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query_omni : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_omni_test_shadow", query_omni, [])
	var query_spot : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_spot_test_shadow", query_spot, [])
	var query_directional : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_directional_test_shadow", query_directional, [])
	scn_proxy.pre_render()


	var actual_shadow_count = scn_proxy.fetch_queue_data("queue_omni_test_shadow").size() + scn_proxy.fetch_queue_data("queue_spot_test_shadow").size() + scn_proxy.fetch_queue_data("queue_directional_test_shadow").size()
	var expected_shadow_count = nb_cast_shadow_at_start
	assert_int(actual_shadow_count).is_equal(expected_shadow_count)

func test_update_shadow():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	scn_instance.get_node("%Cube1").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_ON
	scn_instance.get_node("%Cube2").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_OFF
	scn_instance.get_node("%Sphere").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_OFF

	var query_omni : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_omni_test_shadow", query_omni, [])
	var query_spot : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_spot_test_shadow", query_spot, [])
	var query_directional : ORC_DataQuery = scn_proxy.create_query_gd(
		ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightData,
		["SHADOWS"],
		[true]
	)
	scn_proxy.create_queue("queue_directional_test_shadow", query_directional, [])
	scn_proxy.pre_render()


	var actual_shadow_count = scn_proxy.fetch_queue_data("queue_omni_test_shadow").size() + scn_proxy.fetch_queue_data("queue_spot_test_shadow").size() + scn_proxy.fetch_queue_data("queue_directional_test_shadow").size()
	var expected_shadow_count = nb_mesh_data - nb_cast_shadow_at_start
	assert_int(actual_shadow_count).is_equal(expected_shadow_count)
