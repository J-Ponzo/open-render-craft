extends GdUnitTestSuite

var mock_obj : ORCTEST_GDOverridableMacro_GDDerivedMock
func before() -> void:
	mock_obj = ORCTEST_GDOverridableMacro_GDDerivedMock.new()
	
func test_macro_super_proc_0_args():
	mock_obj.super_proc_0_args()
	assert_int(mock_obj.internal_state).is_equal(1)
	
func test_macro_proc_0_args():
	mock_obj.proc_0_args()
	assert_int(mock_obj.internal_state).is_equal(-1)

func test_macro_super_proc_1_args():
	mock_obj.super_proc_1_args(1)
	assert_int(mock_obj.internal_state).is_equal(2)
	
func test_macro_proc_1_args():
	mock_obj.proc_1_args(1)
	assert_int(mock_obj.internal_state).is_equal(-2)
	
func test_macro_super_proc_2_args():
	mock_obj.super_proc_2_args(1, 1)
	assert_int(mock_obj.internal_state).is_equal(3)
	
func test_macro_proc_2_args():
	mock_obj.proc_2_args(1, 1)
	assert_int(mock_obj.internal_state).is_equal(-3)
	
func test_macro_super_proc_3_args():
	mock_obj.super_proc_3_args(1, 1, 1)
	assert_int(mock_obj.internal_state).is_equal(4)
	
func test_macro_proc_3_args():
	mock_obj.proc_3_args(1, 1, 1)
	assert_int(mock_obj.internal_state).is_equal(-4)
	
func test_macro_super_proc_4_args():
	mock_obj.super_proc_4_args(1, 1, 1, 1)
	assert_int(mock_obj.internal_state).is_equal(5)
	
func test_macro_proc_4_args():
	mock_obj.proc_4_args(1, 1, 1, 1)
	assert_int(mock_obj.internal_state).is_equal(-5)

func test_macro_super_func_0_args():
	assert_int(mock_obj.super_func_0_args()).is_equal(1)
	
func test_macro_func_0_args():
	assert_int(mock_obj.func_0_args()).is_equal(-1)

func test_macro_super_func_1_args():
	assert_int(mock_obj.super_func_1_args(1)).is_equal(2)
	
func test_macro_func_1_args():
	assert_int(mock_obj.func_1_args(1)).is_equal(-2)
	
func test_macro_super_func_2_args():
	assert_int(mock_obj.super_func_2_args(1, 1)).is_equal(3)
	
func test_macro_func_2_args():
	assert_int(mock_obj.func_2_args(1, 1)).is_equal(-3)
	
func test_macro_super_func_3_args():
	assert_int(mock_obj.super_func_3_args(1, 1, 1)).is_equal(4)
	
func test_macro_func_3_args():
	assert_int(mock_obj.func_3_args(1, 1, 1)).is_equal(-4)
	
func test_macro_super_func_4_args():
	assert_int(mock_obj.super_func_4_args(1, 1, 1, 1)).is_equal(5)
	
func test_macro_func_4_args():
	assert_int(mock_obj.func_4_args(1, 1, 1, 1)).is_equal(-5)
