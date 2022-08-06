// EPOS Scheduler Test Program

#include <machine/display.h>
#include <time.h>
#include <synchronizer.h>
#include <process.h>

using namespace EPOS;

Thread *thread[8];

OStream cout;

int CPU_bound();
int IO_bound();

int main()
{

  thread[0] = new Thread(&CPU_bound);
  thread[1] = new Thread(&CPU_bound);
  thread[2] = new Thread(&CPU_bound);
  thread[3] = new Thread(&CPU_bound);
  thread[4] = new Thread(&IO_bound);
  thread[5] = new Thread(&IO_bound);
  thread[6] = new Thread(&IO_bound);
  thread[7] = new Thread(&IO_bound);

  // Nesse teste criamos 8 threads, e por padrão todas são criadas como CPU_BOUND (escolha de projeto)
  // Nosso teste se baseia em verificar se o tipo da thread (CPU_BOUND ou IO_BOUND) está respeitando a regra
  // de migração de cores, ou seja, se e a thread é do tipo CPU_BOUND ela deve ser alocada à primeira metade
  // de cores (0, 1, 2, 3) e se ela é IO_BOUND, à segunda metade (4, 5, 6, 7).
  for (int i = 0; i < 8; i++)
  {
    int core = thread[i]->join();

    if (i < 4){ // indica que é CPU_BOUND
      cout << "Thread do tipo CPU_BOUND deveria estar no core 0 ou 1 e está no core " << core << endl;
    }
    else { // indica que é IO_BOUND
      cout << "Thread do tipo IO_BOUND deveria estar no core 3 ou 4 e está no core " << core << endl;
    }
  }

  for (int i = 0; i < 8; i++)
  {
    delete thread[i];
  }

  return 0;
}

int CPU_bound()
{

  int n1 = 0, n2 = 1, result;
  for (size_t i = 0; i < 3000; i++)
  {
    if (i <= 1)
      result = i;
    else
    {
      result = n1 + n2;
      n1 = n2;
      n2 = result;
    }
  }

  return CPU::id();
}

int IO_bound()
{
  for (size_t i = 0; i < 4; i++)
  {

    Alarm::delay(10000);

  }

  return CPU::id();
}