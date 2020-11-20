﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using System.Runtime.InteropServices;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Simulation.Simulators
{
    public partial class QuantumSimulator
    {
        public Func<Qubit, QVoid> Reset_Body() => (q1) =>
        {
            // The native simulator doesn't have a reset operation, so simulate
            // it via an M follow by a conditional X.
            this.CheckQubit(q1);
            var res = M(this.Id, (uint)q1.Id);
            if (res == 1)
            {
                X(this.Id, (uint)q1.Id);
            }

            return QVoid.Instance;
        };
    }
}
