#include "metric_family.h"

namespace nvidia { namespace inferenceserver {

//
// Implementation for TRITONSERVER_MetricFamily.
//
MetricFamily::MetricFamily(
    TRITONSERVER_MetricKind kind, const char* name, const char* description,
    std::shared_ptr<prometheus::Registry> registry)
{
  // TODO: Check correctness of void* cast here
  switch (kind) {
    case TRITONSERVER_METRIC_KIND_COUNTER:
      family_ = reinterpret_cast<void*>(&prometheus::BuildCounter()
                                             .Name(name)
                                             .Help(description)
                                             .Register(*registry));
      break;
    case TRITONSERVER_METRIC_KIND_GAUGE:
      family_ = reinterpret_cast<void*>(&prometheus::BuildGauge()
                                             .Name(name)
                                             .Help(description)
                                             .Register(*registry));
      break;
    default:
      // TODO: Error unsupported kind
      family_ = nullptr;
      break;
  }

  kind_ = kind;
}

prometheus::MetricType
MetricFamily::PrometheusType()
{
  switch (kind_) {
    case TRITONSERVER_METRIC_KIND_COUNTER:
      return prometheus::MetricType::Counter;
    case TRITONSERVER_METRIC_KIND_GAUGE:
      return prometheus::MetricType::Gauge;
  }

  // Unsupported type
  return prometheus::MetricType::Untyped;
}

}}  // namespace nvidia::inferenceserver
