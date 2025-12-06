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

@export var vertex_shader_path : String
@export var fragment_shader_path : String
@export var vertex_format_def : ORC_VertexFormatDef
@export var cull_mode : RenderingDevice.PolygonCullMode
@export var render_mode : ERenderMode
@export var defines : Array[StringName]
@export var rasterization_state : ORC_PSORasterisationDef
@export var multisample_state : ORC_PSOMultisampleDef
@export var depth_stencil_state : ORC_PSODepthStencilDef
@export var blend_attachments : Array[ORC_PSOColorBlendAttachmentDef] = []
@export var blend_constant : Color = Color(0, 0, 0, 1)
@export var enable_logic_op : bool = false
@export var logic_op : RenderingDevice.LogicOperation = RenderingDevice.LogicOperation.LOGIC_OP_CLEAR
