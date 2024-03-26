#include "BinaryTree.h"
template<Comparable K, CopyConstructible V>
std::strong_ordering operator<=>(const typename BinaryTree<K, V>::forward_iterator_base& one, const typename BinaryTree<K, V>::forward_iterator_base& two)
{
    return std::strong_ordering();
}
