extends Resource
class_name ORC_RenderPassDef

@export var pass_name : StringName
@export var pass_impl : ORC_AImpl_Def
@export var fb_format_def : ORC_FramebufferFormat_Def
@export var direct_pso_defs : Dictionary[StringName, ORC_DirectPSODef]
@export var uber_pso_defs : Dictionary[StringName, ORC_UberPSODef]
