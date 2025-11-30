extends ORCTEST_ScnProxyAddUnique

func before() -> void:
	super.common_before()
	scn_proxy.proxy_factory = ORCTEST_ProxyFactory_GDMock.new()

func after() -> void:
	super.common_after()

func test_add_unique():
	await super.common_add_unique()
