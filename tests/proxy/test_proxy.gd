extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	scn_instance = load_mock_scene()

func load_mock_scene() -> Node:
	var scene = load("res://addons/open-render-craft/tests/proxy/proxy_mock_scene.tscn") as PackedScene
	assert(scene != null, "load_mock_scene() : cannot load mock scene")
	var instance = scene.instantiate()
	assert(instance != null, "load_mock_scene() : cannot instantiate mock scene")
	get_tree().root.add_child(instance)
	return instance

func after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func wait_for_stabilisation() -> void:
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	await get_tree().process_frame
	
	var prev_obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	var max_iter = 32
	var obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	while (max_iter < 32 && prev_obj_count != obj_count):
		await get_tree().process_frame
		prev_obj_count = obj_count
		obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)

func test_setup_scene():
	await wait_for_stabilisation()
	
	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	scn_proxy.setup(scn_instance)

	#await wait_for_stabilisation()

	var nb_proxy_objs = 7
	var nb_primary_data = 7
	var nb_secondary_data = 2	# 2 TopologyData (cube is shared)
	var additional_expected = nb_proxy_objs + nb_primary_data + nb_secondary_data
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before + additional_expected)

func test_cleanup_scene():
	await wait_for_stabilisation()

	scn_proxy.setup(scn_instance)

	await wait_for_stabilisation()

	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	scn_proxy.cleanup()

	#await wait_for_stabilisation()

	var nb_proxy_objs = 7
	var nb_primary_data = 7
	var nb_secondary_data = 2	# 2 TopologyData (cube is shared)
	var removed_expected = nb_proxy_objs + nb_primary_data + nb_secondary_data
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before - removed_expected)
