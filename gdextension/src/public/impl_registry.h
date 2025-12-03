#ifndef ORC_IMPL_REGISTRY_H
#define ORC_IMPL_REGISTRY_H

#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <functional>

using namespace godot;

class ORC_ImplRegistry {
public:
    struct ImplModule {
        const char* name;
        std::function<void(ModuleInitializationLevel)> init_func;
        std::function<void(ModuleInitializationLevel)> uninit_func;
    };

private:
    std::vector<ImplModule> modules;
    ORC_ImplRegistry() = default;

public:
    static ORC_ImplRegistry& get_singleton() {
        static ORC_ImplRegistry instance;
        return instance;
    }

    void register_module(const char* name, 
                        std::function<void(ModuleInitializationLevel)> init_func, 
                        std::function<void(ModuleInitializationLevel)> uninit_func) {
        modules.push_back({name, init_func, uninit_func});
    }
    
    //TODO log error if init_func is nullptr?
    void initialize_all(ModuleInitializationLevel p_level) {
        for (const auto& module : modules) {
            if (module.init_func) {
                UtilityFunctions::print("[ORC] " + String(module.name) + " C++ implementation registered");
                module.init_func(p_level);
            }
        }
    }
    
    //TODO log error if uninit_func is nullptr?
    void uninitialize_all(ModuleInitializationLevel p_level) {
        for (auto it = modules.rbegin(); it != modules.rend(); ++it) {
            if (it->uninit_func) {
                UtilityFunctions::print("[ORC] " + String(it->name) + " C++ implementation unregistered");
                it->uninit_func(p_level);
            }
        }
    }
};

class ORC_ImplModuleRegistrar {
public:
    ORC_ImplModuleRegistrar(const char* name, 
                            std::function<void(ModuleInitializationLevel)> init_func,
                            std::function<void(ModuleInitializationLevel)> uninit_func) {
        ORC_ImplRegistry::get_singleton().register_module(name, init_func, uninit_func);
    }
};

#define ORC_REGISTER_IMPL_MODULE(name, init_func, uninit_func) \
    static ORC_ImplModuleRegistrar __orc_impl_registrar_##init_func(name, init_func, uninit_func)

#endif // ORC_IMPL_REGISTRY_H
