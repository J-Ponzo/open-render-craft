extends Resource
class_name ORC_PSODef

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

@export var vertex_shader : String
@export var fragment_shader : String
