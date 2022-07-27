// EPOS Scheduler Test Program

#include <machine/display.h>
#include <time.h>
#include <synchronizer.h>
#include <process.h>

using namespace EPOS;

Thread *thread[6];

OStream cout;

int CPU_bound();
int IO_bound();

int main()
{
  thread[0] = new Thread(&CPU_bound);
  thread[1] = new Thread(&CPU_bound);
  thread[2] = new Thread(&CPU_bound);

  thread[3] = new Thread(&IO_bound);
  thread[4] = new Thread(&IO_bound);
  thread[5] = new Thread(&IO_bound);

  // Nesse teste criamos 6 threads, e por padrão todas são criadas como CPU_BOUND (escolha de projeto)
  // Nosso teste se baseia em verificar a alteração do tipo da thread, em específico as três últimas, 
  // para IO_BOUND. Essa mudança ocorre quando a thread entra no estado waiting (sleep()).
  // Para a verificação imprimimos para as três primeiras threads um "C" indicando CPU_BOUND acompanhado do tipo
  // com o qual ela retornou, nesse caso esperamos tipo '1'. A confirmação do tipo é feita no segundo 'cout'
  // onde comparamos o tipo recebido com o esperado.Nesse caso quando o retorno é 1 indica True e a 
  // verificação é verdadeira. O mesmo para as últimas três threads, com a alteração da impressão de um "I"
  // indicando IO_BOUND e o tipo esperado é '0'.
  // Esses números (0, 1) vem no ENUM onde definimos os dois tipos (IO_BOUND, CPU_BOUND) em process.h.
  for (int i = 0; i < 6; i++)
  {
    thread[i]->join();

    if (i < 3){
      cout << "C" << thread[i]->type() << endl;
      cout << (thread[i]->type() == 1) << endl; // should return 1
    }
    else {
      cout << "I" << thread[i]->type() << endl;
      cout << (thread[i]->type() == 0) << endl; // should return 0
    }
  }

  for (int i = 0; i < 6; i++)
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

  return 0;
}

int IO_bound()
{
  for (size_t i = 0; i < 4; i++)
  {

    Alarm::delay(10000);

  }

  return 0;
}