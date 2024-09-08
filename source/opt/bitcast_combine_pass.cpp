// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "source/opt/bitcast_combine_pass.h"

namespace spvtools {
namespace opt {

Pass::Status BitCastCombinePass::Process() {
  bool modified = false;
  modified = SimplifyBitcast();
  return (modified ? Status::SuccessWithChange : Status::SuccessWithoutChange);
}

bool BitCastCombinePass::SimplifyBitcast() {
  bool modified = false;
  get_module()->ForEachInst([&](Instruction* inst) {
    // bitcast(X, bitcast(Y, A)) -> bitcast(X, A)

    if (inst->opcode() != spv::Op::OpBitcast) {
      return;
    }

    auto valueId = inst->GetOperand(2).AsId();
    assert(valueId != 0);
    auto value = context()->get_def_use_mgr()->GetDef(valueId);

    inst->RemoveOperand(2);
    inst->InsertOperand(2, Operand(value->GetOperand(2)));
    modified = true;
  });

  return modified;
}
}  // namespace opt
}  // namespace spvtools
