#include "LlvmCoverage.h"

/// Helper function for coverage instrumentation
/// \param InstNum the number of the covered instruction being logged
/// \param FileName the adler32 of the file name that has the covered instruction
///        Note that we use the hash because passing strings to LLVM is not trivial
void SRCIRORLlvmCoverage(int InstNum, uint64_t FileName) {
  // TODO: make the write to this file synchronized
  if (LlvmTrace == NULL) {
    LlvmTrace = fopen("/tmp/srciror_llvm_coverage", "a");
  }
  fprintf(LlvmTrace, "%lu:%d\n", FileName, InstNum);
}
