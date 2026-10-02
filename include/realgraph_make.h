#ifndef REALGRAPH_MAKE_H
#define REALGRAPH_MAKE_H

#include "graph.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Preprocessing and ingestion functions for large real-world graphs (>= 1,000,000 nodes).
 * 
 * Dataset Used: Stanford SNAP roadNet-CA (California Road Network)
 * - Source: Stanford Large Network Dataset Collection (SNAP)
 *   https://snap.stanford.edu/data/roadNet-CA.html
 * - Scale: 1,965,206 nodes, 2,766,607 undirected edges.
 * - Format: Edge list with comments (# FromNodeId \t ToNodeId).
 * 
 * Preprocessing Applied:
 * - Parsing edge-list files, skipping header lines starting with '#'.
 * - Dynamic ID remapping / normalization from sparse raw IDs to contiguous 0-based indices [0, V-1].
 * - Removal of self-loops (u == v).
 * - Multi-edge deduplication.
 * - Bidirectional undirected edge insertion.
 */

/**
 * @brief Adjacency Criterion 1: Full Baseline Road Network.
 * 
 * Criteria Explanation:
 * ---------------------
 * All edges from the raw dataset are ingested. Self-loops are eliminated,
 * and edges are made strictly bidirectional/undirected. Node IDs are remapped
 * contiguously from 0 to V-1.
 * 
 * @param filepath Path to the edge list text file.
 * @return Graph Full road network graph.
 */
Graph load_realgraph_criterion1(const std::string& filepath);

/**
 * @brief Adjacency Criterion 2: Arterial Transit Core (Degree-Threshold Filtering).
 * 
 * Criteria Explanation:
 * ---------------------
 * In urban transport analysis, vertices of degree 1 represent terminal cul-de-sacs or
 * dead ends. Under this criterion, only edges between vertices having degree >= min_degree
 * (default 2) are retained. This prunes peripheral leaves and isolates the core transit arterial
 * network, altering the cycle structure and component distribution.
 * 
 * @param filepath Path to the edge list text file.
 * @param min_degree Minimum degree threshold for edge endpoints (default = 2).
 * @return Graph Core arterial subgraph.
 */
Graph load_realgraph_criterion2(const std::string& filepath, int min_degree = 2);

/**
 * @brief Adjacency Criterion 3: Modular Regional Partitioning (Modular Affinity).
 * 
 * Criteria Explanation:
 * ---------------------
 * Simulates regional jurisdictional / administrative partitioning (e.g., district maintenance
 * zones or alternate-grid traffic divisions). An edge (u, v) is retained if and only if
 * (u % mod_k) == (v % mod_k). This segments the 1.9M+ node graph into mod_k independent
 * structural partitions.
 * 
 * @param filepath Path to the edge list text file.
 * @param mod_k Modulo divisor for partition filtering (default = 2).
 * @return Graph Partitioned modular subgraph.
 */
Graph load_realgraph_criterion3(const std::string& filepath, int mod_k = 2);

#endif // REALGRAPH_MAKE_H
