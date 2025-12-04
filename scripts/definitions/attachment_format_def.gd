extends Resource
class_name ORC_AttachmentFormat_Def

@export var attachment_name : StringName
@export var format : RenderingDevice.DataFormat
@export var usage_flags : Array[RenderingDevice.TextureUsageBits]
@export var samples : RenderingDevice.TextureSamples
@export var width : int = -1
@export var height : int = -1
