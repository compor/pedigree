//
//
//

#include "Utils.hpp"

#include "Analysis/Passes/DDGraphPass.hpp"

#include "Analysis/Creational/DDGraphBuilder.hpp"

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

#define DEBUG_TYPE "pedigree-ddg"

namespace llvm {
class Function;
} // namespace llvm end

// plugin registration for opt

char pedigree::DDGraphPass::ID = 0;
static llvm::RegisterPass<pedigree::DDGraphPass>
    X("pedigree-ddg", PRJ_CMDLINE_DESC("pedigree ddg pass"), false, false);

// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerPedigreeDDGraphPass(const llvm::PassManagerBuilder &Builder,
                                        llvm::legacy::PassManagerBase &PM) {
  PM.add(new pedigree::DDGraphPass());

  return;
}

static llvm::RegisterStandardPasses
    RegisterPedigreeDDGraphPass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                                registerPedigreeDDGraphPass);

//

static llvm::cl::OptionCategory
    PedigreeDDGraphPassCategory("Pedigree DDGraph Pass",
                                "Options for Pedigree DDGraph pass");

#if PEDIGREE_DEBUG
static llvm::cl::opt<bool, true>
    Debug("pedigree-ddg-debug", llvm::cl::desc("debug pedigree ddg pass"),
          llvm::cl::location(pedigree::utility::passDebugFlag),
          llvm::cl::cat(PedigreeDDGraphPassCategory));

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
    llvm::cl::cat(PedigreeDDGraphPassCategory));
#endif // PEDIGREE_DEBUG

//

namespace pedigree {

void DDGraphPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool DDGraphPass::runOnFunction(llvm::Function &CurFunc) {
  DDGraphBuilder builder{};
  Graph = builder.setUnit(CurFunc).build();

  return false;
}

} // namespace pedigree end
