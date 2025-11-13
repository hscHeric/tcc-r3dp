#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <vector>

/// @brief Representa um grafo simples (não-dirigido, sem pesos) usando lista de adjacência.
///
/// Um grafo simples não possui:
/// - Arestas direcionadas (se (u,v) existe, então (v,u) também existe)
/// - Pesos nas arestas
/// - Self-loops (arestas de um vértice para ele mesmo)
/// - Arestas múltiplas entre o mesmo pair de vértices
///
/// @warning Os vértices são numerados de 0 a n-1.
class Graph {
 private:
  size_t num_vertices_;
  size_t num_edges_;
  std::vector<std::vector<size_t>> adj_list_;

 public:
  /// @brief Construtor padrão. Cria um grafo vazio.
  Graph() noexcept;

  /// @brief Cria um grafo com n vértices.
  /// @param n Número de vértices.
  explicit Graph(size_t n);

  /// @brief Constrói o grafo a partir de um arquivo.
  ///
  /// O arquivo deve ter o formato:
  /// - Linhas tem: pares de vértices: u v
  ///
  /// @param filepath Caminho para o arquivo.
  /// @throws std::runtime_error Se o arquivo não puder ser aberto ou estiver malformado.
  explicit Graph(const std::string& filepath);

  /// @brief Retorna o número de vértices.
  /// @return Número de vértices.
  [[nodiscard]] constexpr size_t get_num_vertices() const noexcept;

  /// @brief Retorna o número de arestas.
  /// @return Número de arestas.
  [[nodiscard]] constexpr size_t get_num_edges() const noexcept;

  /// @brief Adiciona uma aresta entre u e v.
  /// @param u Primeiro vértice.
  /// @param v Segundo vértice.
  /// @throws std::out_of_range Se u ou v forem inválidos.
  /// @throws std::invalid_argument Se u == v (self-loop).
  /// @warning Se a aresta já existir, nada é feito.
  void add_edge(size_t u, size_t v);

  /// @brief Remove a aresta entre u e v.
  /// @param u Primeiro vértice.
  /// @param v Segundo vértice.
  /// @throws std::out_of_range Se u ou v forem inválidos.
  /// @warning Se a aresta não existir, nada é feito.
  void remove_edge(size_t u, size_t v);

  /// @brief Verifica se existe aresta entre u e v.
  /// @param u Primeiro vértice.
  /// @param v Segundo vértice.
  /// @return true se a aresta existe, false caso contrário.
  [[nodiscard]] bool has_edge(size_t u, size_t v) const noexcept;

  /// @brief Adiciona um novo vértice ao grafo.
  /// @warning O índice do novo vértice será o número atual de vértices.
  void add_vertex() noexcept;

  /// @brief Retorna o grau do vértice.
  /// @param v Vértice.
  /// @return Grau do vértice.
  /// @throws std::out_of_range Se v for inválido.
  [[nodiscard]] size_t degree(size_t v) const;

  /// @brief Obtém a lista de vizinhos de v.
  /// @param v Vértice.
  /// @return Referência constante para a lista de vizinhos.
  /// @throws std::out_of_range Se v for inválido.
  [[nodiscard]] const std::vector<size_t>& neighbors(size_t v) const;

  /// @brief Obtém um span com os vizinhos de v.
  /// @param v Vértice.
  /// @return Span sobre os vizinhos.
  /// @throws std::out_of_range Se v for inválido.
  [[nodiscard]] std::span<const size_t> neighbors_span(size_t v) const;

  /// @brief Determina se o grafo é conexo.
  /// @return true se for conexo, false caso contrário.
  [[nodiscard]] bool is_connected() const;

  /// @brief Obtém a componente conexa contendo o vértice dado.
  /// @param start Vértice inicial.
  /// @return Vetor ordenado dos vértices da componente.
  /// @throws std::out_of_range Se start for inválido.
  [[nodiscard]] std::vector<size_t> get_connected_component(size_t start) const;

  /// @brief Obtém todas as componentes conexas do grafo.
  /// @return Vetor contendo cada componente.
  [[nodiscard]] std::vector<std::vector<size_t>> get_all_connected_components() const;

  /// @brief Limpa o grafo, removendo todos os vértices e arestas.
  void clear() noexcept;

  /// @brief Reserva capacidade para os vizinhos de um vértice.
  /// @param v Vértice.
  /// @param capacity Capacidade desejada.
  /// @throws std::out_of_range Se v for inválido.
  void reserve_neighbors(size_t v, size_t capacity);

  /// @brief Calcula a densidade do grafo.
  /// @return Densidade entre 0.0 e 1.0.
  [[nodiscard]] double density() const noexcept;

  /// @brief Retorna o grau máximo do grafo.
  /// @return Maior grau.
  [[nodiscard]] size_t max_degree() const noexcept;

  /// @brief Retorna o grau mínimo do grafo.
  /// @return Menor grau.
  [[nodiscard]] size_t min_degree() const noexcept;

  /// @brief Calcula o grau médio do grafo.
  /// @return Grau médio.
  [[nodiscard]] double average_degree() const noexcept;
};
