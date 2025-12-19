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
			primary_data = create_and_register_primary(ORCTEST_Trojan_C_Data, registry)
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

func test_simple_pre_cascade():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)

	var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
	var trojan_1_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var cumuled_flag_tests : bool = true

	# Check no flags at start
	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_1_SRC")

	# Check cascad pre set flags
	trojan_a_data.set_flag("TROJAN_A_TRG", true)
	trojan_1_data.set_flag("TROJAN_1_SRC", true)

	trojan_a_data.register_flag_sources([trojan_1_data])

	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_1_SRC")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1_data.has_flag("TROJAN_1_SRC")

	# Check cascade post unset flags
	trojan_a_data.set_flag("TROJAN_A_TRG", false)
	trojan_1_data.set_flag("TROJAN_1_SRC", false)

	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_1_SRC")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_1_SRC")

	assert_bool(cumuled_flag_tests).is_equal(true)

func test_simple_post_cascade():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)

	var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
	var trojan_1_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var cumuled_flag_tests : bool = true

	# Check no flags at start
	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_1_SRC")

	# Check cascade post set flags
	trojan_a_data.register_flag_sources([trojan_1_data])

	trojan_a_data.set_flag("TROJAN_A_TRG", true)
	trojan_1_data.set_flag("TROJAN_1_SRC", true)

	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_1_SRC")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1_data.has_flag("TROJAN_1_SRC")

	# Check cascade post unset flags
	trojan_a_data.set_flag("TROJAN_A_TRG", false)
	trojan_1_data.set_flag("TROJAN_1_SRC", false)

	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_A_TRG")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_a_data.has_flag("TROJAN_1_SRC")
	cumuled_flag_tests = cumuled_flag_tests && !trojan_1_data.has_flag("TROJAN_1_SRC")

	assert_bool(cumuled_flag_tests).is_equal(true)

func test_multi_trg_cascade():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)

	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)

	var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
	var trojan_1a_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
	var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data
	var cumuled_flag_tests : bool = true

	trojan_a_data.register_flag_sources([trojan_1a_data])
	trojan_b_data.register_flag_sources([trojan_1a_data])
	trojan_b_data.register_flag_sources([trojan_2b_data])
	trojan_1b_data.register_flag_sources([trojan_2b_data])
	trojan_a_data.register_flag_sources([trojan_b_data])
	trojan_1b_data.register_flag_sources([trojan_b_data])

	trojan_a_data.set_flag("TROJAN_A", true)
	trojan_1a_data.set_flag("TROJAN_1A", true)
	trojan_b_data.set_flag("TROJAN_B", true)
	trojan_1b_data.set_flag("TROJAN_1B", true)
	trojan_2b_data.set_flag("TROJAN_2B", true)

	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_A")
	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_1A")
	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_2B")

	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_1A")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_2B")

	cumuled_flag_tests = cumuled_flag_tests && trojan_1a_data.has_flag("TROJAN_1A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_1B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_2B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_1A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_2B")

	assert_bool(cumuled_flag_tests).is_equal(true)

func test_long_cascade():
	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)

	var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
	scn_instance.add_child(trojan_c)

	var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
	var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data
	var trojan_c_data : ORCTEST_Trojan_C_Data = trojan_c.trojan_proxy.get_primary_data() as ORCTEST_Trojan_C_Data
	var trojan_3c_1_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0] as ORCTEST_Trojan_3_Data
	var trojan_3c_2_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1] as ORCTEST_Trojan_3_Data
	var cumuled_flag_tests : bool = true
	
	trojan_1b_data.register_flag_sources([trojan_2b_data])
	trojan_2b_data.register_flag_sources([trojan_b_data])
	trojan_b_data.register_flag_sources([trojan_c_data])
	trojan_c_data.register_flag_sources([trojan_3c_2_data])

	trojan_b_data.set_flag("TROJAN_B", true)
	trojan_1b_data.set_flag("TROJAN_1B", true)
	trojan_2b_data.set_flag("TROJAN_2B", true)
	trojan_c_data.set_flag("TROJAN_C", true)
	trojan_3c_1_data.set_flag("TROJAN_3C_1", true)
	trojan_3c_2_data.set_flag("TROJAN_3C_2", true)

	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_1B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_2B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_C")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_3C_2")

	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_2B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_C")
	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_3C_2")

	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_C")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_3C_2")

	cumuled_flag_tests = cumuled_flag_tests && trojan_c_data.has_flag("TROJAN_C")
	cumuled_flag_tests = cumuled_flag_tests && trojan_c_data.has_flag("TROJAN_3C_2")

	cumuled_flag_tests = cumuled_flag_tests && trojan_3c_1_data.has_flag("TROJAN_3C_1")

	cumuled_flag_tests = cumuled_flag_tests && trojan_3c_2_data.has_flag("TROJAN_3C_2")

	assert_bool(cumuled_flag_tests).is_equal(true)

func test_diamond_cascade():
	var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
	scn_instance.add_child(trojan_a)

	var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
	scn_instance.add_child(trojan_b)

	var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
	var trojan_1a_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
	var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
	var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data
	var cumuled_flag_tests : bool = true

	trojan_b_data.register_flag_sources([trojan_1b_data, trojan_2b_data])
	trojan_1b_data.register_flag_sources([trojan_a_data])
	trojan_2b_data.register_flag_sources([trojan_a_data])

	trojan_1b_data.register_flag_sources([trojan_2b_data])
	trojan_a_data.register_flag_sources([trojan_b_data])
	trojan_1b_data.register_flag_sources([trojan_b_data])

	trojan_a_data.set_flag("TROJAN_A", true)
	trojan_1a_data.set_flag("TROJAN_1A", true)
	trojan_b_data.set_flag("TROJAN_B", true)
	trojan_1b_data.set_flag("TROJAN_1B", true)
	trojan_2b_data.set_flag("TROJAN_2B", true)

	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_1B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_2B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_b_data.has_flag("TROJAN_A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_1B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_1b_data.has_flag("TROJAN_A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_2B")
	cumuled_flag_tests = cumuled_flag_tests && trojan_2b_data.has_flag("TROJAN_A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_a_data.has_flag("TROJAN_A")

	cumuled_flag_tests = cumuled_flag_tests && trojan_1a_data.has_flag("TROJAN_1A")

	assert_bool(cumuled_flag_tests).is_equal(true)

func test_break_exclusive_flags_rule():
	await assert_error(func() :
		var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
		scn_instance.add_child(trojan_a)

		var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
		var trojan_1_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data

		trojan_a_data.set_flag("FLAG", true)
		trojan_1_data.set_flag("FLAG", true)
	).is_push_error("Bla bla")

func test_break_type_unicity_in_cascade_sibling():
	await assert_error(func() :
		var trojan_a : ORCTEST_Trojan_A = ORCTEST_Trojan_A.new()
		scn_instance.add_child(trojan_a)

		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_a_data : ORCTEST_Trojan_A_Data = trojan_a.trojan_proxy.get_primary_data() as ORCTEST_Trojan_A_Data
		var trojan_1a_data : ORCTEST_Trojan_1_Data = trojan_a.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_a_data.register_flag_sources([trojan_1a_data])
		trojan_a_data.register_flag_sources([trojan_1b_data])
	).is_push_error("Inconsistent flag cascade: Type unicity rule is broken")

func test_break_type_unicity_in_cascade_child():
	await assert_error(func() :
		var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
		scn_instance.add_child(trojan_c)

		var trojan_c_data : ORCTEST_Trojan_C_Data = trojan_c.trojan_proxy.get_primary_data() as ORCTEST_Trojan_C_Data
		var trojan_3c_1_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0] as ORCTEST_Trojan_3_Data
		var trojan_3c_2_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1] as ORCTEST_Trojan_3_Data
		
		trojan_3c_1_data.register_flag_sources([trojan_3c_2_data])
	).is_push_error("Inconsistent flag cascade: Type unicity rule is broken")

func test_break_type_unicity_in_cascade_grand_child():
	await assert_error(func() :
		var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
		scn_instance.add_child(trojan_c)

		var trojan_c_data : ORCTEST_Trojan_C_Data = trojan_c.trojan_proxy.get_primary_data() as ORCTEST_Trojan_C_Data
		var trojan_3c_1_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0] as ORCTEST_Trojan_3_Data
		var trojan_3c_2_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1] as ORCTEST_Trojan_3_Data
		
		trojan_3c_1_data.register_flag_sources([trojan_c_data])
		trojan_c_data.register_flag_sources([trojan_3c_2_data])
	).is_push_error("Inconsistent flag cascade: Type unicity rule is broken")

func test_break_type_unicity_in_cascade_grand_child_inverted():
	await assert_error(func() :
		var trojan_c : ORCTEST_Trojan_C = ORCTEST_Trojan_C.new()
		scn_instance.add_child(trojan_c)

		var trojan_c_data : ORCTEST_Trojan_C_Data = trojan_c.trojan_proxy.get_primary_data() as ORCTEST_Trojan_C_Data
		var trojan_3c_1_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[0] as ORCTEST_Trojan_3_Data
		var trojan_3c_2_data : ORCTEST_Trojan_3_Data = trojan_c.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_3_Data_CLASS)[1] as ORCTEST_Trojan_3_Data
		
		trojan_c_data.register_flag_sources([trojan_3c_2_data])
		trojan_3c_1_data.register_flag_sources([trojan_c_data])
	).is_push_error("Inconsistent flag cascade: Type unicity rule is broken")

func test_break_no_cycles_in_cascade_1():
	await assert_error(func() :
		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_b_data.register_flag_sources([trojan_b_data])
	).is_push_error("Inconsistent flag cascade: No cycles rule is broken")

func test_break_no_cycles_in_cascade_2():
	await assert_error(func() :
		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_1b_data.register_flag_sources([trojan_b_data])
		trojan_b_data.register_flag_sources([trojan_1b_data])
	).is_push_error("Inconsistent flag cascade: No cycles rule is broken")

func test_break_no_cycles_in_cascade_2_invert():
	await assert_error(func() :
		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_b_data.register_flag_sources([trojan_1b_data])
		trojan_1b_data.register_flag_sources([trojan_b_data])
	).is_push_error("Inconsistent flag cascade: No cycles rule is broken")

func test_break_no_cycles_in_cascade_3():
	await assert_error(func() :
		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_b_data.register_flag_sources([trojan_1b_data])
		trojan_1b_data.register_flag_sources([trojan_2b_data])
		trojan_2b_data.register_flag_sources([trojan_b_data])
	).is_push_error("Inconsistent flag cascade: No cycles rule is broken")

func test_break_no_cycles_in_cascade_3_invert():
	await assert_error(func() :
		var trojan_b : ORCTEST_Trojan_B = ORCTEST_Trojan_B.new()
		scn_instance.add_child(trojan_b)

		var trojan_b_data : ORCTEST_Trojan_B_Data = trojan_b.trojan_proxy.get_primary_data() as ORCTEST_Trojan_B_Data
		var trojan_1b_data : ORCTEST_Trojan_1_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_1_Data_CLASS)[0] as ORCTEST_Trojan_1_Data
		var trojan_2b_data : ORCTEST_Trojan_2_Data = trojan_b.trojan_proxy.get_all_secondary_data_of_type(ORCTEST_Trojan_2_Data_CLASS)[0] as ORCTEST_Trojan_2_Data

		trojan_2b_data.register_flag_sources([trojan_b_data])
		trojan_1b_data.register_flag_sources([trojan_2b_data])
		trojan_b_data.register_flag_sources([trojan_1b_data])
	).is_push_error("Inconsistent flag cascade: No cycles rule is broken")
