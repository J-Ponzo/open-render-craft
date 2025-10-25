extends GdUnitTestSuite

class MockScnProxy extends ORC_SceneProxyBase:
	static var pre_render_count : int = 0
	static var post_render_count : int = 0
	
	func pre_render_override() -> void:
		self.super_pre_render()
		pre_render_count += 1
		
	func post_render_override() -> void:
		post_render_count += 1

var scn_proxy : MockScnProxy
var scn_instance : Node
var nb_updates : int = 8

var nb_mesh_proxy = 3
var nb_cam_proxy = 1
var nb_omni_proxy = 1
var nb_spot_proxy = 1
var nb_directional_proxy = 1

func before() -> void:
	scn_proxy = MockScnProxy.new()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()
	scn_instance = ORCTEST_ScnProxyTestsCommon.load_mock_scene(get_tree())
	
	scn_proxy.setup(scn_instance)
	
	for i in range(nb_updates):
		scn_proxy.pre_render()
		scn_proxy.post_render()

func after() -> void:
	scn_proxy.proxy_factory = null
	scn_proxy = null
	scn_instance.queue_free()

func test_pre_render():
	var actual_pre_render_count = MockScnProxy.pre_render_count
	var expected_pre_render_count = nb_updates
	assert_int(actual_pre_render_count).is_equal(expected_pre_render_count)

func test_post_render():
	var actual_post_render_count = MockScnProxy.pre_render_count
	var expected_post_render_count = nb_updates
	assert_int(actual_post_render_count).is_equal(expected_post_render_count)

func test_update_all_proxy():	
	var nb_all_proxy = nb_mesh_proxy + nb_cam_proxy + nb_omni_proxy + nb_spot_proxy + nb_directional_proxy
	var actual_all_update_count = ORCTEST_ProxyFactory_GDMock.MockProxyObject.all_update_count
	var expected_all_update_count = nb_updates * nb_all_proxy
	assert_int(actual_all_update_count).is_equal(expected_all_update_count)

func test_update_mesh():	
	var actual_mesh_update_count = ORCTEST_ProxyFactory_GDMock.MeshProxy.mesh_update_count
	var expected_mesh_update_count = nb_updates * nb_mesh_proxy
	assert_int(actual_mesh_update_count).is_equal(expected_mesh_update_count)

func test_update_cam():
	var actual_cam_update_count = ORCTEST_ProxyFactory_GDMock.CameraProxy.cam_update_count
	var expected_cam_update_count = nb_updates * nb_cam_proxy
	assert_int(actual_cam_update_count).is_equal(expected_cam_update_count)

func test_update_omni():	
	var actual_omni_update_count = ORCTEST_ProxyFactory_GDMock.OmniLightProxy.omni_update_count
	var expected_omni_update_count = nb_updates * nb_omni_proxy
	assert_int(actual_omni_update_count).is_equal(expected_omni_update_count)

func test_update_spot():
	var actual_spot_update_count = ORCTEST_ProxyFactory_GDMock.SpotLightProxy.spot_update_count
	var expected_spot_update_count = nb_updates * nb_spot_proxy
	assert_int(actual_spot_update_count).is_equal(expected_spot_update_count)

func test_update_directional():
	var actual_directional_update_count = ORCTEST_ProxyFactory_GDMock.DirectionalLightProxy.directional_update_count
	var expected_directional_update_count = nb_updates * nb_directional_proxy
	assert_int(actual_directional_update_count).is_equal(expected_directional_update_count)
