extends ORCTEST_ScnProxySetup

func before() -> void:
	super.common_before()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()

func after() -> void:
	super.common_after()

func test_setup_scene():
	await super.common_setup_scene()
