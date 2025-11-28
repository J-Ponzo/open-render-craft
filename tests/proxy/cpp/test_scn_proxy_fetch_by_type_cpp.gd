extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_CPPMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())

	scn_proxy.setup(scn_instance)

	var meshes_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_MeshData_CPP")
	scn_proxy.create_queue("meshes", meshes_query, [])
	var topologies_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_TopologyData_CPP")
	scn_proxy.create_queue("topologies", topologies_query, [])

	scn_proxy.pre_render()

func after() -> void:
	scn_proxy.cleanup()
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_get_all_mesh():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var expected_nb = 3
	var actual_nb : int = scn_proxy.fetch_queue_data("meshes").size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_topology():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var expected_nb = 2
	var actual_nb : int = scn_proxy.fetch_queue_data("topologies").size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_camera():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var camera_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_CameraData_CPP")
	scn_proxy.create_queue("cameras", camera_query, [])
	
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("cameras").size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_omni():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var omni_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_OmniLightData_CPP")
	scn_proxy.create_queue("omnis", omni_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("omnis").size()
	assert_int(actual_nb).is_equal(expected_nb)
	
func test_get_all_spot():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var spot_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_SpotLightData_CPP")
	scn_proxy.create_queue("spots", spot_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("spots").size()
	assert_int(actual_nb).is_equal(expected_nb)

func test_get_all_directional():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var directional_query : ORC_DataQuery = scn_proxy.create_query_cpp("ORCTEST_DirectionalLightData_CPP")
	scn_proxy.create_queue("directionals", directional_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("directionals").size()
	assert_int(actual_nb).is_equal(expected_nb)
