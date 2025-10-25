extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	scn_instance = Test_ScnProxyCommon.load_mock_scene(get_tree())

func after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_cleanup_scene():
	await Test_ScnProxyCommon.wait_for_stabilisation(get_tree())

	scn_proxy.setup(scn_instance)

	await Test_ScnProxyCommon.wait_for_stabilisation(get_tree())

	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	scn_proxy.cleanup()

	await Test_ScnProxyCommon.wait_for_stabilisation(get_tree())

	var nb_proxy_objs = 7
	var nb_primary_data = 7
	var nb_secondary_data = 2	# 2 TopologyData (cube is shared)
	var removed_expected = nb_proxy_objs + nb_primary_data + nb_secondary_data
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before - removed_expected)
