//
//
//

#ifndef DATADEPENDENCEGRAPH_HPP
#define DATADEPENDENCEGRAPH_HPP

#include "llvm/IR/Instruction.h"
// using llvm::Instruction

#include "boost/graph/adjacency_list.hpp"
// using boost::adjacency_list
// using boost::vecS
// using boost::directedS

#include "boost/graph/labeled_graph.hpp"
// using boost::labeled_graph

#include <cstdint>
// using uint8_t

namespace pedigree {

using namespace boost;

enum class DependenceType : uint8_t {
  unknown = 0,
  flow = 1,
  anti = 2,
  out = 4,
};

enum class DependenceOrigin : uint8_t {
  unknown = 0,
  data = 1,
  memory = 2,
  control = 4,
};

template <typename VertexPropertyTy, typename EdgePropertyTy>
using UnderlyingGraphTy =
    adjacency_list<vecS, vecS, directedS, VertexPropertyTy, EdgePropertyTy>;

template <typename VertexPropertyTy, typename EdgePropertyTy>
using DependenceGraphTy =
    labeled_graph<UnderlyingGraphTy<VertexPropertyTy, EdgePropertyTy>,
                  llvm::Instruction *>;

//

struct DataDependenceVertex {
  llvm::Instruction *instruction;

  explicit DataDependenceVertex(
      const llvm::Instruction *ins = nullptr) noexcept {
    instruction = const_cast<llvm::Instruction *>(ins);
  }
};

struct DataDependenceEdge {
  DependenceType type;
  DependenceOrigin origin;
};

using DataDependenceGraphTy =
    DependenceGraphTy<DataDependenceVertex, DataDependenceEdge>;

template <typename Graph>
Graph CreateGraph(const llvm::Function &Func) noexcept {
  Graph g{};

  for (auto &bb : Func)
    for (auto &ins : bb) {
      auto src = add_vertex(&ins, DataDependenceVertex(&ins), g);
      for (auto &u : ins.uses()) {
        // TODO: this adds all instructions encounter as vertices
        // provide decision option to user
        auto *user = llvm::dyn_cast<llvm::Instruction>(u.getUser());
        if (user)
          auto dst = add_vertex(user, DataDependenceVertex(user), g);
      }
    }

  return g;
}

} // namespace pedigree end

#endif // header