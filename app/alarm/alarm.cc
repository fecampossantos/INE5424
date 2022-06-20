// EPOS Alarm Component Test Program

#include <time.h>

using namespace EPOS;

const int iterations = 10;

void scream_hello(void);
void whisper_bye(void);

OStream cout;

int main()
{
    cout << "Alarm test" << endl;

    cout << "Creating two alarms, then going to sleep" << endl;

    Function_Handler handler_a(&scream_hello);
    Alarm alarm_a(3000000, &handler_a, iterations);

    Function_Handler handler_b(&whisper_bye);
    Alarm alarm_b(2000000, &handler_b, iterations);

    // Note that in case of idle-waiting, this thread will go into suspend
    // and the alarm handlers above will trigger the functions in the context
    // of the idle thread!
    Alarm::delay(3000000 * (iterations + 2));

    cout << "the end" << endl;

    return 0;
}

void scream_hello()
{
    for(int i = 0; i < 10; i++)
        cout << "HELLO!";
    cout << endl;
}

void whisper_bye(void)
{
    for(int i = 0; i < 10; i++)
        cout << "goodbye";
    cout << endl;
}