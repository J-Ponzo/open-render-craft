extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	scn_proxy.setup(scn_instance)

func after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_get_all_mesh():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var expected_nb = 3
	var actual_nb : int = scn_proxy.get_by_type(ORCTEST_MeshData).size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_topology():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var expected_nb = 2
	var actual_nb : int = scn_proxy.get_by_type(ORCTEST_TopologyData).size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_camera():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var expected_nb = 1
	var actual_nb : int = scn_proxy.get_by_type(ORCTEST_CameraData).size()
	assert_int(actual_nb).is_equal(expected_nb)
	
# Ignore this since Inner class are not supported
#func test_get_all_omni():
	#await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	#var expected_nb = 1
	#var actual_nb : int = scn_proxy.get_by_type(ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightData).size()
	#assert_int(actual_nb).is_equal(expected_nb)
	
# Ignore this since Inner class are not supported
#func test_get_all_spot():
	#await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	#var expected_nb = 1
	#var actual_nb : int = scn_proxy.get_by_type(ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightData).size()
	#assert_int(actual_nb).is_equal(expected_nb)

# Ignore this since Inner class are not supported	
#func test_get_all_directional():
	#await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	#var expected_nb = 1
	#var actual_nb : int = scn_proxy.get_by_type(ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightData).size()
	#assert_int(actual_nb).is_equal(expected_nb)
