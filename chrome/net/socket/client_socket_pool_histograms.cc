// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/socket/client_socket_pool_histograms.h"

#include <string>

#include "base/histogram.h"
#include "net/socket/client_socket_handle.h"

namespace net {

ClientSocketPoolHistograms::ClientSocketPoolHistograms(
    const std::string& pool_name) {
  // UMA_HISTOGRAM_ENUMERATION
  socket_type_ = LinearHistogram::FactoryGet("Net.SocketType_" + pool_name, 1,
      ClientSocketHandle::NUM_TYPES, ClientSocketHandle::NUM_TYPES + 1,
      Histogram::kUmaTargetedHistogramFlag);
  // UMA_HISTOGRAM_CUSTOM_TIMES
  request_time_ = Histogram::FactoryTimeGet(
      "Net.SocketRequestTime_" + pool_name,
      base::TimeDelta::FromMilliseconds(1),
      base::TimeDelta::FromMinutes(10),
      100, Histogram::kUmaTargetedHistogramFlag);
  // UMA_HISTOGRAM_CUSTOM_TIMES
  unused_idle_time_ = Histogram::FactoryTimeGet(
      "Net.SocketIdleTimeBeforeNextUse_UnusedSocket_" + pool_name,
      base::TimeDelta::FromMilliseconds(1),
      base::TimeDelta::FromMinutes(6),
      100, Histogram::kUmaTargetedHistogramFlag);
  // UMA_HISTOGRAM_CUSTOM_TIMES
  reused_idle_time_ = Histogram::FactoryTimeGet(
      "Net.SocketIdleTimeBeforeNextUse_ReusedSocket_" + pool_name,
      base::TimeDelta::FromMilliseconds(1),
      base::TimeDelta::FromMinutes(6),
      100, Histogram::kUmaTargetedHistogramFlag);
}

void ClientSocketPoolHistograms::AddSocketType(int type) const {
  socket_type_->Add(type);
}

void ClientSocketPoolHistograms::AddRequestTime(base::TimeDelta time) const {
  request_time_->AddTime(time);
}

void ClientSocketPoolHistograms::AddUnusedIdleTime(base::TimeDelta time) const {
  unused_idle_time_->AddTime(time);
}

void ClientSocketPoolHistograms::AddReusedIdleTime(base::TimeDelta time) const {
  reused_idle_time_->AddTime(time);
}

}  // namespace net
