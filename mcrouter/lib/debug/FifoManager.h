/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <condition_variable>
#include <memory>
#include <thread>
#include <unordered_map>

#include <boost/filesystem.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <folly/Singleton.h>

#include "mcrouter/lib/debug/Fifo.h"

namespace facebook {
namespace memcache {

/**
 * Manager of fifos.
 */
class FifoManager {
 public:
  ~FifoManager();

  /**
   * Fetches (creates if not found) a fifo by its full base path + threadId.
   * The final path of the returned fifo will have the following format:
   * "{fifoBasePath}.{threadId}".
   * At any given point in time, this instance manages at most one fifo per
   * basePath/threadId pair.
   *
   * @param fifoBasePath  Base path of the fifo.
   * @return              The "thread_local" fifo.
   */
  std::shared_ptr<Fifo> fetchThreadLocal(const std::string& fifoBasePath);

  /**
   * Removes all elements from the fifo manager.
   */
  void clear();

  /**
   * Returns the singleton instance of FifoManager.
   * Note: Keep FifoManager's shared pointer for as little as possible.
   */
  static std::shared_ptr<FifoManager> getInstance();

 private:
  FifoManager();

  std::unordered_map<std::string, std::shared_ptr<Fifo>> fifos_;
  // Note: folly::SharedMutex has caused build issues on Ubuntu 14.04 due to a
  // gcc-4.8 bug. Here, boost::shared_mutex is an appropriate workaround.
  boost::shared_mutex fifosMutex_;

  // Thread that connects to fifos
  std::thread thread_;
  bool running_{true};
  std::mutex mutex_;
  std::condition_variable cv_;

  /**
   * Fetches a fifo by its full path. If the fifo does not
   * exist yet, creates it and returns it to the caller.
   *
   * @param fifoPath  Full path of the fifo.
   * @return          The fifo.
   */
  std::shared_ptr<Fifo> fetch(const std::string& fifoPath);

  /**
   * Finds a fifo by its full path. If not found, returns null.
   *
   * @param fifoPath  Full path of the fifo.
   * @return          The fifo or null if not found.
   */
  std::shared_ptr<Fifo> find(const std::string& fifoPath);

  /**
   * Creates a fifo and stores it into the map.
   *
   * @param fifoPath  Full path of the fifo.
   * @return          The newly created fifo.
   */
  std::shared_ptr<Fifo> createAndStore(const std::string& fifoPath);

  friend class folly::Singleton<FifoManager>;
};
}
} // facebook::memcache
