#include<safe_rid.h>
#include<rd_helper.h>

using namespace godot;

void ORC_SafeRID::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_rid"), &ORC_SafeRID::get_rid);
    ClassDB::bind_method(D_METHOD("set_rid", "rid"), &ORC_SafeRID::set_rid);
    ADD_PROPERTY(PropertyInfo(Variant::RID, "rid"), "set_rid", "get_rid");

    ClassDB::bind_method(D_METHOD("free_rid"), &ORC_SafeRID::free_rid);
}

RID ORC_SafeRID::get_rid() const {
    return rid;
}

void ORC_SafeRID::set_rid(const RID& rid) {
    free_rid();
    this->rid = rid;
}

void ORC_SafeRID::free_rid() {
    if (is_valid()) {
		ORC_RDHelper::get_rd()->free_rid(rid);
        rid = RID();
    }
}

void ORC_BufferRID::_bind_methods() {
}

bool ORC_BufferRID::is_valid() const {
    return rid.is_valid();
}

void ORC_SetRID::_bind_methods() {
}

bool ORC_SetRID::is_valid() const {
	return ORC_RDHelper::get_rd()->uniform_set_is_valid(rid);
}

void ORC_SamplerRID::_bind_methods() {
}

bool ORC_SamplerRID::is_valid() const {
    return rid.is_valid();
}

void ORC_VertexArrayRID::_bind_methods() {
}

bool ORC_VertexArrayRID::is_valid() const {
    return rid.is_valid();
}

void ORC_IndexArrayRID::_bind_methods() {
}

bool ORC_IndexArrayRID::is_valid() const {
    return rid.is_valid();
}

void ORC_TextureRID::_bind_methods() {
}

bool ORC_TextureRID::is_valid() const {
    return rid.is_valid();
}