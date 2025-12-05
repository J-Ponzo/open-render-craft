#ifndef ORC_SHADER_PREPROCESSOR_H
#define ORC_SHADER_PREPROCESSOR_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class ORC_ShaderPreprocessor : public Object {
    GDCLASS(ORC_ShaderPreprocessor, Object)

private:
    static ORC_ShaderPreprocessor* singleton;
    static bool use_filenames_in_line_directives;
    static bool is_debug;

    static String _inject_defines(const String& raw_source, const TypedArray<StringName>& defines);
    static String _expand_includes_rec(const String& path, const String& raw_source, Dictionary& already_included_paths);

protected:
    static void _bind_methods();

public:
    ORC_ShaderPreprocessor();
    ~ORC_ShaderPreprocessor();

    static ORC_ShaderPreprocessor* get_singleton() { return singleton; }

    String preprocess(const String& path, const String& raw_source, const TypedArray<StringName>& defines);
    String generate_dummy_shader_for_partial_source(const String& partial_source_path, const String& raw_partial_source);
};

}

#endif
