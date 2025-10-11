#ifndef ORC_SCENEPROXYBASE_H
#define ORC_SCENEPROXYBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class ORC_SceneProxyBase : public RefCounted {
	GDCLASS(ORC_SceneProxyBase, RefCounted)

protected:
	static void _bind_methods();

public:
	ORC_SceneProxyBase();
	~ORC_SceneProxyBase();

	void setup(Node* scene);
	void pre_render();
	void post_render();
	void cleanup();
};

}

#endif