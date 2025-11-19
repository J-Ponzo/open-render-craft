extends Resource
class_name ORC_Renderer_Def

@export var renderer_impl : ORC_AImpl_Def
@export var proxy_factory_impl : ORC_AImpl_Def
@export var renderer_pass_defs : Dictionary[StringName, ORC_RenderPassDef]
@export var attachment_format_defs : Dictionary[StringName, ORC_AttachmentFormat_Def]
@export var proxy_queue_defs : Array[ORC_ProxyQueue_Def] = []
