extends ORC_ProxyObject
class_name ORCTEST_TrojanProxy

func get_all_secondary_data_of_type(type : StringName) -> Array[ORC_SecondaryData]:
	var result : Array[ORC_SecondaryData] = []
	for secondary_data in primary_data.secondary_data_array:
		var data_type = secondary_data.get_script().get_global_name()
		if data_type == type:
			result.append(secondary_data)
	return result
