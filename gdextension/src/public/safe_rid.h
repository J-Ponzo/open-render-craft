#ifndef ORC_SAFE_RID_H
#define ORC_SAFE_RID_H

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class ORC_SafeRID : public RefCounted {
GDCLASS(ORC_SafeRID, RefCounted)

protected:
    static void _bind_methods();

    RID rid;
    virtual bool is_valid() const { return false; }

public:
    void set_rid(const RID& rid);
    RID get_rid() const;
    void free_rid();
};

class ORC_BufferRID : public ORC_SafeRID {
GDCLASS(ORC_BufferRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

class ORC_SetRID : public ORC_SafeRID {
GDCLASS(ORC_SetRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

class ORC_SamplerRID : public ORC_SafeRID {
GDCLASS(ORC_SamplerRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

class ORC_VertexArrayRID : public ORC_SafeRID {
GDCLASS(ORC_VertexArrayRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

class ORC_IndexArrayRID : public ORC_SafeRID {
GDCLASS(ORC_IndexArrayRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

class ORC_TextureRID : public ORC_SafeRID {
GDCLASS(ORC_TextureRID, ORC_SafeRID)

protected:
    static void _bind_methods();
    virtual bool is_valid() const override;
};

}

#endif