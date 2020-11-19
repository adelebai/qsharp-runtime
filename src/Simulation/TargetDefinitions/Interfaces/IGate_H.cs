// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Intrinsic.Interfaces
{
    public interface IGate_H : IOperationFactory
    {
        Func<Qubit, QVoid> H_Body();

        Func<(IQArray<Qubit>, Qubit), QVoid> H_ControlledBody();
    }
}