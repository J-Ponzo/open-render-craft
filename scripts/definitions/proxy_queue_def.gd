extends Resource
class_name ORC_ProxyQueue_Def

@export var queue_name : StringName
@export var init_query : ORC_DataQuery_Def
@export var processors : Array[ORC_AImpl_Def] = []
