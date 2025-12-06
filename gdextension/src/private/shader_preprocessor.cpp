#include <shader_preprocessor.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/reg_ex.hpp>
#include <godot_cpp/classes/reg_ex_match.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

bool ORC_ShaderPreprocessor::use_filenames_in_line_directives = true;
bool ORC_ShaderPreprocessor::is_debug = true;

ORC_ShaderPreprocessor::ORC_ShaderPreprocessor() {
}

ORC_ShaderPreprocessor::~ORC_ShaderPreprocessor() {
}

void ORC_ShaderPreprocessor::_bind_methods() {
    ClassDB::bind_static_method("ORC_ShaderPreprocessor", D_METHOD("preprocess", "path", "raw_source", "defines"), &ORC_ShaderPreprocessor::preprocess);
    ClassDB::bind_static_method("ORC_ShaderPreprocessor", D_METHOD("generate_dummy_shader_for_partial_source", "partial_source_path", "raw_partial_source"), &ORC_ShaderPreprocessor::generate_dummy_shader_for_partial_source);
}

String ORC_ShaderPreprocessor::_inject_defines(const String& raw_source, const TypedArray<StringName>& defines) {
    PackedStringArray lines = raw_source.split("\n");
    
    if (lines.size() == 0) return raw_source;
    
    String output = lines[0] + "\n\n";
    
    for (int i = 0; i < defines.size(); i++) {
        StringName define = defines[i];
        output += "#define " + String(define) + "\n";
    }
    output += "\n";
    
    for (int i = 1; i < lines.size(); i++) {
        output += lines[i] + "\n";
    }
    
    return output;
}

String ORC_ShaderPreprocessor::_expand_includes_rec(const String& path, const String& raw_source, Dictionary& already_included_paths) {
    if (already_included_paths.has(path)) {
        UtilityFunctions::push_error("Include cycle detected with : " + path);
        return "";
    }
    already_included_paths[path] = true;
    
    String output = "";
    
    PackedStringArray lines = raw_source.split("\n");
    
    Ref<RegEx> version_regex;
    version_regex.instantiate();
    version_regex->compile("^#version\\s+([0-9]+)");
    
    Ref<RegEx> include_regex;
    include_regex.instantiate();
    include_regex->compile("^#include\\s+\"(.*)\"");
    
    for (int l = 0; l < lines.size(); l++) {
        String line = lines[l];
        
        Ref<RegExMatch> match = include_regex->search(line);
        if (match.is_valid() && match->get_group_count() > 0) {
            String include_path = match->get_string(1);
            String include_raw_source = FileAccess::get_file_as_string(include_path);
            
            if (use_filenames_in_line_directives) {
                output += "#line 1 \"" + include_path + "\"\n";
            } else {
                output += "#line 1 \"\"\n";
            }
            
            output += _expand_includes_rec(include_path, include_raw_source, already_included_paths);
            
            if (use_filenames_in_line_directives) {
                output += "#line " + String::num_int64(l + 1) + " \"" + path + "\"\n";
            } else {
                output += "#line " + String::num_int64(l + 1) + " \"\"\n";
            }
        } else {
            output += line + "\n";
            
            Ref<RegExMatch> version_match = version_regex->search(line);
            if (use_filenames_in_line_directives && version_match.is_valid()) {
                output += "#extension GL_GOOGLE_cpp_style_line_directive : require\n";
            }
        }
    }
    
    return output;
}

String ORC_ShaderPreprocessor::preprocess(const String& path, const String& raw_source, const TypedArray<StringName>& defines) {
    Dictionary already_included_paths;
    String preprocessed_source = _expand_includes_rec(path, raw_source, already_included_paths);
    preprocessed_source = _inject_defines(preprocessed_source, defines);
    
    if (is_debug) {
        UtilityFunctions::print(preprocessed_source);
    }
    
    return preprocessed_source;
}

String ORC_ShaderPreprocessor::generate_dummy_shader_for_partial_source(const String& partial_source_path, const String& raw_partial_source) {
    String dummy_shader = "";
    dummy_shader += "#version 450\n";
    
    if (use_filenames_in_line_directives) {
        dummy_shader += "#extension GL_GOOGLE_cpp_style_line_directive : require\n";
        dummy_shader += "#line 1 \"" + partial_source_path + "\"\n";
    } else {
        dummy_shader += "#line 1 \"\"\n";
    }
    
    Dictionary already_included_paths;
    dummy_shader += _expand_includes_rec(partial_source_path, raw_partial_source, already_included_paths);
    
    if (use_filenames_in_line_directives) {
        dummy_shader += "#line -1 \"<dummy_shader>\"\n";
    } else {
        dummy_shader += "#line -1 \"\"\n";
    }
    
    dummy_shader += "void main() { }";
    
    return dummy_shader;
}
