extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	
func test_init_scene():
	var scene = load("res://addons/open-render-craft/tests/proxy/proxy_mock_scene.tscn") as PackedScene
	assert(scene != null, "test_init_scene() : cannot load mock scene")
	
	var instance = scene.instantiate()
	assert(instance != null, "test_init_scene() : cannot instanciate mock scene")
	
	var prev_obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	var max_iter = 32
	get_tree().root.add_child(instance)
	await get_tree().process_frame
	
	# Wait for stabilisation
	var obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	while (max_iter < 32 && prev_obj_count != obj_count):
		await get_tree().process_frame
		prev_obj_count = obj_count
		obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	
	var nb_obj_before : int = obj_count
	scn_proxy.setup(instance)
	
	var nb_proxy_objs = 7
	var nb_primary_data = 7
	var nb_secondary_data = 5	# 3 SurfaceData + 2 TopologyData (cube is shared)
	var additional_expected = nb_proxy_objs + nb_primary_data + nb_secondary_data
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before + additional_expected)
