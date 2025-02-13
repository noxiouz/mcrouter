/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <folly/Range.h>
#include <folly/json.h>

#include "mcrouter/PoolFactory.h"
#include "mcrouter/ProxyBase.h"
#include "mcrouter/TkoTracker.h"
#include "mcrouter/lib/config/RouteHandleProviderIf.h"
#include "mcrouter/routes/McrouterRouteHandle.h"

namespace folly {
struct dynamic;
} // namespace folly

namespace facebook {
namespace memcache {
namespace mcrouter {

FOLLY_ATTR_WEAK MemcacheRouterInfo::RouteHandlePtr makeSRRoute(
    RouteHandleFactory<MemcacheRouterInfo::RouteHandleIf>&,
    const folly::dynamic& json,
    ProxyBase& proxy);

template <class RouteHandleIf>
class ExtraRouteHandleProviderIf;
class ProxyBase;

/**
 * RouteHandleProviderIf implementation that can create mcrouter-specific
 * routes.
 */
template <class RouterInfo>
class McRouteHandleProvider
    : public RouteHandleProviderIf<typename RouterInfo::RouteHandleIf> {
 public:
  using RouteHandleIf = typename RouterInfo::RouteHandleIf;
  using RouteHandlePtr = std::shared_ptr<RouteHandleIf>;
  using RouteHandleFactoryFunc = std::function<RouteHandlePtr(
      RouteHandleFactory<RouteHandleIf>&,
      const folly::dynamic&)>;
  using RouteHandleFactoryMap = std::
      unordered_map<folly::StringPiece, RouteHandleFactoryFunc, folly::Hash>;
  using RouteHandleFactoryFuncWithProxy = std::function<RouteHandlePtr(
      RouteHandleFactory<RouteHandleIf>&,
      const folly::dynamic&,
      ProxyBase&)>;
  using RouteHandleFactoryMapWithProxy = std::unordered_map<
      folly::StringPiece,
      RouteHandleFactoryFuncWithProxy,
      folly::Hash>;

  McRouteHandleProvider(ProxyBase& proxy, PoolFactory& poolFactory);

  std::vector<RouteHandlePtr> create(
      RouteHandleFactory<RouteHandleIf>& factory,
      folly::StringPiece type,
      const folly::dynamic& json) final;

  const folly::dynamic& parsePool(const folly::dynamic& json) final;

  folly::StringKeyedUnorderedMap<RouteHandlePtr> releaseAsyncLogRoutes() {
    return std::move(asyncLogRoutes_);
  }

  folly::StringKeyedUnorderedMap<std::vector<RouteHandlePtr>> releasePools() {
    return std::move(pools_);
  }

  folly::StringKeyedUnorderedMap<
      std::vector<std::shared_ptr<const AccessPoint>>>
  releaseAccessPoints() {
    for (auto& it : accessPoints_) {
      it.second.shrink_to_fit();
    }
    return std::move(accessPoints_);
  }

  ~McRouteHandleProvider() override;

 private:
  ProxyBase& proxy_;
  PoolFactory& poolFactory_;
  std::unique_ptr<ExtraRouteHandleProviderIf<RouterInfo>> extraProvider_;

  // poolName -> AsynclogRoute
  folly::StringKeyedUnorderedMap<RouteHandlePtr> asyncLogRoutes_;

  // poolName -> destinations
  folly::StringKeyedUnorderedMap<std::vector<RouteHandlePtr>> pools_;

  // poolName -> AccessPoints
  folly::StringKeyedUnorderedMap<
      std::vector<std::shared_ptr<const AccessPoint>>>
      accessPoints_;

  const RouteHandleFactoryMap routeMap_;
  const RouteHandleFactoryMapWithProxy routeMapWithProxy_;

  const std::vector<RouteHandlePtr>& makePool(
      RouteHandleFactory<RouteHandleIf>& factory,
      const PoolFactory::PoolJson& json);

  RouteHandlePtr makePoolRoute(
      RouteHandleFactory<RouteHandleIf>& factory,
      const folly::dynamic& json);

  RouteHandlePtr createSRRoute(
      RouteHandleFactory<RouteHandleIf>& factory,
      const folly::dynamic& json);

  RouteHandlePtr createAsynclogRoute(
      RouteHandlePtr route,
      std::string asynclogName);

  template <class Transport>
  RouteHandlePtr createDestinationRoute(
      std::shared_ptr<AccessPoint> ap,
      std::chrono::milliseconds timeout,
      std::chrono::milliseconds connectTimeout,
      uint32_t qosClass,
      uint32_t qosPath,
      folly::StringPiece poolName,
      size_t indexInPool,
      int32_t poolStatIndex,
      bool disableRequestDeadlineCheck,
      const std::shared_ptr<PoolTkoTracker>& poolTkoTracker,
      bool keepRoutingPrefix);

  RouteHandleFactoryMap buildRouteMap();

  void buildRouteMap0(RouteHandleFactoryMap& map);
  void buildRouteMap1(RouteHandleFactoryMap& map);
  void buildRouteMap2(RouteHandleFactoryMap& map);
  void buildRouteMap3(RouteHandleFactoryMap& map);

  RouteHandleFactoryMapWithProxy buildRouteMapWithProxy();

  // This can be removed when the buildRouteMap specialization for
  // MemcacheRouterInfo is removed.
  RouteHandleFactoryMap buildCheckedRouteMap();
  RouteHandleFactoryMapWithProxy buildCheckedRouteMapWithProxy();

  std::unique_ptr<ExtraRouteHandleProviderIf<RouterInfo>> buildExtraProvider();
};

} // namespace mcrouter
} // namespace memcache
} // namespace facebook

#include "McRouteHandleProvider-inl.h"
