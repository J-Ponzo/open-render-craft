extends Resource
class_name ORC_Renderer_Def

@export var renderer_impl : ORC_AImpl_Def
@export var proxy_factory_impl : ORC_AImpl_Def
@export var renderer_pass_defs : Dictionary[StringName, ORC_RenderPassDef]
@export var attachment_format_defs : Dictionary[StringName, ORC_AttachmentFormat_Def]
#@export var feature_flag_manager_def : TL_FeatureFlagManager_Def
#@export var proxy_queues_manager_def : TL_ProxyQueueManagerDef
#@export var sort_key_manager_def : TL_SortKeyManagerDef
