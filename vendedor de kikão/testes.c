#include<stdio.h>
#include<conio.h>
#include<stdlib.h>// necessário p/ as funções rand() e srand()
#include<time.h>//necessário p/ função time()

int main(void)
{
  time_t start_time, end_time;
  double elapsed_time;

    // Obter o tempo de início
  start_time = time(NULL);

    // Realizar alguma tarefa que você deseja medir o tempo
    // Exemplo: Loop de contagem até 1 milhão
  for (int i = 0; i < 1000000; i++) {
        // Faz alguma coisa...
  }

    // Obter o tempo de término
  end_time = time(NULL);

    // Calcular o tempo decorrido em segundos
  elapsed_time = difftime(end_time, start_time);

  printf("Tempo decorrido: %.2f segundos\n", elapsed_time);




  //int i;
  //int tempo_chegada_us = (rand() % 451 + 50) * 1000;
  //printf("%d", tempo_chegada_us);
  
  //printf("Gerando 10 valores aleatorios:\n\n");
  

  /* srand(time(NULL)) objetiva inicializar o gerador de números aleatórios
  com o valor da função time(NULL). Este por sua vez, é calculado
  como sendo o total de segundos passados desde 1 de janeiro de 1970
  até a data atual.
  Desta forma, a cada execução o valor da "semente" será diferente.
  */
  //srand(time(NULL));
  /*
  for (i=0; i < 100; i++)
  {
    // gerando valores aleatórios na faixa de 0 a 100
    printf("%d ", (rand() % 450) + 50);
  }
  */
  return 0;
}