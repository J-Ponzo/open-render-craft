#ifndef ORC_SHADER_PREPROCESSOR_H
#define ORC_SHADER_PREPROCESSOR_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

//TODO add tests for this
class ORC_ShaderPreprocessor : public RefCounted {
    GDCLASS(ORC_ShaderPreprocessor, RefCounted)

private:
    static bool use_filenames_in_line_directives;
    static bool is_debug;

    static String _inject_defines(const String& raw_source, const TypedArray<StringName>& defines);
    static String _expand_includes_rec(const String& path, const String& raw_source, Dictionary& already_included_paths);

protected:
    static void _bind_methods();

public:
    ORC_ShaderPreprocessor();
    ~ORC_ShaderPreprocessor();

    static String preprocess(const String& path, const String& raw_source, const TypedArray<StringName>& defines);
    static String generate_dummy_shader_for_partial_source(const String& partial_source_path, const String& raw_partial_source);
};

}

#endif
