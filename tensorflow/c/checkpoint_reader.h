/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_C_CHECKPOINT_READER_H
#define TENSORFLOW_C_CHECKPOINT_READER_H

#include "tensorflow/c/tf_status_helper.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/util/tensor_bundle/tensor_bundle.h"
#include "tensorflow/core/util/tensor_slice_reader.h"

namespace tensorflow {

namespace checkpoint {

class TensorSliceReader;

// A wrapper around BundleReader (for V2 checkpoints) and
// checkpoint::TensorSliceReader (for V1), that is more easily SWIG wrapped for
// other languages.
//
// The class currently only interacts with single-slice (i.e., non-partitioned)
// variables.
class CheckpointReader {
 public:
  CheckpointReader(const string& filepattern, TF_Status* out_status);
  ~CheckpointReader();

  bool HasTensor(const string& name) const;
  const string DebugString() const;

  const TensorSliceReader::VarToShapeMap& GetVariableToShapeMap() const;

  // Attempts to look up the tensor named "name" and stores the found result in
  // "out_tensor".
  void GetTensor(const string& name,
                 std::unique_ptr<tensorflow::Tensor>* out_tensor,
                 TF_Status* out_status) const;

 private:
  // Uses "v2_reader_" to build a "var name -> shape" map; owned by caller.
  // REQUIRES: "v2_reader_ != nullptr && v2_reader_.status().ok()".
  TensorSliceReader::VarToShapeMap* BuildV2VarToShapeMap();

  // Invariant: exactly one of "reader_" and "v2_reader_" is non-nullptr.
  TensorSliceReader* reader_;                               // Owned.
  BundleReader* v2_reader_;                                 // Owned.
  TensorSliceReader::VarToShapeMap* var_to_shape_map_ptr_;  // Owned.

  TF_DISALLOW_COPY_AND_ASSIGN(CheckpointReader);
};

}  // namespace checkpoint
}  // namespace tensorflow

#endif  // TENSORFLOW_C_CHECKPOINT_READER_H
