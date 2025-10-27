class_name ORC_RendererFactory

static var rd = RenderingServer.get_rendering_device()

static func create_renderer(renderer_def : ORC_Renderer_Def) -> ORC_RendererBase:
	var renderer_inst : ORC_RendererBase = ORC_ImplFactory.create_impl(renderer_def.renderer_impl) as ORC_RendererBase
	var scn_proxy_inst : ORC_SceneProxyBase = ORC_ImplFactory.create_impl(renderer_def.scene_proxy_impl) as ORC_SceneProxyBase
	var proxy_factory_inst : ORC_ProxyFactory = ORC_ImplFactory.create_impl(renderer_def.proxy_factory_impl) as ORC_ProxyFactory
	renderer_inst.scene_proxy = scn_proxy_inst
	scn_proxy_inst.renderer = renderer_inst
	scn_proxy_inst.proxy_factory = proxy_factory_inst
	
	for key : StringName in renderer_def.renderer_pass_defs.keys():
		create_render_pass(renderer_inst, key, renderer_def.renderer_pass_defs[key])
	
	for key : StringName in renderer_def.renderer_pass_defs.keys():
		create_attachment(renderer_inst, key, renderer_def.attachment_format_defs[key])
	
	return renderer_inst

static func create_attachment(renderer_inst : ORC_RendererBase, attach_key : StringName, attach_format_def : ORC_AttachmentFormat_Def) -> RID:
	var attachment : RID = create_texture_attachment(attach_format_def)
	renderer_inst.attachments[attach_key] = attachment
	return attachment

static func create_texture_attachment(attach_format_def : ORC_AttachmentFormat_Def) -> RID:
	var width : int = attach_format_def.width
	var height : int = attach_format_def.height
	if width == -1:
		width = ProjectSettings.get_setting("display/window/size/viewport_width")
	if height == -1:
		height = ProjectSettings.get_setting("display/window/size/viewport_height")
	
	var tf = RDTextureFormat.new();
	tf.usage_bits = 0
	for bit in attach_format_def.usage_flags:
		tf.usage_bits |= bit
	tf.width = width
	tf.height = height
	tf.format = attach_format_def.format
	var view = RDTextureView.new();

	return rd.texture_create(tf, view)

static func create_render_pass(renderer_inst : ORC_RendererBase, render_pass_key : StringName, render_pass_def : ORC_RenderPassDef) -> ORC_RenderPassBase:
	var render_pass_inst = ORC_ImplFactory.create_impl(render_pass_def.pass_impl) as ORC_RenderPassBase
	if render_pass_inst == null:
		return null
		
	render_pass_inst.renderer = renderer_inst
	renderer_inst.render_passes[render_pass_key] = render_pass_inst

	return render_pass_inst
