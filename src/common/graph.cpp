#include "graph.hpp"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

Graph::Graph() noexcept : num_vertices_(0), num_edges_(0) {}

Graph::Graph(size_t n) : num_vertices_(n), num_edges_(0) { adj_list_.resize(n); }

Graph::Graph(const std::string& filepath) : num_vertices_(0), num_edges_(0) {
  namespace fs = std::filesystem;

  // Verifica se o arquivo existe
  if (!fs::exists(filepath)) [[unlikely]] {
    throw std::runtime_error("Arquivo não encontrado: " + filepath);
  }

  std::ifstream file(filepath);
  if (!file.is_open()) [[unlikely]] {
    throw std::runtime_error("Não foi possível abrir o arquivo: " + filepath);
  }

  std::string line;
  size_t line_number = 0;

  std::vector<std::pair<size_t, size_t>> raw_edges;
  std::vector<size_t> vertices;

  // Leitura do arquivo
  while (std::getline(file, line)) {
    ++line_number;

    // Remove espaços em branco no início e fim
    auto first = line.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
      continue;  // linha vazia
    }
    auto last = line.find_last_not_of(" \t\r\n");
    line = line.substr(first, last - first + 1);

    // Comentários
    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::istringstream iss(line);
    size_t u_orig = 0;
    size_t v_orig = 0;

    if (!(iss >> u_orig >> v_orig)) [[unlikely]] {
      throw std::runtime_error("Erro na linha " + std::to_string(line_number) + ": formato inválido (esperado: u v)");
    }

    raw_edges.emplace_back(u_orig, v_orig);
    vertices.push_back(u_orig);
    vertices.push_back(v_orig);
  }

  file.close();
  if (!file) [[unlikely]] {
    throw std::runtime_error("Falha ao fechar o arquivo: " + filepath);
  }

  if (raw_edges.empty()) {
    num_vertices_ = 0;
    num_edges_ = 0;
    adj_list_.clear();
    return;
  }

  std::sort(vertices.begin(), vertices.end());
  vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

  num_vertices_ = vertices.size();
  adj_list_.assign(num_vertices_, {});

  std::unordered_map<size_t, size_t> map_old_to_new;
  map_old_to_new.reserve(num_vertices_);

  for (size_t i = 0; i < num_vertices_; ++i) {
    map_old_to_new[vertices[i]] = i;
  }

  for (const auto& [u_orig, v_orig] : raw_edges) {
    size_t u = map_old_to_new[u_orig];
    size_t v = map_old_to_new[v_orig];

    // Ignora self-loops após remapeamento
    if (u == v) {
      continue;
    }

    // add_edge trata duplicatas
    add_edge(u, v);
  }
}
