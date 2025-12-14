extends GdUnitTestSuite
class_name ORCTEST_ScnProxyFeatureFlags

var scn_proxy : ORC_SceneProxyBase
var scn_instance : Node

const ORCTEST_Trojan_A_Data_CLASS = "ORCTEST_Trojan_A_Data"
const ORCTEST_Trojan_B_Data_CLASS = "ORCTEST_Trojan_B_Data"
const ORCTEST_Trojan_C_Data_CLASS = "ORCTEST_Trojan_C_Data"
const ORCTEST_Trojan_1_Data_CLASS = "ORCTEST_Trojan_1_Data"
const ORCTEST_Trojan_2_Data_CLASS = "ORCTEST_Trojan_2_Data"
const ORCTEST_Trojan_3_Data_CLASS = "ORCTEST_Trojan_3_Data"

class ORCTEST_MyProxyFactory extends ORC_ProxyFactory:
	func create_proxy_from_override(node : Node) -> ORC_ProxyObject:
		var proxy_object : ORC_ProxyObject = null
		if node is ORCTEST_Trojan_A || node is ORCTEST_Trojan_B || node is ORCTEST_Trojan_C:
			proxy_object = ORCTEST_TrojanProxy.new()
			node.trojan_proxy = proxy_object as ORCTEST_TrojanProxy
		return proxy_object
	
	func create_data_from_override(node : Node, registry : ORC_ProxyRegistry) -> ORC_PrimaryData:
		var primary_data : ORC_PrimaryData = null
		var secondary_data : ORC_SecondaryData = null
		if node is ORCTEST_Trojan_A:
			primary_data = create_and_register_primary(ORCTEST_Trojan_A_Data, registry)
			primary_data.set_flag("TROJAN_A_INIT", true)
			secondary_data = create_and_register_secondary(ORCTEST_Trojan_1_Data, registry, primary_data)
			secondary_data.set_flag("TROJAN_1_INIT", true)
		elif node is ORCTEST_Trojan_B:
			primary_data = create_and_register_primary(ORCTEST_Trojan_B_Data, registry)
			primary_data.set_flag("TROJAN_B_INIT", true)
			secondary_data = create_and_register_secondary(ORCTEST_Trojan_1_Data, registry, primary_data)
			secondary_data.set_flag("TROJAN_1_INIT", true)
			secondary_data = create_and_register_secondary(ORCTEST_Trojan_2_Data, registry, primary_data)
			secondary_data.set_flag("TROJAN_2_INIT", true)
		elif node is ORCTEST_Trojan_C:
			primary_data = create_and_register_primary(ORCTEST_Trojan_A_Data, registry)
			primary_data.set_flag("TROJAN_C_INIT", true)
			secondary_data = create_and_register_secondary(ORCTEST_Trojan_3_Data, registry, primary_data)
			secondary_data.set_flag("TROJAN_3_INIT", true)
			secondary_data = create_and_register_secondary(ORCTEST_Trojan_3_Data, registry, primary_data)
			secondary_data.set_flag("TROJAN_3_INIT", true)
		return primary_data

func before() -> void:
	scn_instance = Node.new()
	get_tree().root.add_child(scn_instance)
	scn_proxy = ORC_SceneProxyBase.new()
	scn_proxy.proxy_factory = ORCTEST_MyProxyFactory.new()
	scn_proxy.setup(scn_instance)

func after() -> void:
	scn_instance.queue_free()
	scn_proxy.proxy_factory = null
	scn_proxy = null

func test_init_flags():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)

	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)

	var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
	scn_instance.add_child(trojan_c)

	var cumuled_has_flag : bool = true

	cumuled_has_flag = cumuled_has_flag && trojan_a.trojan_proxy.get_primary_data().has_flag("TROJAN_A_INIT")
	cumuled_has_flag = cumuled_has_flag && trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_INIT")

	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_primary_data().has_flag("TROJAN_B_INIT")
	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_INIT")
	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0].has_flag("TROJAN_2_INIT")
	
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_primary_data().has_flag("TROJAN_C_INIT") 
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0].has_flag("TROJAN_3_INIT")
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1].has_flag("TROJAN_3_INIT")
	assert_bool(cumuled_has_flag).is_equal(true)

func test_set_flags():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)
	trojan_a.trojan_proxy.get_primary_data().set_flag("TROJAN_A_SET", true)
	trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].set_flag("TROJAN_1_SET", true)

	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)
	trojan_b.trojan_proxy.get_primary_data().set_flag("TROJAN_B_SET", true)
	trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].set_flag("TROJAN_1_SET", true)
	trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0].set_flag("TROJAN_2_SET", true)

	var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
	scn_instance.add_child(trojan_c)
	trojan_c.trojan_proxy.get_primary_data().set_flag("TROJAN_C_SET", true)
	trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0].set_flag("TROJAN_3_SET", true)
	trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1].set_flag("TROJAN_3_SET", true)

	var cumuled_has_flag : bool = true
	cumuled_has_flag = cumuled_has_flag && trojan_a.trojan_proxy.get_primary_data().has_flag("TROJAN_A_SET")
	cumuled_has_flag = cumuled_has_flag && trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_SET")

	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_primary_data().has_flag("TROJAN_B_SET")
	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_SET")
	cumuled_has_flag = cumuled_has_flag && trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0].has_flag("TROJAN_2_SET")
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_primary_data().has_flag("TROJAN_C_SET") 
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0].has_flag("TROJAN_3_SET")
	cumuled_has_flag = cumuled_has_flag && trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1].has_flag("TROJAN_3_SET")
	assert_bool(cumuled_has_flag).is_equal(true)

func test_unset_flags():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)
	trojan_a.trojan_proxy.get_primary_data().set_flag("TROJAN_A_INIT", false)
	trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].set_flag("TROJAN_1_INIT", false)

	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)
	trojan_b.trojan_proxy.get_primary_data().set_flag("TROJAN_B_INIT", false)
	trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].set_flag("TROJAN_1_INIT", false)
	trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0].set_flag("TROJAN_2_INIT", false)

	var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
	scn_instance.add_child(trojan_c)
	trojan_c.trojan_proxy.get_primary_data().set_flag("TROJAN_C_INIT", false)
	trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0].set_flag("TROJAN_3_INIT", false)
	trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1].set_flag("TROJAN_3_INIT", false)

	var cumuled_has_flag : bool = true

	cumuled_has_flag = cumuled_has_flag && !trojan_a.trojan_proxy.get_primary_data().has_flag("TROJAN_A_INIT")
	cumuled_has_flag = cumuled_has_flag && !trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_INIT")

	cumuled_has_flag = cumuled_has_flag && !trojan_b.trojan_proxy.get_primary_data().has_flag("TROJAN_B_INIT")
	cumuled_has_flag = cumuled_has_flag && !trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0].has_flag("TROJAN_1_INIT")
	cumuled_has_flag = cumuled_has_flag && !trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0].has_flag("TROJAN_2_INIT")
	
	cumuled_has_flag = cumuled_has_flag && !trojan_c.trojan_proxy.get_primary_data().has_flag("TROJAN_C_INIT") 
	cumuled_has_flag = cumuled_has_flag && !trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0].has_flag("TROJAN_3_INIT")
	cumuled_has_flag = cumuled_has_flag && !trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1].has_flag("TROJAN_3_INIT")
	assert_bool(cumuled_has_flag).is_equal(true)