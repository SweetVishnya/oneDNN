/*******************************************************************************
* Copyright 2022-2023 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <CL/cl.h>

#include "common/c_types_map.hpp"
#include "gpu/ocl/ocl_utils.hpp"

namespace dnnl {
namespace impl {
namespace gpu {
namespace ocl {

struct ocl_stream_t;

void notify_before_exec();
void register_profile_event(
        ocl_wrapper_t<cl_event> &event, const ocl_stream_t *stream);
status_t get_profile_info(int *num_entries, uint64_t *nsecs, uint64_t *cycles);

status_t reset_profiling();

} // namespace ocl
} // namespace gpu
} // namespace impl
} // namespace dnnl
