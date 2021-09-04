#include "binaryninjacore.h"
#include "binaryninjaapi.h"
#include "disasm.h"

using namespace BinaryNinja;

static BNArchitecture *arch = NULL;

extern "C" bool init_backend(const char *arch_name) {
    BNSetBundledPluginDirectory(BINJA_PLUGIN_DIR);
    BNInitPlugins(true);
    arch = BNGetArchitectureByName(arch_name);
    return arch;
}

extern "C" bool is_indirect_branch(uint8_t *insn_data, size_t insn_size) {
    BNInstructionInfo info;
    BNGetInstructionInfo(arch, insn_data, 0 /* addr */, insn_size, &info);
    if (info.branchCount) {
        BNBranchType br = info.branchType[0];
        return (br == BNBranchType::UnresolvedBranch) || (br == BNBranchType::IndirectBranch);
    }
    return false;
}