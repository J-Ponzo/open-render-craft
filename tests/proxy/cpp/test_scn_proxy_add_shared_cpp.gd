extends ORCTEST_ScnProxyAddShared

func before() -> void:
	super.common_before()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_CPPMock.new()

func after() -> void:
	super.common_after()

func test_add_shared():
	await super.common_add_shared()
