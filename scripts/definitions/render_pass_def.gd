extends Resource
class_name ORC_RenderPassDef

@export var pass_name : StringName
@export var pass_impl : ORC_AImpl_Def
@export var fb_format_def : ORC_FramebufferFormat_Def
@export var explicite_pso_defs : Dictionary[StringName, ORC_ExpicitPSODef]
@export var generated_pso_defs : Dictionary[StringName, ORC_GeneratedPSODef]
