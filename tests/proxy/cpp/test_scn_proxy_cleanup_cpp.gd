extends ORCTEST_ScnProxyCleanup

func before() -> void:
	super.common_before()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_CPPMock.new()

func after() -> void:
	super.common_after()

func test_cleanup_scene():
	await super.common_cleanup_scene()
