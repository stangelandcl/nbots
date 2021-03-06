#include <stdlib.h>
#include <stdint.h>

#include "nb/memory_bot.h"
#include "nb/container_bot/array.h"
#include "nb/graph_bot/graph.h"
#include "nb/graph_bot/matching/greedy.h"

typedef struct {
	uint32_t id1, id2;
	double w;
} edge_t;


static void init_array(uint32_t *nodal_match, const nb_graph_t *const graph);
static uint32_t count_edges(const nb_graph_t *const graph);
static void set_edges(const nb_graph_t *const graph, edge_t *edges);
static double get_wij(const nb_graph_t *const graph,
		      uint32_t i, uint32_t j);
static int8_t compare_edges(const void *const a, const void *const b);
static void greedy_matching(uint32_t N_edges, const edge_t *edges,
			    uint32_t *nodal_match);

void nb_graph_matching_greedy(const nb_graph_t *const graph,
			      uint32_t *nodal_match)
{
	init_array(nodal_match, graph);
	uint32_t N_edges = count_edges(graph);
	edge_t *edges = nb_allocate_mem(N_edges * sizeof(*edges));
	set_edges(graph, edges);
	nb_qsort(edges, N_edges, sizeof(*edges), compare_edges);
	greedy_matching(N_edges, edges, nodal_match);
	nb_free_mem(edges);
}

static void init_array(uint32_t *nodal_match, const nb_graph_t *const graph)
{
	for (uint32_t i = 0; i < graph->N; i++)
		nodal_match[i] = i;
}

static uint32_t count_edges(const nb_graph_t *const graph)
{
	uint32_t N_edges = 0;
	for (uint32_t i = 0; i < graph->N; i++)
		N_edges += graph->N_adj[i];
	return N_edges / 2;
}

static void set_edges(const nb_graph_t *const graph, edge_t *edges)
{
	uint32_t cnt = 0;
	for (uint32_t id1 = 0; id1 < graph->N; id1++) {
		for (uint32_t i = 0; i < graph->N_adj[id1]; i++) {
			uint32_t id2 = graph->adj[id1][i];
			if (id1 < id2) {
				edges[cnt].id1 = id1;
				edges[cnt].id2 = id2;
				edges[cnt].w = get_wij(graph, id1, i);
				cnt += 1;
			}
		}
	}
}

static double get_wij(const nb_graph_t *const graph,
		      uint32_t i, uint32_t j)
{
	double out;
	if (NULL == graph->wij)
		out = 1.0;
	else
		out = graph->wij[i][j];
	return out;
}

static int8_t compare_edges(const void *const a, const void *const b)
{
	const edge_t *const edg_a = a;
	const edge_t *const edg_b = b;
	int8_t out = 0;
	if (edg_a->w < edg_b->w)
		out = 1;
	else if (edg_a->w > edg_b->w)
		out = -1;
	return out;
}

static void greedy_matching(uint32_t N_edges, const edge_t *edges,
			    uint32_t *nodal_match)
{
	for (uint32_t i = 0; i < N_edges; i++) {
		edge_t edg = edges[i];
		if (edg.w < 1e-6)
			break; /* Does not consider null weights */

		if (nodal_match[edg.id1] == edg.id1 &&
		    nodal_match[edg.id2] == edg.id2) {
			nodal_match[edg.id1] = edg.id2;
			nodal_match[edg.id2] = edg.id1;
		}
	}
}
