class_name ORC_RendererFactory

static func create_renderer(renderer_def : ORC_Renderer_Def) -> ORC_RendererBase:
	var renderer_inst : ORC_RendererBase = ORC_ImplFactory.create_impl(renderer_def.renderer_impl) as ORC_RendererBase
	var scn_proxy_inst : ORC_SceneProxyBase = ORC_SceneProxyBase.new()
	var proxy_factory_inst : ORC_ProxyFactory = ORC_ImplFactory.create_impl(renderer_def.proxy_factory_impl) as ORC_ProxyFactory
	renderer_inst.scene_proxy = scn_proxy_inst
	scn_proxy_inst.renderer = renderer_inst
	scn_proxy_inst.proxy_factory = proxy_factory_inst
	
	for attach_format_def in renderer_def.attachment_format_defs:
		create_attachment(renderer_inst, attach_format_def)

	for render_pass_def in renderer_def.renderer_pass_defs:
		create_render_pass(renderer_inst, render_pass_def, renderer_def)
	
	for queue_def in renderer_def.proxy_queue_defs:
		create_proxy_queue(scn_proxy_inst, queue_def)
	
	return renderer_inst

static func create_proxy_queue(scene_proxy : ORC_SceneProxyBase, queue_def : ORC_ProxyQueue_Def) -> void:
	var processors : Array[ORC_QueueProcessor] = []
	
	for impl_def in queue_def.processors:
		var processor = ORC_ImplFactory.create_impl(impl_def) as ORC_QueueProcessor
		if processor != null:
			processors.append(processor)
	
	var init_query : ORC_DataQuery = create_query_from_def(scene_proxy, queue_def.init_query)
	scene_proxy.create_queue(queue_def.queue_name, init_query, processors)

static func create_query_from_def(scene_proxy : ORC_SceneProxyBase, query_def : ORC_DataQuery_Def) -> ORC_DataQuery:
	if query_def.type is ORC_GDImpl_Def:
		var gd_impl_def = query_def.type as ORC_GDImpl_Def
		var script = gd_impl_def.gd_script
		return scene_proxy.create_query_gd(script, query_def.flag_names, query_def.flag_values)
	elif query_def.type is ORC_CPPImpl_Def:
		var cpp_impl_def = query_def.type as ORC_CPPImpl_Def
		return scene_proxy.create_query_cpp(cpp_impl_def.cpp_class_name, query_def.flag_names, query_def.flag_values)
	return null

static func create_attachment(renderer_inst : ORC_RendererBase, attach_format_def : ORC_AttachmentFormat_Def) -> RID:
	var attachment : RID = create_texture_attachment(attach_format_def)
	renderer_inst.create_attachment(attach_format_def.attachment_name, attachment)
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

	return ORC_RDHelper.get_rd().texture_create(tf, view)

static func create_render_pass(renderer_inst : ORC_RendererBase, render_pass_def : ORC_RenderPassDef, renderer_def : ORC_Renderer_Def) -> ORC_RenderPassBase:
	var render_pass_inst = ORC_ImplFactory.create_impl(render_pass_def.pass_impl) as ORC_RenderPassBase
	if render_pass_inst == null:
		return null
		
	render_pass_inst.renderer = renderer_inst
	renderer_inst.create_render_pass(render_pass_def.pass_name, render_pass_inst)

	render_pass_inst.framebuffer_format = create_framebuffer_format_from_def(render_pass_def.fb_format_def, renderer_def.attachment_format_defs)

	var named_attachments : Array[RID]
	for name in render_pass_def.fb_format_def.get_all_attachment_keys():
		named_attachments.append(renderer_inst.get_attachment(name))
	render_pass_inst.framebuffer = ORC_RDHelper.get_rd().framebuffer_create(named_attachments, render_pass_inst.framebuffer_format)

	for key : StringName in render_pass_def.direct_pso_defs.keys():
		render_pass_inst.direct_psos[key] = create_pso(render_pass_def.direct_pso_defs[key], render_pass_inst.framebuffer_format)

	for key : StringName in render_pass_def.pso_factory_defs.keys():
		render_pass_inst.pso_factories[key] = create_pso_factory(render_pass_def.pso_factory_defs[key])

	return render_pass_inst

static func create_pso_factory(pso_factory_def : ORC_PSOFactoryDef) -> ORC_PSOFactory:
	var factory = ORC_ImplFactory.create_impl(pso_factory_def.factory_impl) as ORC_PSOFactory
	if factory != null: # TODO : mayby use this everywhere?
		var vertex_file = FileAccess.open(pso_factory_def.uber_vertex_shader_path, FileAccess.READ)
		if vertex_file != null:
			factory.uber_vertex_shader_src = vertex_file.get_as_text()
		else:
			push_error("Failed to open uber vertex shader file: %s" % pso_factory_def.uber_vertex_shader_path)
		
		var fragment_file = FileAccess.open(pso_factory_def.uber_fragment_shader_path, FileAccess.READ)
		if fragment_file != null:
			factory.uber_fragment_shader_src = fragment_file.get_as_text()
		else:
			push_error("Failed to open uber fragment shader file: %s" % pso_factory_def.uber_fragment_shader_path)
	return factory

static func create_framebuffer_format_from_def(fb_format_def : ORC_FramebufferFormat_Def, attachment_format_defs : Array[ORC_AttachmentFormat_Def]) -> int:
	var attachment_formats : Array[RDAttachmentFormat]
	var attachments_by_name : Dictionary = {}
	for attach_def in attachment_format_defs:
		attachments_by_name[attach_def.attachment_name] = attach_def

	for attach_key : StringName in fb_format_def.get_all_attachment_keys():
		var attachment_format : RDAttachmentFormat = RDAttachmentFormat.new()
		attachment_format.format = attachments_by_name[attach_key].format
		attachment_format.usage_flags = 0
		for bit in attachments_by_name[attach_key].usage_flags:
			attachment_format.usage_flags |= bit
		attachment_formats.append(attachment_format)

	return ORC_RDHelper.get_rd().framebuffer_format_create(attachment_formats)

static func create_pso(pso_def : ORC_PSODef, framebuffer_format : int) -> ORC_PSO:
	var instance = ORC_PSO.new()

	var path : String = pso_def.vertex_shader_path
	var file_path = path
	var file = FileAccess.open(file_path, FileAccess.READ)
	if file == null:
		push_error("Failed to open vertex shader file: %s" % file_path)
		return null

	var raw_source : String = file.get_as_text()
	var preprocessed_source : String = ORC_ShaderPreprocessor.preprocess(path, raw_source, pso_def.defines)
	var vertex_shader_src : String = preprocessed_source

	path = pso_def.fragment_shader_path
	file_path = path
	file = FileAccess.open(file_path, FileAccess.READ)
	if file == null:
		push_error("Failed to open fragment shader file: %s" % file_path)
		return null

	raw_source = file.get_as_text()
	preprocessed_source = ORC_ShaderPreprocessor.preprocess(path, raw_source, pso_def.defines)
	var fragment_shader_src : String = preprocessed_source

	instance.shader_program = compile_shader(vertex_shader_src, fragment_shader_src)

	var vf_def : ORC_VertexFormatDef = pso_def.vertex_format_def
	var vertex_format_info = create_vertex_format_info(vf_def)
	instance.vertex_format = ORC_RDHelper.create_vertex_format(vertex_format_info)

	var rasterizationState = RDPipelineRasterizationState.new()
	rasterizationState.cull_mode = pso_def.rasterization_state.cull_mode
	rasterizationState.depth_bias_clamp = pso_def.rasterization_state.depth_bias_clamp
	rasterizationState.depth_bias_constant_factor = pso_def.rasterization_state.depth_bias_constant_factor
	rasterizationState.depth_bias_enabled = pso_def.rasterization_state.depth_bias_enabled
	rasterizationState.depth_bias_slope_factor = pso_def.rasterization_state.depth_bias_slope_factor
	rasterizationState.discard_primitives = pso_def.rasterization_state.discard_primitives
	rasterizationState.enable_depth_clamp = pso_def.rasterization_state.enable_depth_clamp
	rasterizationState.front_face = pso_def.rasterization_state.front_face
	rasterizationState.line_width = pso_def.rasterization_state.line_width
	rasterizationState.patch_control_points = pso_def.rasterization_state.patch_control_points
	rasterizationState.wireframe = pso_def.rasterization_state.wireframe

	var multisampleState = RDPipelineMultisampleState.new()
	multisampleState.enable_alpha_to_coverage = pso_def.multisample_state.enable_alpha_to_coverage
	multisampleState.enable_alpha_to_one = pso_def.multisample_state.enable_alpha_to_one
	multisampleState.enable_sample_shading = pso_def.multisample_state.enable_sample_shading
	multisampleState.min_sample_shading = pso_def.multisample_state.min_sample_shading
	multisampleState.sample_count = pso_def.multisample_state.sample_count
	multisampleState.sample_masks = pso_def.multisample_state.sample_masks

	var depthStencilState = RDPipelineDepthStencilState.new()
	depthStencilState.back_op_compare = pso_def.depth_stencil_state.back_op_compare
	depthStencilState.back_op_compare_mask = pso_def.depth_stencil_state.back_op_compare_mask
	depthStencilState.back_op_depth_fail = pso_def.depth_stencil_state.back_op_depth_fail
	depthStencilState.back_op_fail = pso_def.depth_stencil_state.back_op_fail
	depthStencilState.back_op_pass = pso_def.depth_stencil_state.back_op_pass
	depthStencilState.back_op_reference = pso_def.depth_stencil_state.back_op_reference
	depthStencilState.back_op_write_mask = pso_def.depth_stencil_state.back_op_write_mask
	depthStencilState.depth_compare_operator = pso_def.depth_stencil_state.depth_compare_operator
	depthStencilState.depth_range_max = pso_def.depth_stencil_state.depth_range_max
	depthStencilState.depth_range_min = pso_def.depth_stencil_state.depth_range_min
	depthStencilState.enable_depth_range = pso_def.depth_stencil_state.enable_depth_range
	depthStencilState.enable_depth_test = pso_def.depth_stencil_state.enable_depth_test
	depthStencilState.enable_depth_write = pso_def.depth_stencil_state.enable_depth_write
	depthStencilState.enable_stencil = pso_def.depth_stencil_state.enable_stencil
	depthStencilState.front_op_compare = pso_def.depth_stencil_state.front_op_compare
	depthStencilState.front_op_compare_mask = pso_def.depth_stencil_state.front_op_compare_mask
	depthStencilState.front_op_depth_fail = pso_def.depth_stencil_state.front_op_depth_fail
	depthStencilState.front_op_fail = pso_def.depth_stencil_state.front_op_fail
	depthStencilState.front_op_pass = pso_def.depth_stencil_state.front_op_pass
	depthStencilState.front_op_reference = pso_def.depth_stencil_state.front_op_reference
	depthStencilState.front_op_write_mask = pso_def.depth_stencil_state.front_op_write_mask

	var colorBlendState = RDPipelineColorBlendState.new()
	for color_blend_attachment_def : ORC_PSOColorBlendAttachmentDef in pso_def.blend_attachments:
		var colorBlendStateAttachment : RDPipelineColorBlendStateAttachment = RDPipelineColorBlendStateAttachment.new()
		colorBlendStateAttachment.alpha_blend_op = color_blend_attachment_def.alpha_blend_op
		colorBlendStateAttachment.color_blend_op = color_blend_attachment_def.color_blend_op
		colorBlendStateAttachment.dst_alpha_blend_factor = color_blend_attachment_def.dst_alpha_blend_factor
		colorBlendStateAttachment.dst_color_blend_factor = color_blend_attachment_def.dst_color_blend_factor
		colorBlendStateAttachment.enable_blend = color_blend_attachment_def.enable_blend
		colorBlendStateAttachment.src_alpha_blend_factor = color_blend_attachment_def.src_alpha_blend_factor
		colorBlendStateAttachment.src_color_blend_factor = color_blend_attachment_def.src_color_blend_factor
		colorBlendStateAttachment.write_a = color_blend_attachment_def.write_a
		colorBlendStateAttachment.write_b = color_blend_attachment_def.write_b
		colorBlendStateAttachment.write_g = color_blend_attachment_def.write_g
		colorBlendStateAttachment.write_r = color_blend_attachment_def.write_r
		colorBlendState.attachments.append(colorBlendStateAttachment)

	colorBlendState.blend_constant = pso_def.blend_constant
	colorBlendState.enable_logic_op = pso_def.enable_logic_op
	colorBlendState.logic_op = pso_def.logic_op

	instance.pipeline = ORC_RDHelper.get_rd().render_pipeline_create(instance.shader_program, framebuffer_format, instance.vertex_format, RenderingDevice.RENDER_PRIMITIVE_TRIANGLES, rasterizationState, multisampleState, depthStencilState, colorBlendState)

	return instance

static func compile_shader(vertex_src : String, fragment_src : String) -> RID:
	var shader_source = RDShaderSource.new()
	shader_source.language = RenderingDevice.SHADER_LANGUAGE_GLSL;
	shader_source.source_vertex = vertex_src;
	shader_source.source_fragment = fragment_src;
	
	return ORC_RDHelper.get_rd().shader_create_from_spirv(ORC_RDHelper.get_rd().shader_compile_spirv_from_source(shader_source))

static func create_vertex_format_info(vertex_format_def : ORC_VertexFormatDef) -> ORC_VertexFormatInfo:
	var vertex_format_info = ORC_VertexFormatInfo.new()
	vertex_format_info.is_2d = vertex_format_def.is_2d
	vertex_format_info.has_normal = vertex_format_def.has_normal
	vertex_format_info.has_tangent = vertex_format_def.has_tangent
	vertex_format_info.has_color = vertex_format_def.has_color
	vertex_format_info.has_uv = vertex_format_def.has_uv
	vertex_format_info.has_uv2 = vertex_format_def.has_uv2
	vertex_format_info.has_bones = vertex_format_def.has_bones
	vertex_format_info.has_weights = vertex_format_def.has_weights
	return vertex_format_info



static func create_vertex_format(vertex_format_def : ORC_VertexFormatDef) -> int:
	var vertex_format_info : ORC_VertexFormatInfo = create_vertex_format_info(vertex_format_def)
	return ORC_RDHelper.create_vertex_format(vertex_format_info)
