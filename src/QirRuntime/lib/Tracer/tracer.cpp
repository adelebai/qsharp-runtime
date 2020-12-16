// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <assert.h>

#include "Tracer.hpp"

extern "C"
{
    Result UseZero()
    {
        return reinterpret_cast<Result>(0);
    }

    Result UseOne()
    {
        return reinterpret_cast<Result>(1);
    }

    QUBIT* quantum__rt__qubit_allocate() // NOLINT
    {
        return g_sim->AllocateQubit();
    }

    void quantum__rt__qubit_release(QUBIT* qubit) // NOLINT
    {
        g_sim->ReleaseQubit(qubit);
    }

    // Increments the reference count of a Result pointer.
    void quantum__rt__result_reference(RESULT* r) // NOLINT
    {
        // If we don't have the result in our map, assume it has been allocated by a measurement with refcount = 1,
        // and this is the first attempt to share it.
        std::unordered_map<RESULT*, int>& trackedResults = AllocatedResults();
        auto rit = trackedResults.find(r);
        if (rit == trackedResults.end())
        {
            trackedResults[r] = 2;
        }
        else
        {
            rit->second += 1;
        }
    }

    // Decrements the reference count of a Result pointer and releases the result if appropriate.
    void quantum__rt__result_unreference(RESULT* r) // NOLINT
    {
        // If we don't have the result in our map, assume it has been never shared.
        std::unordered_map<RESULT*, int>& trackedResults = AllocatedResults();
        auto rit = trackedResults.find(r);
        if (rit == trackedResults.end())
        {
            g_sim->ReleaseResult(r);
        }
        else
        {
            const int refcount = rit->second;
            assert(refcount > 0);
            if (refcount == 1)
            {
                trackedResults.erase(rit);
                g_sim->ReleaseResult(r);
            }
            else
            {
                rit->second = refcount - 1;
            }
        }
    }

    bool quantum__rt__result_equal(RESULT* r1, RESULT* r2) // NOLINT
    {
        if (r1 == r2)
        {
            return true;
        }
        return g_sim->AreEqualResults(r1, r2);
    }

    // Returns a string representation of the result.
    QirString* quantum__rt__result_to_string(RESULT* result) // NOLINT
    {
        ResultValue rv = g_sim->GetResultValue(result);
        assert(rv != Result_Pending);

        return (rv == Result_Zero) ? quantum__rt__string_create("Zero") : quantum__rt__string_create("One");
    }

    // Returns a string representation of the qubit.
    QirString* quantum__rt__qubit_to_string(QUBIT* qubit) // NOLINT
    {
        return quantum__rt__string_create(g_sim->QubitToString(qubit).c_str());
    }
}