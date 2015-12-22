// Copyright (c) 2012-2015, Jeffrey N. Johnson
// All rights reserved.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef POLYGLOT_FE_MESH_H
#define POLYGLOT_FE_MESH_H

#include "core/point.h"
#include "core/serializer.h"
#include "polyglot/polyglot.h"

// This type identifies the various types of (3D) finite elements in an fe_mesh.
typedef enum
{
  FE_INVALID,
  FE_TETRAHEDRON_4, 
  FE_TETRAHEDRON_8, 
  FE_TETRAHEDRON_10, 
  FE_TETRAHEDRON_14,
  FE_PYRAMID_5,
  FE_PYRAMID_13,
  FE_WEDGE_6, 
  FE_WEDGE_15,
  FE_WEDGE_16,
  FE_HEXAHEDRON_8,
  FE_HEXAHEDRON_9,
  FE_HEXAHEDRON_20,
  FE_HEXAHEDRON_27,
  FE_POLYHEDRON
} fe_mesh_element_t;

// This type represents a block of finite elements consisting of a single 
// given type. Element blocks can be used to construct or edit finite element 
// meshes. Note that elements are numbered from 0 to N-1 within an N-element 
// block, but that the index spaces of faces, edges, and nodes exist within 
// the context of the entire underlying fe_mesh.
typedef struct fe_block_t fe_block_t;

// Constructs a new finite element block of the given non-polyhedral type 
// by specifying the nodes that make up each element. elem_node_indices 
// is an array that lists the node indices for each element, in order. The 
// number of nodes per element is defined by the element type. The elem_node_indices
// array is consumed by this function.
fe_block_t* fe_block_new(int num_elements,
                         fe_mesh_element_t type,
                         int* elem_node_indices);

// Constructs a new finite element block of polyhedra
// by specifying the faces that make up each element, their types, and the
// indices of the nodes for each face. num_elem_faces is an array defining the 
// number of faces attached to each element in the block; face_types is an 
// array of types for the faces of each element; face_node_indices is an 
// array that lists the node indices for each face, in order, defined by the 
// face type. All arrays are consumed by this function.
fe_block_t* fe_polyhedral_block_new(int num_elements,
                                    int* num_elem_faces,
                                    int* elem_face_indices,
                                    int* num_face_nodes,
                                    int* face_node_indices);

// Destroys the given finite element block.
void fe_block_free(fe_block_t* block);

// Returns an exact copy of the given finite element block.
fe_block_t* fe_block_clone(fe_block_t* block);

// Returns the type of element in the given block.
fe_mesh_element_t fe_block_element_type(fe_block_t* block);

// Returns the number of elements in the given block.
int fe_block_num_elements(fe_block_t* block);

// Returns the number of nodes in the given element within the block.
int fe_block_num_element_nodes(fe_block_t* block, int elem_index);

// Retrieves the indices of nodes for the given element within the block, 
// copying them into elem_node_indices, which must be large enough to store 
// them.
void fe_block_get_element_nodes(fe_block_t* block, 
                                int elem_index, 
                                int* elem_nodes);

// Returns the number of faces in the given element within the block.
int fe_block_num_element_faces(fe_block_t* block, int elem_index);

// Retrieves the indices of faces for the given element within the block, 
// copying them into elem_face_indices, which must be large enough to store 
// them.
void fe_block_get_element_faces(fe_block_t* block, 
                                int elem_index, 
                                int* elem_faces);

// Returns the number of edges in the given element within the block.
int fe_block_num_element_edges(fe_block_t* block, int elem_index);

// Retrieves the indices of edges for the given element within the block, 
// copying them into elem_edge_indices, which must be large enough to store 
// them.
void fe_block_get_element_edges(fe_block_t* block, 
                                int elem_index, 
                                int* elem_edges);

// Returns a serializer object that can read/write finite element blocks 
// from/to byte arrays.
serializer_t* fe_block_serializer();

// This type represents an unstructured finite element mesh consisting of 
// blocks of elements, faces, edges, and nodes. It exists primarily to 
// import/manipulate/export meshes that were generated by finite element 
// tools. Its design is similar to that of the mesh representation in Sandia's
// Exodus library, though it is not tied exclusively to that format.
typedef struct fe_mesh_t fe_mesh_t;

// Construct a new finite element mesh on the given MPI communicator, 
// with the given number of nodes (to be associated with elements).
fe_mesh_t* fe_mesh_new(MPI_Comm comm, int num_nodes);

// Destroys the given finite element mesh.
void fe_mesh_free(fe_mesh_t* mesh);

// Returns an exact copy of the given finite element mesh.
fe_mesh_t* fe_mesh_clone(fe_mesh_t* mesh);

// Adds the given element block to the mesh. Each element block defines its 
// set of elements by connecting nodes in the underlying mesh, so each of 
// these nodes must be available.
void fe_mesh_add_block(fe_mesh_t* mesh, 
                       const char* name,
                       fe_block_t* block);

// Returns the number of blocks in the fe_mesh.
int fe_mesh_num_blocks(fe_mesh_t* mesh);

// Traverses the element blocks in the fe_mesh. Reset the traversal by 
// setting *pos to 0.
bool fe_mesh_next_block(fe_mesh_t* mesh, 
                        int* pos, 
                        char** block_name, 
                        fe_block_t** block);

// Returns the number of elements in the fe_mesh.
int fe_mesh_num_elements(fe_mesh_t* mesh);

// Returns the number of nodes in the given element within the mesh.
int fe_mesh_num_element_nodes(fe_mesh_t* mesh, int elem_index);

// Retrieves the indices of nodes for the given element within the mesh, 
// copying them into elem_node_indices, which must be large enough to store 
// them.
void fe_mesh_get_element_nodes(fe_mesh_t* mesh, 
                               int elem_index, 
                               int* elem_nodes);

// Returns the number of faces in the given element within the mesh.
int fe_mesh_num_element_faces(fe_mesh_t* mesh, int elem_index);

// Retrieves the indices of faces for the given element within the mesh, 
// copying them into elem_face_indices, which must be large enough to store 
// them.
void fe_mesh_get_element_faces(fe_mesh_t* mesh, 
                               int elem_index, 
                               int* elem_faces);

// Returns the number of edges in the given element within the mesh.
int fe_mesh_num_element_edges(fe_mesh_t* mesh, int elem_index);

// Retrieves the indices of edges for the given element within the mesh, 
// copying them into elem_edge_indices, which must be large enough to store 
// them.
void fe_mesh_get_element_edges(fe_mesh_t* mesh, 
                               int elem_index, 
                               int* elem_edges);

// Returns the number of faces in the fe_mesh.
int fe_mesh_num_faces(fe_mesh_t* mesh);

// Returns the number of edges in the given face within an fe_mesh.
int fe_mesh_num_face_edges(fe_mesh_t* mesh,
                           int face_index);

// Retrieves the indices of edges for the given face within the mesh, 
// copying them into face_edge_indices, which must be large enough to store 
// them.
void fe_mesh_get_face_edges(fe_mesh_t* mesh, 
                            int face_index, 
                            int* face_edges);

// Returns the number of nodes in the given face within an fe_mesh.
int fe_mesh_num_face_nodes(fe_mesh_t* mesh,
                           int face_index);

// Retrieves the indices of nodes for the given face within the mesh, 
// copying them into face_node_indices, which must be large enough to store 
// them.
void fe_mesh_get_face_nodes(fe_mesh_t* mesh, 
                            int face_index, 
                            int* face_nodes);

// Returns the number of edges in the fe_mesh.
int fe_mesh_num_edges(fe_mesh_t* mesh);

// Retrieves the indices of nodes for the given edge within the mesh, 
// copying them into edge_node_indices, which must be large enough to store 
// them.
void fe_mesh_get_edge_nodes(fe_mesh_t* mesh, 
                            int edge_index, 
                            int* edge_nodes);

// Returns the number of nodes in the fe_mesh.
int fe_mesh_num_nodes(fe_mesh_t* mesh);

// Returns the number of nodes in the given edge within an fe_mesh.
int fe_mesh_num_edge_nodes(fe_mesh_t* mesh,
                           int edge_index);

// Returns an internal pointer to the set of points defining the positions 
// of the nodes within the mesh.
point_t* fe_mesh_node_coordinates(fe_mesh_t* mesh);

// Returns a serializer object that can read/write finite element meshes 
// from/to byte arrays.
serializer_t* fe_mesh_serializer();

#endif

