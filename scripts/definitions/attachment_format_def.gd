extends Resource
class_name ORC_AttachmentFormat_Def

# TODO : Rename to ORC_TextureFormat_Def or ORC_TextureAttachmentFormat_Def ? because RDAttachmentFormat only has format, usage_flags and samples. width, height, type and array_layers belong to RDTextureFormat
@export var attachment_name : StringName
@export var format : RenderingDevice.DataFormat
@export var usage_flags : Array[RenderingDevice.TextureUsageBits]
@export var samples : RenderingDevice.TextureSamples
@export var width : int = -1
@export var height : int = -1
@export var texture_type : RenderingDevice.TextureType = RenderingDevice.TEXTURE_TYPE_2D
@export var array_layers : int = 1