#ifndef ORCTEST_CAMERAPROXY_H
#define ORCTEST_CAMERAPROXY_H

#include "mock_proxy_object.h"

namespace godot {

class ORCTEST_CameraProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_CameraProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int cam_update_count;
	static int get_cam_update_count() { return cam_update_count; }
	static void reset_cam_update_count() { cam_update_count = 0; }

	void update_impl();
};

}

#endif
