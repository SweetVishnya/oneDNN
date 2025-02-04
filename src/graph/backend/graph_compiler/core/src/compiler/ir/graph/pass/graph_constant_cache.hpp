/*******************************************************************************
 * Copyright 2023 Intel Corporation
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

#ifndef GRAPH_BACKEND_GRAPH_COMPILER_CORE_SRC_COMPILER_IR_GRAPH_PASS_GRAPH_CONSTANT_CACHE_HPP
#define GRAPH_BACKEND_GRAPH_COMPILER_CORE_SRC_COMPILER_IR_GRAPH_PASS_GRAPH_CONSTANT_CACHE_HPP

#include <memory>
#include <compiler/ir/statics_table.hpp>
#include <unordered_map>

namespace dnnl {
namespace impl {
namespace graph {
namespace gc {

struct cached_const_graph_tensor;
struct const_graph_tensor_cache;
class sc_graph_t;
struct graph_weak_ptr_hasher {
    size_t operator()(const std::weak_ptr<sc_graph_t> &v) const;
};
struct graph_weak_ptr_cmper {
    bool operator()(const std::weak_ptr<sc_graph_t> &v1,
            const std::weak_ptr<sc_graph_t> &v2) const;
};

using graph_weak_ptr_map = std::unordered_map<std::weak_ptr<sc_graph_t>,
        std::weak_ptr<cached_const_graph_tensor>, graph_weak_ptr_hasher,
        graph_weak_ptr_cmper>;

using tensor_id_map = std::unordered_map<uint64_t,
        std::weak_ptr<cached_const_graph_tensor>>;

struct cached_const_graph_tensor {
    std::shared_ptr<sc_graph_t> dependency_;
    // the data pointer is initialized null. It will be later updated when
    // finishing visiting the whole graph in graph_constant_input_folding pass.
    // The update of this pointer is protected by
    // shared_global_data_allocator_t::lock_
    void *buf_ = nullptr;
    size_t size_;
    graph_weak_ptr_map::iterator graph_iter_;
    tensor_id_map::iterator id_iter_;
    std::shared_ptr<const_graph_tensor_cache> cache_owner_;
    // the base pointer of buf_. buf_ may be cut from a larger buffer buf_base_.
    std::shared_ptr<void> buf_base_;
    cached_const_graph_tensor(const std::shared_ptr<sc_graph_t> &dep,
            size_t buf_size,
            const std::shared_ptr<const_graph_tensor_cache> &owner)
        : dependency_ {dep}, size_ {buf_size}, cache_owner_ {owner} {}
    ~cached_const_graph_tensor();
};

namespace op_attr_key {
constexpr const char *const_input_cache = "temp.const_input_cache";
}

} // namespace gc
} // namespace graph
} // namespace impl
} // namespace dnnl

#endif
