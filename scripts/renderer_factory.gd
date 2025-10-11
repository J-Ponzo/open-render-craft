class_name ORC_RendererFactory

static func create_renderer(renderer_def : ORC_Renderer_Def) -> ORC_RendererBase:
	var renderer_inst : ORC_RendererBase = ClassDB.instantiate("ORC_RendererBase") as ORC_RendererBase
	var scn_proxy_inst : ORC_SceneProxyBase = ClassDB.instantiate("ORC_SceneProxyBase") as ORC_SceneProxyBase
	renderer_inst.scene_proxy = scn_proxy_inst
	return renderer_inst
