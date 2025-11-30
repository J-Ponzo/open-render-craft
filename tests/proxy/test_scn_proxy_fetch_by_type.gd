extends GdUnitTestSuite
class_name ORCTEST_ScnProxyFetchByType

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func create_proxy_factory() -> ORC_ProxyFactory:
	return null

func create_mesh_query() -> ORC_DataQuery:
	return null

func create_topology_query() -> ORC_DataQuery:
	return null

func create_camera_query() -> ORC_DataQuery:
	return null

func create_omni_query() -> ORC_DataQuery:
	return null

func create_spot_query() -> ORC_DataQuery:
	return null

func create_directional_query() -> ORC_DataQuery:
	return null

func common_before() -> void:
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = create_proxy_factory()
	scn_proxy.setup(scn_instance)

	var meshes_query : ORC_DataQuery = create_mesh_query()
	scn_proxy.create_queue("meshes", meshes_query, [])
	var topologies_query : ORC_DataQuery = create_topology_query()
	scn_proxy.create_queue("topologies", topologies_query, [])

	scn_proxy.pre_render()

func common_after() -> void:
	scn_proxy.cleanup()
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func common_get_all_mesh():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var expected_nb = 3
	var actual_nb : int = scn_proxy.fetch_queue_data("meshes").size()
	assert_int(actual_nb).is_equal(expected_nb)

func common_get_all_topology():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	var expected_nb = 2
	var actual_nb : int = scn_proxy.fetch_queue_data("topologies").size()
	assert_int(actual_nb).is_equal(expected_nb)

func common_get_all_camera():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var camera_query : ORC_DataQuery = create_camera_query()
	scn_proxy.create_queue("cameras", camera_query, [])
	
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("cameras").size()
	assert_int(actual_nb).is_equal(expected_nb)

func common_get_all_omni():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var omni_query : ORC_DataQuery = create_omni_query()
	scn_proxy.create_queue("omnis", omni_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("omnis").size()
	assert_int(actual_nb).is_equal(expected_nb)

func common_get_all_spot():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var spot_query : ORC_DataQuery = create_spot_query()
	scn_proxy.create_queue("spots", spot_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("spots").size()
	assert_int(actual_nb).is_equal(expected_nb)

func common_get_all_directional():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var directional_query : ORC_DataQuery = create_directional_query()
	scn_proxy.create_queue("directionals", directional_query, [])
	scn_proxy.pre_render()

	var expected_nb = 1
	var actual_nb : int = scn_proxy.fetch_queue_data("directionals").size()
	assert_int(actual_nb).is_equal(expected_nb)
