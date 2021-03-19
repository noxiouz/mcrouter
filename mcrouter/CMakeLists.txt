cmake_minimum_required(VERSION 3.14)
project("mcrouter")

set(CMAKE_MODULE_PATH 
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
  ${CMAKE_MODULE_PATH}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ..
)

add_definitions(-DLIBMC_FBTRACE_DISABLE=1 -DHAVE_CONFIG_H=1)

configure_file(
  mcrouter_config-impl.h ${CMAKE_SOURCE_DIR}/config-impl.h COPYONLY
)
configure_file(
  RouterRegistry-impl.h ${CMAKE_SOURCE_DIR}/RouterRegistry.h COPYONLY
)
configure_file(
  ThriftAcceptor-impl.h ${CMAKE_SOURCE_DIR}/ThriftAcceptor.h COPYONLY
)
configure_file(
  mcrouter_sr_deps-impl.h ${CMAKE_SOURCE_DIR}/mcrouter_sr_deps.h COPYONLY
)

#TODO: add commands to generate files from thrift

find_package(gflags REQUIRED)
find_package(Folly REQUIRED)
find_package(fizz REQUIRED)
find_package(wangle REQUIRED)
find_package(FBThrift REQUIRED)

add_library(libmcrouter_core
  AsyncLog.cpp
  AsyncLog.h
  AsyncWriter.cpp
  AsyncWriter.h
  AsyncWriterEntry.h
  CallbackPool-inl.h
  CallbackPool.h
  CarbonRouterClient-inl.h
  CarbonRouterClient.h
  CarbonRouterClientBase.cpp
  CarbonRouterClientBase.h
  CarbonRouterFactory.h
  CarbonRouterFactory.cpp
  CarbonRouterInstance-inl.h
  CarbonRouterInstance.cpp
  CarbonRouterInstance.h
  CarbonRouterInstanceBase.cpp
  CarbonRouterInstanceBase.h
  ConfigApi.cpp
  ConfigApi.h
  ConfigApiIf.h
  ExecutorObserver.h
  ExponentialSmoothData.h
  FileDataProvider.cpp
  FileDataProvider.h
  FileObserver.cpp
  FileObserver.h
  ForEachPossibleClient.h
  flavor.cpp
  flavor.h
  LeaseTokenMap.cpp
  LeaseTokenMap.h
  mcrouter_config-impl.h 
  mcrouter_config.cpp 
  mcrouter_config.h 
  mcrouter_options_list.h 
  McrouterClient.h 
  McrouterFiberContext.cpp 
  McrouterFiberContext.h 
  McrouterInstance.h 
  McrouterLogFailure.cpp 
  McrouterLogFailure.h 
  McrouterLogger.cpp 
  McrouterLogger.h 
  McrouterManager.cpp
  McrouterManager.h
  lib/network/McCallbackUtils.h 
  Observable-inl.h 
  Observable.h 
  options-template.h 
  options.cpp 
  options.h 
  OptionsUtil.cpp 
  OptionsUtil.h 
  PoolFactory.cpp 
  PoolFactory.h 
  Proxy-inl.h 
  Proxy.h 
  ProxyBase-inl.h 
  ProxyBase.cpp 
  ProxyBase.h 
  ProxyConfig-inl.h 
  ProxyConfig.h 
  ProxyConfigBuilder.cpp 
  ProxyConfigBuilder.h 
  ProxyDestination-inl.h 
  ProxyDestination.h 
  ProxyDestinationBase.cpp
  ProxyDestinationBase.h
  ProxyDestinationKey.cpp
  ProxyDestinationKey.h
  ProxyDestinationMap-inl.h
  ProxyDestinationMap.cpp
  ProxyDestinationMap.h
  ProxyRequestContext.cpp
  ProxyRequestContext.h
  ProxyRequestContextTyped-inl.h
  ProxyRequestContextTyped.h
  ProxyRequestLogger-inl.h
  ProxyRequestLogger.h
  ProxyRequestPriority.h
  ProxyStats.cpp
  ProxyStats.h
  route.cpp
  route.h
  routes/AllAsyncRouteFactory.h
  routes/AllFastestRouteFactory.h
  routes/AllInitialRouteFactory.h
  routes/AllMajorityRouteFactory.h
  routes/AllSyncRouteFactory.h
  routes/AsynclogRoute.h
  routes/BigValueRoute-inl.h
  routes/BigValueRoute.cpp
  routes/BigValueRoute.h
  routes/BigValueRouteIf.h
  routes/CarbonLookasideRoute.h
  routes/CarbonLookasideRoute.cpp
  routes/DefaultShadowPolicy.h
  routes/DestinationRoute.h
  routes/DevNullRoute.h
  routes/ErrorRoute.h
  routes/ExtraRouteHandleProviderIf.h
  routes/FailoverPolicy.h
  routes/FailoverRateLimiter.cpp
  routes/FailoverRateLimiter.h
  routes/FailoverRoute-inl.h
  routes/FailoverRoute.h
  routes/FailoverWithExptimeRouteFactory.h
  routes/HostIdRouteFactory.h
  routes/KeySplitRoute.cpp
  routes/KeySplitRoute.h
  routes/L1L2CacheRouteFactory.h
  routes/L1L2SizeSplitRoute.cpp
  routes/L1L2SizeSplitRoute.h
  routes/LatestRoute.h
  routes/McExtraRouteHandleProvider-inl.h
  routes/McExtraRouteHandleProvider.h
  routes/McImportResolver.cpp
  routes/McImportResolver.h
  routes/McRouteBuildRouteMap0.cpp
  routes/McRouteBuildRouteMap1.cpp
  routes/McRouteBuildRouteMap2.cpp
  routes/McRouteBuildRouteMap3.cpp
  routes/McRouteHandleBuilder.h
  routes/McRouteHandleProvider-inl.h
  routes/McRouteHandleProvider.cpp
  routes/McRouteHandleProvider.h
  routes/McrouterRouteHandle.h
  routes/MigrateRouteFactory.h
  routes/MissFailoverRoute.h
  routes/ModifyExptimeRoute.h
  routes/ModifyKeyRoute.h
  routes/NullRoute.cpp
  routes/OperationSelectorRoute-inl.h
  routes/OperationSelectorRoute.h
  routes/OriginalClientHashRoute.h
  routes/OutstandingLimitRoute.h
  routes/PoolRouteUtils.h
  routes/PrefixSelectorRoute.h
  routes/ProxyRoute-inl.h
  routes/ProxyRoute.h
  routes/RandomRouteFactory.h
  routes/RateLimiter.cpp
  routes/RateLimiter.h
  routes/RendezvousRouteHelpers.cpp
  routes/RendezvousRouteHelpers.h
  routes/RateLimitRoute.h
  routes/RootRoute.h 
  routes/RouteHandleMap-inl.h 
  routes/RouteHandleMap.h 
  routes/RoutePolicyMap-inl.h 
  routes/RoutePolicyMap.h 
  routes/RouteSelectorMap.h 
  routes/ShadowRoute.h 
  routes/ShadowRoute-inl.h 
  routes/ShadowRouteIf.h 
  routes/ShadowSettings.cpp 
  routes/ShadowSettings.h 
  routes/ShardHashFunc.cpp 
  routes/ShardHashFunc.h 
  routes/ShardSelectionRouteFactory.h 
  routes/ShardSelectionRouteFactory-inl.h 
  routes/ShardSelectionRouteFactory.cpp 
  routes/ShardSplitRoute.cpp 
  routes/ShardSplitRoute.h 
  routes/ShardSplitter.cpp 
  routes/ShardSplitter.h 
  routes/SlowWarmupRoute.h 
  routes/SlowWarmUpRouteSettings.cpp 
  routes/SlowWarmupRouteSettings.h 
  routes/StagingRoute.cpp 
  routes/StagingRoute.h 
  routes/TimeProviderFunc.h 
  routes/WarmUpRoute.cpp 
  routes/WarmUpRoute.h 
  RouterRegistry-impl.h 
  RoutingPrefix.cpp 
  RoutingPrefix.h 
  RuntimeVarsData.cpp 
  RuntimeVarsData.h 
  ServiceInfo-inl.h 
  ServiceInfo.h 
  stat_list.h 
  stats.cpp 
  stats.h 
  ThreadUtil.cpp 
  ThreadUtil.h 
  ThriftAcceptor.h 
  ThriftAcceptor.cpp 
  TkoCounters.h 
  TkoLog.cpp 
  TkoLog.h 
  TkoTracker.cpp 
  TkoTracker.h 
  ExternalStatsHandler.cpp 
  ExternalStatsHandler.h
)

target_link_libraries(libmcrouter_core
  Folly::folly
)

add_executable(mcrouter
  main.cpp
  Server-inl.h
  Server.h
  ServerOnRequest.h
  RequestAclChecker.h
  RequestAclChecker.cpp
  StandaloneConfig.cpp
  StandaloneConfig.h
  StandaloneUtils.cpp
  StandaloneUtils.h
  standalone_options.h
  standalone_options_list.h
)

add_subdirectory(lib)

target_link_libraries(mcrouter
  libmcrouter_core
  libmcrouter
  FBThrift::thriftcpp2
  Folly::folly
  wangle::wangle
  gflags
)
