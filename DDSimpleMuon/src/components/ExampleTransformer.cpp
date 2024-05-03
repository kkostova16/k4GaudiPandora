/*
 * Copyright (c) 2020-2023 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Gaudi/Property.h"
#include "GaudiAlg/Transformer.h"

// Define BaseClass_t
#include "k4FWCore/BaseClass.h"

#include <string>

struct ExampleTransformer final : Gaudi::Functional::Transformer<int(const int&), BaseClass_t> {
  ExampleTransformer(const std::string& name, ISvcLocator* svcLoc)
      : Transformer(name, svcLoc, KeyValue("ExampleTransformerInputLocation", "/InputExampleInt"),
                    {KeyValue("ExampleTransformerOutputLocation", "/OutputExampleInt")}) {}

  int operator()(const int& input) const override {
    info() << "ExampleInt = " << input << endmsg;
    return input + 1;
  }
};

DECLARE_COMPONENT(ExampleTransformer)
