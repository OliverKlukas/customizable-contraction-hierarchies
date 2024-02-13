#ifndef OPTIMIZEDKIT_TYPES_HPP
#define OPTIMIZEDKIT_TYPES_HPP

#include <vector>

namespace OptimizedKit {
    typedef unsigned VertexId;
    typedef unsigned EdgeId;
    typedef std::vector<VertexId> Order;
    typedef std::vector<bool> Separator;
    typedef std::vector<VertexId> VertexMapping;
    typedef std::vector<bool> Filter;
}

#endif //OPTIMIZEDKIT_TYPES_HPP
