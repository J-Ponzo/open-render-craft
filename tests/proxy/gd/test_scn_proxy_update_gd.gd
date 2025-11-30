extends ORCTEST_ScnProxyUpdate


func before() -> void:
	super.common_before()

func after() -> void:
	super.common_after()

func create_proxy_factory() -> ORC_ProxyFactory:
	return ORCTEST_ProxyFactory_GDMock.new()

func reset_proxy_update_counters() -> void:
	ORCTEST_ScnProxyTestsCommon.reset_proxy_update_counters_gd()

func get_all_update_count() -> int:
	return ORCTEST_MockProxyObject.all_update_count

func get_mesh_update_count() -> int:
	return ORCTEST_MeshProxy.mesh_update_count

func get_cam_update_count() -> int:
	return ORCTEST_CameraProxy.cam_update_count

func get_omni_update_count() -> int:
	return ORCTEST_ProxyFactory_GDMock.ORCTEST_OmniLightProxy.omni_update_count

func get_spot_update_count() -> int:
	return ORCTEST_ProxyFactory_GDMock.ORCTEST_SpotLightProxy.spot_update_count

func get_directional_update_count() -> int:
	return ORCTEST_ProxyFactory_GDMock.ORCTEST_DirectionalLightProxy.directional_update_count

func test_update_all_proxy():
	await super.common_update_all_proxy()

func test_update_mesh():
	await super.common_update_mesh()

func test_update_cam():
	await super.common_update_cam()

func test_update_omni():
	await super.common_update_omni()

func test_update_spot():
	await super.common_update_spot()

func test_update_directional():
	await super.common_update_directional()
