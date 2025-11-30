extends GdUnitTestSuite
class_name ORCTEST_ScnProxyRemoveSharedLast

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

func common_before() -> void:
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	scn_proxy = ORC_SceneProxyBase.new()

func common_after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func common_remove_shared_last():
	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())
	
	scn_proxy.setup(scn_instance)

	var shared_mesh := ORCTEST_ScnProxyTestsCommon.create_triangle_mesh()

	var mesh_instance1 := MeshInstance3D.new()
	mesh_instance1.mesh = shared_mesh
	scn_instance.add_child(mesh_instance1)

	var mesh_instance2 := MeshInstance3D.new()
	mesh_instance2.mesh = shared_mesh
	scn_instance.add_child(mesh_instance2)

	mesh_instance1.free()

	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var nb_obj_before : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	mesh_instance2.free()

	await ORCTEST_ScnProxyTestsCommon.wait_for_stabilisation(get_tree())

	var nb_mesh_inst_removed = 1
	var nb_mesh_removed = 0			# The shared_mesh var still exists
	var nb_proxy_objs_removed = 1
	var nb_primary_data_removed = 1
	var nb_secondary_data_removed = 1 # The TopologyData was the last shared
	var removed_expected = nb_mesh_inst_removed + nb_mesh_removed + nb_proxy_objs_removed + nb_primary_data_removed + nb_secondary_data_removed
	var actual_total : int = Performance.get_monitor(Performance.OBJECT_COUNT)
	assert_int(actual_total).is_equal(nb_obj_before - removed_expected)
