extends Resource
class_name ORC_RenderPassDef

@export var pass_name : StringName
@export var pass_impl : ORC_AImpl_Def
@export var fb_format_defs : Dictionary[StringName, ORC_FramebufferFormat_Def]
@export var direct_pso_defs : Dictionary[StringName, ORC_PSODef]
@export var pso_factory_defs : Dictionary[StringName, ORC_PSOFactoryDef]
