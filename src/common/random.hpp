#pragma once

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

/**
 * @class RNG
 * @brief Gerador de números aleatórios seguro para threads com estado por
 * thread
 *
 * Esta classe mantém um gerador Mersenne Twister separado para cada thread,
 * eliminando a sobrecarga de sincronização e garantindo resultados
 * reprodutíveis ao usar a mesma semente mestre.
 */
class RNG {
 private:
  std::vector<std::mt19937_64> generators_;  ///< Geradores por thread
  uint64_t master_seed_;                     ///< Semente mestre para reprodutibilidade
  int num_threads_;                          ///< Número de threads configuradas

  /**
   * @brief Inicializa os geradores para todas as threads
   *
   * Usa a semente mestre para gerar de forma determinística sementes únicas
   * para o gerador de cada thread, garantindo reprodutibilidade.
   */
  void init_generators() {
    generators_.reserve(num_threads_);
    std::mt19937_64 seed_gen(master_seed_);

    for (int i = 0; i < num_threads_; ++i) {
      generators_.emplace_back(seed_gen());
    }
  }

 public:
  /**
   * @brief Constrói o RNG com semente aleatória
   * @param threads Número de threads que usarão este RNG
   *
   * Usa std::random_device para gerar uma semente não determinística.
   * Não é adequado para experimentos reprodutíveis.
   */
  explicit RNG(int threads) : master_seed_(std::random_device{}()), num_threads_(threads) { init_generators(); }

  /**
   * @brief Constrói o RNG com semente fixa
   * @param threads Número de threads que usarão este RNG
   * @param seed Semente mestre para sequências aleatórias reprodutíveis
   *
   * Usar a mesma semente produzirá sequências idênticas em execuções,
   * essencial para experimentos reprodutíveis em meta-heurísticas.
   */
  RNG(int threads, uint64_t seed) : master_seed_(seed), num_threads_(threads) { init_generators(); }

  /**
   * @brief Re-inicializa todos os geradores com uma nova semente
   * @param seed Nova semente mestre
   *
   * Útil para rodar múltiplos experimentos independentes com
   * sequências aleatórias diferentes sem recriar o objeto RNG.
   */
  void reseed(uint64_t seed) {
    master_seed_ = seed;
    std::mt19937_64 seed_gen(seed);

    for (int i = 0; i < num_threads_; ++i) {
      generators_[i].seed(seed_gen());
    }
  }

  /**
   * @brief Gera um número inteiro uniforme no intervalo fechado [min, max]
   * @param thread_id ID da thread chamadora (0 a num_threads-1)
   * @param min Valor mínimo (inclusive)
   * @param max Valor máximo (inclusive)
   * @return Número inteiro aleatório em [min, max]
   *
   * @warning thread_id deve estar dentro do intervalo válido, sem verificação
   * de limites
   */
  int uniform_int(int thread_id, int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generators_[thread_id]);
  }

  /**
   * @brief Gera um número real uniforme no intervalo semi-aberto [min, max)
   * @param thread_id ID da thread chamadora (0 a num_threads-1)
   * @param min Valor mínimo (inclusive, padrão 0.0)
   * @param max Valor máximo (exclusive, padrão 1.0)
   * @return Número real aleatório em [min, max)
   *
   * @warning thread_id deve estar dentro do intervalo válido, sem verificação
   * de limites
   */
  double uniform_real(int thread_id, double min = 0.0, double max = 1.0) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(generators_[thread_id]);
  }

  /**
   * @brief Gera um valor de distribuição normal (Gaussiana)
   * @param thread_id ID da thread chamadora (0 a num_threads-1)
   * @param mean Média da distribuição (padrão 0.0)
   * @param stddev Desvio padrão (padrão 1.0)
   * @return Valor aleatório da distribuição N(mean, stddev²)
   *
   * @warning thread_id deve estar dentro do intervalo válido, sem verificação
   * de limites
   */
  double normal(int thread_id, double mean = 0.0, double stddev = 1.0) {
    std::normal_distribution<double> dist(mean, stddev);
    return dist(generators_[thread_id]);
  }

  /**
   * @brief Gera um valor booleano com a probabilidade dada
   * @param thread_id ID da thread chamadora (0 a num_threads-1)
   * @param p Probabilidade de retornar verdadeiro (padrão 0.5)
   * @return true com probabilidade p, false com probabilidade (1-p)
   *
   * @warning thread_id deve estar dentro do intervalo válido, sem verificação
   * de limites
   * @warning p deve estar no intervalo [0.0, 1.0], sem validação
   */
  bool bernoulli(int thread_id, double p = 0.5) {
    std::bernoulli_distribution dist(p);
    return dist(generators_[thread_id]);
  }

  /**
   * @brief Embaralha aleatoriamente os elementos de um vetor
   * @tparam T Tipo do elemento do vetor
   * @param thread_id ID da thread chamadora (0 a num_threads-1)
   * @param vec Vetor a ser embaralhado (modificado no local)
   *
   * Útil para gerar permutações aleatórias em problemas como TSP,
   * problemas de alocação e outras tarefas de otimização combinatória.
   *
   * @warning thread_id deve estar dentro do intervalo válido, sem verificação
   * de limites
   */
  template <typename T>
  void shuffle(int thread_id, std::vector<T>& vec) {
    std::shuffle(vec.begin(), vec.end(), generators_[thread_id]);
  }

  /**
   * @brief Retorna o número de threads configuradas
   * @return Número de threads para as quais este RNG foi inicializado
   */
  [[nodiscard]] int get_num_threads() const { return num_threads_; }

  /**
   * @brief Retorna a semente mestre usada para a inicialização
   * @return Valor da semente mestre
   *
   * Útil para registro e reprodução de resultados experimentais.
   */
  [[nodiscard]] uint64_t get_master_seed() const { return master_seed_; }
};

int sum(int a, int b);
