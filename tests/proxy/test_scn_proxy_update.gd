extends GdUnitTestSuite
class_name ORCTEST_ScnProxyUpdate

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node
var nb_updates : int = 8

var nb_mesh_proxy = 3
var nb_cam_proxy = 1
var nb_omni_proxy = 1
var nb_spot_proxy = 1
var nb_directional_proxy = 1

func create_proxy_factory() -> ORC_ProxyFactory:
	return null

func reset_proxy_update_counters() -> void:
	pass

func get_all_update_count() -> int:
	return 0

func get_mesh_update_count() -> int:
	return 0

func get_cam_update_count() -> int:
	return 0

func get_omni_update_count() -> int:
	return 0

func get_spot_update_count() -> int:
	return 0

func get_directional_update_count() -> int:
	return 0

func common_before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = create_proxy_factory()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	reset_proxy_update_counters()
	
	scn_proxy.setup(scn_instance)
	
	for i in range(nb_updates):
		scn_proxy.pre_render()

func common_after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func common_update_all_proxy():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var nb_all_proxy = nb_mesh_proxy + nb_cam_proxy + nb_omni_proxy + nb_spot_proxy + nb_directional_proxy
	var actual_all_update_count = get_all_update_count()
	var expected_all_update_count = nb_updates * nb_all_proxy
	assert_int(actual_all_update_count).is_equal(expected_all_update_count)

func common_update_mesh():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var actual_mesh_update_count = get_mesh_update_count()
	var expected_mesh_update_count = nb_updates * nb_mesh_proxy
	assert_int(actual_mesh_update_count).is_equal(expected_mesh_update_count)

func common_update_cam():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var actual_cam_update_count = get_cam_update_count()
	var expected_cam_update_count = nb_updates * nb_cam_proxy
	assert_int(actual_cam_update_count).is_equal(expected_cam_update_count)

func common_update_omni():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var actual_omni_update_count = get_omni_update_count()
	var expected_omni_update_count = nb_updates * nb_omni_proxy
	assert_int(actual_omni_update_count).is_equal(expected_omni_update_count)

func common_update_spot():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var actual_spot_update_count = get_spot_update_count()
	var expected_spot_update_count = nb_updates * nb_spot_proxy
	assert_int(actual_spot_update_count).is_equal(expected_spot_update_count)

func common_update_directional():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var actual_directional_update_count = get_directional_update_count()
	var expected_directional_update_count = nb_updates * nb_directional_proxy
	assert_int(actual_directional_update_count).is_equal(expected_directional_update_count)
