#ifndef ORC_QUERYFILTERPROCESSOR_H
#define ORC_QUERYFILTERPROCESSOR_H

#include <queue_processor.h>
#include <feature_query.h>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_QueryFilterProcessor : public ORC_QueueProcessor {
    GDCLASS(ORC_QueryFilterProcessor, ORC_QueueProcessor)

private:
    Ref<ORC_FeatureQuery> query;

protected:
    static void _bind_methods();

public:
    void set_query(const Ref<ORC_FeatureQuery>& p_query);
    Ref<ORC_FeatureQuery> get_query() const { return query; }
    
    TypedArray<ORC_ProxyData> process_impl(const TypedArray<ORC_ProxyData>& input) override;
};

}

#endif // ORC_QUERYFILTERPROCESSOR_H
