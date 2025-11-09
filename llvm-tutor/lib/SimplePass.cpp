#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct SimplePass : public PassInfoMixin<SimplePass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs() << "Running SimplePass on function: " << F.getName() << "\n";
    return PreservedAnalyses::all();
  }
};

} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      LLVM_PLUGIN_API_VERSION, "SimplePass", LLVM_VERSION_STRING,
      [](PassBuilder &PB) {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, ModulePassManager &MPM,
               ArrayRef<PassBuilder::PipelineElement>) {
              // Register at the module level so -passes="simple-pass" works
              if (Name == "simple-pass") {
                errs() << "In the block.\n";
                // This adaptor makes the FunctionPass run on every function
                MPM.addPass(createModuleToFunctionPassAdaptor(SimplePass()));
                return true;
              }
              return false;
            });
      }};
}
