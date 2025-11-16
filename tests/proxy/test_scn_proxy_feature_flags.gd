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

func test_is_primary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_FeatureQuery = scn_proxy.create_query(
		["IS_PRIMARY"],
		[true]
	)
	var actual_primary_count = scn_proxy.get_by_query(query).size()
	var expected_primary_count = nb_cam_data + nb_mesh_data + nb_omni_data + nb_spot_data + nb_directional_data
	assert_int(actual_primary_count).is_equal(expected_primary_count)

func test_is_secondary():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_FeatureQuery = scn_proxy.create_query(
		["IS_PRIMARY"],
		[false]
	)
	var actual_secondary_count = scn_proxy.get_by_query(query).size()
	var expected_secondary_count = nb_topology_data
	assert_int(actual_secondary_count).is_equal(expected_secondary_count)

func test_is_light():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_FeatureQuery = scn_proxy.create_query(
		["IS_LIGHT"],
		[true]
	)

	var actual_light_count = scn_proxy.get_by_query(query).size()
	var expected_light_count = nb_omni_data + nb_spot_data + nb_directional_data
	assert_int(actual_light_count).is_equal(expected_light_count)

func test_shadow():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var query : ORC_FeatureQuery = scn_proxy.create_query(
		["SHADOWS"],
		[true]
	)

	var actual_shadow_count = scn_proxy.get_by_query(query).size()
	var expected_shadow_count = nb_cast_shadow_at_start
	assert_int(actual_shadow_count).is_equal(expected_shadow_count)

func test_update_shadow():	
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	scn_instance.get_node("%Cube1").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_ON
	scn_instance.get_node("%Cube2").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_OFF
	scn_instance.get_node("%Sphere").cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_OFF

	scn_proxy.pre_render()

	var query : ORC_FeatureQuery = scn_proxy.create_query(
		["SHADOWS"],
		[true]
	)

	var actual_shadow_count = scn_proxy.get_by_query(query).size()
	var expected_shadow_count = nb_mesh_data - nb_cast_shadow_at_start
	assert_int(actual_shadow_count).is_equal(expected_shadow_count)
