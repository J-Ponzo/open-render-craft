class_name ORCTEST_ScnProxyTestsCommon

static func load_mock_scene(scn_tree : SceneTree) -> Node:
	var scene = load("res://addons/open-render-craft/tests/proxy/mocks/proxy_mock_scene.tscn") as PackedScene
	assert(scene != null, "load_mock_scene() : cannot load mock scene")
	var instance = scene.instantiate()
	assert(instance != null, "load_mock_scene() : cannot instantiate mock scene")
	scn_tree.root.add_child(instance)
	return instance

static func wait_for_stabilisation(scn_tree : SceneTree) -> void:	
	var prev_obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	var max_iter = 32
	var obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)
	while (max_iter < 32 && prev_obj_count != obj_count):
		await scn_tree.process_frame
		prev_obj_count = obj_count
		obj_count = Performance.get_monitor(Performance.OBJECT_COUNT)

static func create_triangle_mesh() -> ArrayMesh:
	var mesh := ArrayMesh.new()
	var arrays := []
	arrays.resize(Mesh.ARRAY_MAX)
	arrays[Mesh.ARRAY_VERTEX] = PackedVector3Array([Vector3(0,0,0), Vector3(1,0,0), Vector3(0,1,0)])
	arrays[Mesh.ARRAY_INDEX] = PackedInt32Array([0,1,2])
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arrays)
	return mesh
