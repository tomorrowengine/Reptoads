# SparseToDenseVector

A sparse to dense vector. Useful for having a handle instead of a index for.

## Classes
### sparse_to_dense_vector
A generic vector indexed by handle(aka id) instead of index directly. 
This implementation uses a `unordered_map<handle, index>` as sparse to dense map for quick lookup times and uses `unordered_map<index, SparseToDense::iterator>` as a DenseToSparse map for quick deletion times.

### light_sparse_to_dense_vector
A generic vector indexed by handle(aka id) instead of index directly. 
This implementation only uses a `unordered_map<handle, index>` as sparse to dense map for quick lookup times and does not use any other map for the reverse lookup. This means that deletion complexity is linear because we loop over all elements in the sparse_to_dense map to find a matching key-value pair.
