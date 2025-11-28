extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

var nb_mesh_data = 3
var nb_cam_data = 1
var nb_omni_data = 1
var nb_spot_data = 1
var nb_directional_data = 1
var nb_topology_data = 2	# cube shared

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_CPPMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	ORCTEST_ScnProxyTestsCommon.reset_proxy_update_counters_cpp()
	
	scn_proxy.setup(scn_instance)
	scn_proxy.pre_render()

func after() -> void:
	scn_proxy.cleanup()
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_camera_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_CameraData_CPP",
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
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_MeshData_CPP",
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
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_OmniLightData_CPP",
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
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_SpotLightData_CPP",
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
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_DirectionalLightData_CPP",
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
	
	var query : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_TopologyData_CPP",
		["IS_PRIMARY"],
		[false]
	)
	scn_proxy.create_queue("queue_test_is_secondary", query, [])
	scn_proxy.pre_render()

	var actual_secondary_count = scn_proxy.fetch_queue_data("queue_test_is_secondary").size()
	var expected_secondary_count = nb_topology_data
	assert_int(actual_secondary_count).is_equal(expected_secondary_count)

func test_is_light():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query_omni : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_OmniLightData_CPP",
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_omni_test_is_light", query_omni, [])

	var query_spot : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_SpotLightData_CPP",
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_spot_test_is_light", query_spot, [])

	var query_directional : ORC_DataQuery = scn_proxy.create_query_cpp(
		"ORCTEST_DirectionalLightData_CPP",
		["IS_LIGHT"],
		[true]
	)
	scn_proxy.create_queue("queue_directional_test_is_light", query_directional, [])

	scn_proxy.pre_render()

	var actual_light_count = scn_proxy.fetch_queue_data("queue_omni_test_is_light").size() + scn_proxy.fetch_queue_data("queue_spot_test_is_light").size() + scn_proxy.fetch_queue_data("queue_directional_test_is_light").size()
	var expected_light_count = nb_omni_data + nb_spot_data + nb_directional_data
	assert_int(actual_light_count).is_equal(expected_light_count)
