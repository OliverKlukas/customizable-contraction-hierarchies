#include <utils/id_mapper.hpp>

OptimizedKit::IdMapper::IdMapper(const OptimizedKit::Filter &filter) {
    unsigned localId = 0;
    for (bool globalId : filter) {
        if (globalId) {
            mapping.push_back(localId);
            ++localId;
        } else {
            mapping.push_back(-1);
        }
    }
    localIdCount = localId;
}

unsigned OptimizedKit::IdMapper::toLocal(unsigned globalId) const {
    assert(globalId < mapping.size());
    return mapping[globalId];
}

void OptimizedKit::IdMapper::remove(const OptimizedKit::Filter &filter) {
    removeElementsByFilterInplace(mapping, filter);
    localIdCount -= std::count(filter.begin(), filter.end(), true);
}
