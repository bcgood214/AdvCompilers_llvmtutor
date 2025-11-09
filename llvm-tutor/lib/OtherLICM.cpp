#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

struct SimpleLICM : public PassInfoMixin<SimpleLICM> {
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &) {
    errs() << ">>> Inside SimpleLICM run() for loop with header: "
           << L.getHeader()->getName() << "\n";

    for (BasicBlock *BB : L.getBlocks()) {
      errs() << "  BasicBlock: " << BB->getName() << "\n";
      for (Instruction &I : *BB)
        errs() << "    " << I << "\n";
    }

    return PreservedAnalyses::all();
  }
};

llvm::PassPluginLibraryInfo getSimpleLICMPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "simple-licm", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      errs() << "Registering simple-licm callback\n";
      PB.registerPipelineParsingCallback(
        [](StringRef Name, LoopPassManager &LPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "simple-licm") {
            LPM.addPass(SimpleLICM());
            return true;
          }
          return false;
        });
    }
  };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSimpleLICMPluginInfo();
}
