#include <dynamic_graph/dynamic_connectivity.hpp>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds Duration;

double DurationToSeconds(const Duration& duration) {
  return
    std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

int main() {
  constexpr int64_t kNumVertices{20000};
  constexpr int32_t kIterations{5};
  constexpr int64_t kOperationsPerIteration{kNumVertices};
  constexpr double kProportionOfEdgesToDelete{.5};

  // Initializes graph of `kNumVertices` vertices. For `kIterations` iterations,
  //   - Picks `kOperationsPerIteration` edges and adds all edges that aren't
  //   already in the graph.
  //   - Performs `kOperationsPerIterations / 2` connectivity queries.
  //   - Deletes roughly a `kProportionOfEdgesToDelete` proportion of the edges.
  //   - Performs `kOperationsPerIterations / 2` connectivity queries.

  std::mt19937 rng{0};
  std::uniform_int_distribution<Vertex>
    vertex_distribution{0, kNumVertices-1};
  std::uniform_real_distribution<double> unit_distribution(0, 1);
  DynamicConnectivity graph(kNumVertices);

  std::unordered_set<UndirectedEdge, UndirectedEdgeHash> edges;
  edges.reserve(kOperationsPerIteration);
  std::unordered_set<UndirectedEdge, UndirectedEdgeHash> edges_to_add;
  edges_to_add.reserve(kOperationsPerIteration);
  std::vector<UndirectedEdge> edges_to_delete;
  edges_to_delete.reserve(kOperationsPerIteration);
  int64_t num_edges_added{0};
  int64_t num_edges_deleted{0};
  int64_t num_connectivity_queries{0};
  Duration add_edge_time{0};
  Duration delete_edge_time{0};
  Duration connectivity_query_time{0};
  const auto benchmark_start{Clock::now()};
  for (int32_t iteration = 0; iteration < kIterations; iteration++) {
    {
      edges_to_add.clear();
      for (int64_t i = 0; i < kOperationsPerIteration; i++) {
        const UndirectedEdge e{
          vertex_distribution(rng), vertex_distribution(rng)};
        if (e.first != e.second && edges.find(e) == edges.end()) {
          edges_to_add.emplace(std::move(e));
        }
      }
      const auto add_edge_start{Clock::now()};
      for (const auto& e : edges_to_add) {
        graph.AddEdge(e);
      }
      const auto add_edge_finish{Clock::now()};
      add_edge_time +=
        std::chrono::duration_cast<Duration>(add_edge_finish - add_edge_start);
      num_edges_added += edges_to_add.size();
      edges.insert(edges_to_add.begin(), edges_to_add.end());
    }
    {
      const auto connectivity_query_start{Clock::now()};
      for (int64_t i = 0; i < kOperationsPerIteration / 2; i++) {
        graph.IsConnected(vertex_distribution(rng), vertex_distribution(rng));
      }
      const auto connectivity_query_finish{Clock::now()};
      connectivity_query_time +=
        std::chrono::duration_cast<Duration>(
            connectivity_query_finish - connectivity_query_start);
      num_connectivity_queries += kOperationsPerIteration / 2;
    }
    {
      edges_to_delete.clear();
      for (const auto& e : edges) {
        if (unit_distribution(rng) < kProportionOfEdgesToDelete) {
          edges_to_delete.emplace_back(e);
        }
      }
      const auto delete_edge_start{Clock::now()};
      for (const auto& e : edges_to_delete) {
        graph.DeleteEdge(e);
      }
      const auto delete_edge_finish{Clock::now()};
      delete_edge_time +=
        std::chrono::duration_cast<Duration>(
            delete_edge_finish - delete_edge_start);
      num_edges_deleted += edges_to_delete.size();
      for (const auto& e : edges_to_delete) {
        edges.erase(e);
      }
    }
    {
      const auto connectivity_query_start{Clock::now()};
      for (int64_t i = 0; i < kOperationsPerIteration / 2; i++) {
        graph.IsConnected(vertex_distribution(rng), vertex_distribution(rng));
      }
      const auto connectivity_query_finish{Clock::now()};
      connectivity_query_time +=
        std::chrono::duration_cast<Duration>(
            connectivity_query_finish - connectivity_query_start);
      num_connectivity_queries += kOperationsPerIteration / 2;
    }
  }
  const auto benchmark_end{Clock::now()};

  const int32_t kPrecision{4};
  std::cout << std::setprecision(kPrecision);
  std::cout << "Graph of " << kNumVertices << " vertices.\n";
  std::cout << DurationToSeconds(add_edge_time) << " seconds to add "
    << num_edges_added << " edges.\n";
  std::cout << DurationToSeconds(delete_edge_time) << " seconds to delete "
    << num_edges_deleted << " edges.\n";
  std::cout << DurationToSeconds(connectivity_query_time)
    << " seconds to query connectivity " << num_connectivity_queries
    << " times.\n";
  std::cout << DurationToSeconds(benchmark_end - benchmark_start)
    << " seconds to run benchmark.\n";

  return 0;
}
