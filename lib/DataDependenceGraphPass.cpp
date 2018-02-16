//
//
//

#include "Utils.hpp"

#include "DataDependenceGraphPass.hpp"

#include "llvm/IR/Function.h"
// using llvm::Function

#include "llvm/Pass.h"
// using llvm::RegisterPass

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses

#include "llvm/Support/CommandLine.h"
// using llvm::cl::opt
// using llvm::cl::desc
// using llvm::cl::location
// using llvm::cl::cat
// using llvm::cl::OptionCategory

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs

#define DEBUG_TYPE "pedigreeddg"

// plugin registration for opt

char pedigree::DataDependenceGraphPass::ID = 0;
static llvm::RegisterPass<pedigree::DataDependenceGraphPass>
    X("pedigree-ddg", PRJ_CMDLINE_DESC("pedigree ddg pass"), false, false);

// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerPedigreeDDGPass(const llvm::PassManagerBuilder &Builder,
                                    llvm::legacy::PassManagerBase &PM) {
  PM.add(new pedigree::DataDependenceGraphPass());

  return;
}

static llvm::RegisterStandardPasses
    RegisterPedigreeDDGPass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                            registerPedigreeDDGPass);

//

static llvm::cl::OptionCategory
    PedigreeDDGPassCategory("Pedigree DDG Pass",
                            "Options for Pedigree DDG pass");

#if PEDIGREE_DEBUG
static llvm::cl::opt<bool, true>
    Debug("pedigree-ddg-debug", llvm::cl::desc("debug pedigree ddg pass"),
          llvm::cl::location(pedigree::utility::passDebugFlag),
          llvm::cl::cat(PedigreeDDGPassCategory));

static llvm::cl::opt<LogLevel, true> DebugLevel(
    "pedigree-ddg-debug-level",
    llvm::cl::desc("debug level for pedigree ddg pass"),
    llvm::cl::location(pedigree::utility::passLogLevel),
    llvm::cl::values(
        clEnumValN(LogLevel::info, "info", "informational messages"),
        clEnumValN(LogLevel::notice, "notice", "significant conditions"),
        clEnumValN(LogLevel::warning, "warning", "warning conditions"),
        clEnumValN(LogLevel::error, "error", "error conditions"),
        clEnumValN(LogLevel::debug, "debug", "debug messages"), nullptr),
    llvm::cl::cat(PedigreeDDGPassCategory));
#endif // PEDIGREE_DEBUG

//

namespace pedigree {

void DataDependenceGraphPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool DataDependenceGraphPass::runOnFunction(llvm::Function &CurFunc) {
  auto ddg = CreateGraph<DataDependenceGraphTy>(CurFunc);

  return false;
}

} // namespace pedigree end