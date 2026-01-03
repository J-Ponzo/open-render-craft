@tool
extends Resource
class_name ORC_PSODef

# TODO extract from here ?
enum ERenderMode {
	Opaque,
	Transparent_Mix,
	Transparent_Add,
	Transparent_Subtract,
	Transparent_Multiply,
	Transparent_PremultAlpha,
	AlphaScissor,
	AlphaHash
}

# TODO this system is not enough because it does not support auto updating when the shader source files change on disk.
func _get_shader_raw_src(path : String) -> String:
	var file = FileAccess.open(path, FileAccess.READ)
	if file == null:
		return ""
	return file.get_as_text()

func update_vertex_shader_raw_src_from_path() -> void:
	vertex_shader_raw_src = _get_shader_raw_src(vertex_shader_path)

@export var vertex_shader_path : String:
	set(value):
		vertex_shader_path = value
		update_vertex_shader_raw_src_from_path()
var vertex_shader_raw_src : String

func update_fragment_shader_raw_src_from_path() -> void:
	fragment_shader_raw_src = _get_shader_raw_src(fragment_shader_path)

@export var fragment_shader_path : String:
	set(value):
		fragment_shader_path = value
		update_fragment_shader_raw_src_from_path()
var fragment_shader_raw_src : String

@export var vertex_format_def : ORC_VertexFormatDef
@export var defines : Array[StringName]
@export var rasterization_state : ORC_PSORasterisationDef = ORC_PSORasterisationDef.new()
@export var multisample_state : ORC_PSOMultisampleDef = ORC_PSOMultisampleDef.new()
@export var depth_stencil_state : ORC_PSODepthStencilDef = ORC_PSODepthStencilDef.new()
@export var blend_attachments : Array[ORC_PSOColorBlendAttachmentDef] = []
@export var blend_constant : Color = Color(0, 0, 0, 1)
@export var enable_logic_op : bool = false
@export var logic_op : RenderingDevice.LogicOperation = RenderingDevice.LogicOperation.LOGIC_OP_CLEAR
