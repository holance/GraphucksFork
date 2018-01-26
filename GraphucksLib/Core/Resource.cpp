#include "pch.h"
#include "Resource.h"
#include <algorithm>

using namespace Graphucks;
using namespace std;

void DependantObject::RegisterInvalidatable(IInvalidatable* invalidatable)
{
    m_invalidatables.push_back(invalidatable);
}

void DependantObject::NotifyInvalidatables(INVALIDATE_FLAGS flags)
{
    for (auto invalidatableItem : m_invalidatables)
    {
        invalidatableItem->Invalidate(flags);
    }
}


void Graphucks::DependantObject::UnregisterInvalidatable(IInvalidatable* invalidatable)
{
    if(!invalidatable || m_invalidatables.empty())
        return;

    m_invalidatables.erase(remove_if(m_invalidatables.begin(), m_invalidatables.end(), 
        [invalidatable](IInvalidatable* const invalidatableItem)
        {
            if(invalidatable == invalidatableItem)
            {
                return true;
            }

            return false;
        }), m_invalidatables.end());
}
