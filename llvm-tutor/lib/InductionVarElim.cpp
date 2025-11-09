#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {
  class InductionVarElim : public PassInfoMixin<InductionVarElim> {
    public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

      bool Changed = false;

      for (Loop *L : LI) {
        Changed |= eliminateLoop(L, SE);
      }

      return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

    private:
    bool eliminateLoop(Loop *L, ScalarEvolution &SE) {
      bool Changed = false;

      DataLayout DL = L->getHeader()->getModule()->getDataLayout();
      SCEVExpander Expander(SE, DL, "ive");

      for (BasicBlock *BB : L->blocks()) {
        for (auto I = BB->begin(), E = BB->end(); I != E;) {
          Instruction *Inst = &*I++;
          auto *PN = dyn_cast<PHINode>(Inst);
          if (!PN || !PN->getType()->isIntegerTy()) {
            continue;
          }

          const SCEV *S = SE.getSCEV(PN);
          if (auto *AR = dyn_cast<SCEVAddRecExpr>(S)) {
            if (const SCEVConstant *StepC = 
              dyn_cast<SCEVConstant>(AR->getStepRecurrence(SE))) {
                if (StepC->getAPInt().getSExtValue() == 1) {
                  continue;
                }
              }

          Value *NewExpr = Expander.expandCodeFor(AR, PN->getType(),
            L->getHeader()->getFirstNonPHI());
          
          PN->replaceAllUsesWith(NewExpr);
          PN->eraseFromParent();

          errs() << "Replaced PHI with " << *NewExpr << "\n";

          Changed = true;
          break;
        }
      }
    }

      for (Loop *SubL : L->getSubLoops()) {
        Changed |= eliminateLoop(SubL, SE);
      }

      return Changed;
    }
  };
}

llvm::PassPluginLibraryInfo getInductionVarElimPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "InductionVarElim",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "induction-var-elim") {
                    FPM.addPass(InductionVarElim());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getInductionVarElimPluginInfo();
}