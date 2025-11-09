/* SimpleLICM.cpp
 *
 * This pass hoists loop-invariant code before the loop when it is safe to do so.
 *
 * Compatible with New Pass Manage
*/

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/CFG.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ValueTracking.h"

#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

using namespace llvm;

struct SimpleLICM : public PassInfoMixin<SimpleLICM> {
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR,
                        LPMUpdater &) {
    DominatorTree &DT = AR.DT;

    errs() << "Hello, SimpleLICM!\n";

    BasicBlock *Preheader = L.getLoopPreheader();
    if (!Preheader) {
      errs() << "No preheader, skipping loop\n";
      return PreservedAnalyses::all();
    }

    SmallPtrSet<Instruction *, 8> InvariantSet;
    bool Change = true;

    // Worklist algorithm to identify loop invariant instructions
    /*************************************/

    while (Change) {
      Change = false;
      for (BasicBlock *BB : L.getBlocks()) {
        for (Instruction &I : *BB) {

          if (isa<PHINode>(&I) || I.isTerminator() || InvariantSet.count(&I)) {
            continue;
          }

          // if (InvariantSet.count(&I)) {
          //   continue;
          // }

          bool isInvariant = true;
          for (Use &Op : I.operands()) {
            Value *V = Op.get();
            if (Instruction *OpInst = dyn_cast<Instruction>(V)) {
              if (L.contains(OpInst) && !InvariantSet.count(OpInst)) {
                isInvariant = false;
                break;
              }
            }
          }

          if (!isInvariant) {
            continue;
          }

          if (I.mayHaveSideEffects() || I.mayReadOrWriteMemory()) {
            continue;
          }

          InvariantSet.insert(&I);
          errs() << "  Found loop invariant: " << I << "\n";
          Change = true;
        }
      }
    }
    /*************************************/ 

    // Actually hoist the instructions
    for (Instruction *I : InvariantSet) {
      errs() << "From invariant set: " << *I << "\n";
      // Student note: I removed the call to dominatesAllLoopExits
      if (isSafeToSpeculativelyExecute(I)) {
        errs() << "Hoisting: " << *I << "\n";
        I->moveBefore(Preheader->getTerminator());
      }
    }

    return PreservedAnalyses::none();
  }

  bool dominatesAllLoopExits(Instruction *I, Loop *L, DominatorTree &DT) {
    SmallVector<BasicBlock *, 8> ExitBlocks;
    L->getExitBlocks(ExitBlocks);
    for (BasicBlock *EB : ExitBlocks) {
      if (!DT.dominates(I, EB))
        return false;
    }
    return true;
  }
};

llvm::PassPluginLibraryInfo getSimpleLICMPluginInfo() {
  errs() << "SimpleLICM plugin: getSimpleLICMPluginInfo() called\n";
  return {LLVM_PLUGIN_API_VERSION, "simple-licm", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, LoopPassManager &LPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "simple-licm") {
                    LPM.addPass(SimpleLICM());
                    return true;
                  }                
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  errs() << "SimpleLICM plugin: llvmGetPassPluginInfo() called\n";
  return getSimpleLICMPluginInfo();
}
