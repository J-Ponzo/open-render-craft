class_name ORCTEST_ScnProxyTestsCommon

static func reset_proxy_update_counters_gd() -> void:
	ORCTEST_MockProxyObject.all_update_count = 0
	ORCTEST_MeshProxy.mesh_update_count = 0
	ORCTEST_CameraProxy.cam_update_count = 0
	ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightProxy.omni_update_count = 0
	ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightProxy.spot_update_count = 0
	ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightProxy.directional_update_count = 0

static func reset_proxy_update_counters_cpp() -> void:
	ORCTEST_MockProxyObject_CPP.reset_update_count()
	ORCTEST_CameraProxy_CPP.reset_cam_update_count()
	ORCTEST_MeshProxy_CPP.reset_mesh_update_count()
	ORCTEST_OmniLightProxy_CPP.reset_omni_update_count()
	ORCTEST_SpotLightProxy_CPP.reset_spot_update_count()
	ORCTEST_DirectionalLightProxy_CPP.reset_directional_update_count()

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
