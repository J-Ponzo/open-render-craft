#ifndef ORC_MACROS_H
#define ORC_MACROS_H

// Declare GDScript overridable method
#define DECLARE_GD_OVERRIDABLE_METHOD(RET_TYPE, METHOD_NAME, ...) \
    RET_TYPE gdcall_##METHOD_NAME(__VA_ARGS__); \
    RET_TYPE gdsuper_##METHOD_NAME(__VA_ARGS__); \
    virtual RET_TYPE METHOD_NAME(__VA_ARGS__);

// Define GDScript overridable method with 0 arguments
#define DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(CLASS_NAME, RET_TYPE, METHOD_NAME) \
RET_TYPE CLASS_NAME::gdcall_##METHOD_NAME() { \
    static const StringName override_name(#METHOD_NAME "_override"); \
    if (has_method(override_name)) { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            call(override_name); \
        } else { \
            Variant ret = call(override_name); \
            return (RET_TYPE)ret; \
        } \
    } else { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            METHOD_NAME(); \
        } else { \
            return METHOD_NAME(); \
        } \
    } \
} \
RET_TYPE CLASS_NAME::gdsuper_##METHOD_NAME() { \
    if constexpr (std::is_same_v<RET_TYPE, void>) { \
        METHOD_NAME(); \
    } else { \
        return METHOD_NAME(); \
    } \
}

// Define GDScript overridable method with 1 argument
#define DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(CLASS_NAME, RET_TYPE, METHOD_NAME, T1, A1) \
RET_TYPE CLASS_NAME::gdcall_##METHOD_NAME(T1 A1) { \
    static const StringName override_name(#METHOD_NAME "_override"); \
    if (has_method(override_name)) { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            call(override_name, A1); \
        } else { \
            Variant ret = call(override_name, A1); \
            return (RET_TYPE)ret; \
        } \
    } else { \
        if constexpr (!std::is_same_v<RET_TYPE, void>) { \
            return METHOD_NAME(A1); \
        } else { \
            METHOD_NAME(A1); \
        } \
    } \
} \
RET_TYPE CLASS_NAME::gdsuper_##METHOD_NAME(T1 A1) { \
    if constexpr (!std::is_same_v<RET_TYPE, void>) { \
        return METHOD_NAME(A1); \
    } else { \
        METHOD_NAME(A1); \
    } \
}

// Define GDScript overridable method with 2 argument
#define DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(CLASS_NAME, RET_TYPE, METHOD_NAME, T1, A1, T2, A2) \
RET_TYPE CLASS_NAME::gdcall_##METHOD_NAME(T1 A1, T2 A2) { \
    static const StringName override_name(#METHOD_NAME "_override"); \
    if (has_method(override_name)) { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            call(override_name, A1, A2); \
        } else { \
            Variant ret = call(override_name, A1, A2); \
            return (RET_TYPE)ret; \
        } \
    } else { \
        if constexpr (!std::is_same_v<RET_TYPE, void>) { \
            return METHOD_NAME(A1, A2); \
        } else { \
            METHOD_NAME(A1, A2); \
        } \
    } \
} \
RET_TYPE CLASS_NAME::gdsuper_##METHOD_NAME(T1 A1, T2 A2) { \
    if constexpr (!std::is_same_v<RET_TYPE, void>) { \
        return METHOD_NAME(A1, A2); \
    } else { \
        METHOD_NAME(A1, A2); \
    } \
}

// Define GDScript overridable method with 3 argument
#define DEFINE_GD_OVERRIDABLE_METHOD_3_ARGS(CLASS_NAME, RET_TYPE, METHOD_NAME, T1, A1, T2, A2, T3, A3) \
RET_TYPE CLASS_NAME::gdcall_##METHOD_NAME(T1 A1, T2 A2, T3 A3) { \
    static const StringName override_name(#METHOD_NAME "_override"); \
    if (has_method(override_name)) { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            call(override_name, A1, A2, A3); \
        } else { \
            Variant ret = call(override_name, A1, A2, A3); \
            return (RET_TYPE)ret; \
        } \
    } else { \
        if constexpr (!std::is_same_v<RET_TYPE, void>) { \
            return METHOD_NAME(A1, A2, A3); \
        } else { \
            METHOD_NAME(A1, A2, A3); \
        } \
    } \
} \
RET_TYPE CLASS_NAME::gdsuper_##METHOD_NAME(T1 A1, T2 A2, T3 A3) { \
    if constexpr (!std::is_same_v<RET_TYPE, void>) { \
        return METHOD_NAME(A1, A2, A3); \
    } else { \
        METHOD_NAME(A1, A2, A3); \
    } \
}

// Define GDScript overridable method with 4 argument
#define DEFINE_GD_OVERRIDABLE_METHOD_4_ARGS(CLASS_NAME, RET_TYPE, METHOD_NAME, T1, A1, T2, A2, T3, A3, T4, A4) \
RET_TYPE CLASS_NAME::gdcall_##METHOD_NAME(T1 A1, T2 A2, T3 A3, T4 A4) { \
    static const StringName override_name(#METHOD_NAME "_override"); \
    if (has_method(override_name)) { \
        if constexpr (std::is_same_v<RET_TYPE, void>) { \
            call(override_name, A1, A2, A3, A4); \
        } else { \
            Variant ret = call(override_name, A1, A2, A3, A4); \
            return (RET_TYPE)ret; \
        } \
    } else { \
        if constexpr (!std::is_same_v<RET_TYPE, void>) { \
            return METHOD_NAME(A1, A2, A3, A4); \
        } else { \
            METHOD_NAME(A1, A2, A3, A4); \
        } \
    } \
} \
RET_TYPE CLASS_NAME::gdsuper_##METHOD_NAME(T1 A1, T2 A2, T3 A3, T4 A4) { \
    if constexpr (!std::is_same_v<RET_TYPE, void>) { \
        return METHOD_NAME(A1, A2, A3, A4); \
    } else { \
        METHOD_NAME(A1, A2, A3, A4); \
    } \
}

// Bind GDScript overridable method
#define BIND_GD_OVERRIDABLE_METHOD(CLASS_NAME, METHOD_NAME, ...) \
    ClassDB::bind_method(D_METHOD(#METHOD_NAME, ##__VA_ARGS__), &CLASS_NAME::gdcall_##METHOD_NAME); \
    ClassDB::bind_method(D_METHOD("super_" #METHOD_NAME, ##__VA_ARGS__), &CLASS_NAME::gdsuper_##METHOD_NAME);

#endif