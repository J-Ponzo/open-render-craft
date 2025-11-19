extends Resource
class_name ORC_ProxyQueue_Def

@export var queue_name : StringName
@export var parent_queue_name : StringName = ""
@export var processors : Array[ORC_AImpl_Def] = []
