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
	
	get_tree().root.add_child(instance)
	
	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT) 
	scn_proxy.setup(instance)
	assert_int(Performance.get_monitor(Performance.OBJECT_COUNT) as int).is_equal(nb_obj_before + 7 + 7 + 2) 
