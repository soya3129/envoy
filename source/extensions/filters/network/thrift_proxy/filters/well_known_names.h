#pragma once

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace ThriftFilters {

/**
 * Well-known http filter names.
 * NOTE: New filters should use the well known name: envoy.filters.thrift.name.
 */
class ThriftFilterNameValues {
public:
  // Ratelimit filter
  const std::string RATE_LIMIT = "envoy.filters.thrift.rate_limit";

  // Router filter
  const std::string ROUTER = "envoy.filters.thrift.router";
};

typedef ConstSingleton<ThriftFilterNameValues> ThriftFilterNames;

} // namespace ThriftFilters
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
