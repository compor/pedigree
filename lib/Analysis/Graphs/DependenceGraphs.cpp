//
//
//

#include "Config.hpp"

#include "Analysis/Graphs/DependenceGraphs.hpp"

#include "llvm/IR/Instruction.h"
// using llvm::Instruction

#include "llvm/IR/BasicBlock.h"
// using llvm::BasicBlock

namespace pedigree {

template class GenericDependenceNode<llvm::Instruction, BasicDependenceInfo>;
template class GenericDependenceNode<llvm::BasicBlock, BasicDependenceInfo>;

template class GenericDependenceGraph<
    GenericDependenceNode<llvm::Instruction, BasicDependenceInfo>>;

template class GenericDependenceGraph<
    GenericDependenceNode<llvm::BasicBlock, BasicDependenceInfo>>;

} // namespace pedigree end