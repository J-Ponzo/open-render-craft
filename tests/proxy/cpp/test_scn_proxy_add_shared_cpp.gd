extends GdUnitTestSuite

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func before() -> void:
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_CPPMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())

func after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_add_shared():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	scn_proxy.setup(scn_instance)

	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	var shared_mesh := ORCTEST_ScnProxyTestsCommon.create_triangle_mesh()

	var mesh_instance1 := MeshInstance3D.new()
	mesh_instance1.mesh = shared_mesh
	scn_instance.add_child(mesh_instance1)

	var mesh_instance2 := MeshInstance3D.new()
	mesh_instance2.mesh = shared_mesh
	scn_instance.add_child(mesh_instance2)

	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var nb_mesh_inst_added = 2
	var nb_mesh_added = 1			# The Mesh is shared
	var nb_proxy_objs_added = 2
	var nb_primary_data_added = 2
	var nb_secondary_data_added = 1 # The TopologyData is shared
	var additional_expected = nb_mesh_inst_added + nb_mesh_added + nb_proxy_objs_added + nb_primary_data_added + nb_secondary_data_added
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before + additional_expected)
