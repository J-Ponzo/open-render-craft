extends ORCTEST_ScnProxyRemoveShared

func before() -> void:
	super.common_before()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()

func after() -> void:
	super.common_after()

func test_remove_shared():
	await super.common_remove_shared()
