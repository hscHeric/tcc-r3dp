#include <omp.h>

#include <iostream>
#include <vector>

#include "common/random.hpp"

int main() {
  // Número de threads a serem simuladas
  int num_threads = 4;

  // Criação de um objeto RNG com uma semente fixa para reprodutibilidade
  uint64_t seed = 123456789;
  RNG rng(num_threads, seed);

  // Teste de funções de geração de números aleatórios para todas as threads

  // 1. Testando uniform_int (geração de inteiros aleatórios)
  std::cout << "Testando uniform_int (geração de inteiros aleatórios):\n";
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    int random_int = rng.uniform_int(i, 1, 10);  // Intervalo [1, 10]
    std::cout << "Thread " << i << ": " << random_int << '\n';
  }

  // 2. Testando uniform_real (geração de números reais aleatórios)
  std::cout << "\nTestando uniform_real (geração de números reais aleatórios):\n";
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    double random_real = rng.uniform_real(i, 0.0, 1.0);  // Intervalo [0.0, 1.0)
    std::cout << "Thread " << i << ": " << random_real << '\n';
  }

  // 3. Testando normal (distribuição normal / Gaussiana)
  std::cout << "\nTestando normal (distribuição normal):\n";
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    double random_normal = rng.normal(i, 0.0, 1.0);  // Média 0.0, desvio padrão 1.0
    std::cout << "Thread " << i << ": " << random_normal << '\n';
  }

  // 4. Testando bernoulli (distribuição Bernoulli)
  std::cout << "\nTestando bernoulli (distribuição Bernoulli):\n";
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    bool random_bool = rng.bernoulli(i, 0.5);  // Probabilidade 0.5 para verdadeiro
    std::cout << "Thread " << i << ": " << random_bool << '\n';
  }

  // 5. Testando shuffle (embaralhamento de vetor)
  std::cout << "\nTestando shuffle (embaralhamento de vetor):\n";
  std::vector<int> vec = {1, 2, 3, 4, 5};
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    rng.shuffle(i, vec);
    std::cout << "Thread " << i << ": ";
    for (int val : vec) {
      std::cout << val << " ";
    }
    std::cout << '\n';
  }

  // 6. Testando reseed (re-semeando os geradores)
  std::cout << "\nTestando reseed (re-semeando os geradores):\n";
  rng.reseed(987654321);  // Nova semente mestre
#pragma omp parallel for
  for (int i = 0; i < num_threads; ++i) {
    int random_int_after_reseed = rng.uniform_int(i, 1, 10);  // Novo valor após reseed
    std::cout << "Thread " << i << ": " << random_int_after_reseed << '\n';
  }

  // 7. Testando get_num_threads e get_master_seed
  std::cout << "\nTestando get_num_threads e get_master_seed:\n";
  std::cout << "Número de threads: " << rng.get_num_threads() << '\n';
  std::cout << "Semente mestre: " << rng.get_master_seed() << '\n';

  return 0;
}
