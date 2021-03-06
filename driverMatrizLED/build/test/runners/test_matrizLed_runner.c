/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Automagically Detected Files To Include=====*/
#include "unity.h"

int GlobalExpectCount;
int GlobalVerifyOrder;
char* GlobalOrderError;

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_todos_los_leds_inician_apagados(void);
extern void test_encender_un_led(void);
extern void test_encender_y_apagar_un_led(void);
extern void test_encender_todos_los_leds(void);
extern void test_encender_y_apagar_los_leds(void);
extern void test_encender_y_apagar_todos_los_leds(void);
extern void test_consultar_estado_de_led(void);


/*=======Mock Management=====*/
static void CMock_Init(void)
{
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
  GlobalOrderError = NULL;
}
static void CMock_Verify(void)
{
}
static void CMock_Destroy(void)
{
}

/*=======Teardown (stub)=====*/
void tearDown(void) {}

/*=======Test Reset Options=====*/
void resetTest(void);
void resetTest(void)
{
  tearDown();
  CMock_Verify();
  CMock_Destroy();
  CMock_Init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  CMock_Verify();
}

/*=======Test Runner Used To Run Each Test=====*/
static void run_test(UnityTestFunction func, const char* name, int line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    CMock_Init();
    if (TEST_PROTECT())
    {
        setUp();
        func();
    }
    if (TEST_PROTECT())
    {
        tearDown();
        CMock_Verify();
    }
    CMock_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

/*=======MAIN=====*/
int main(void)
{
  UnityBegin("test_matrizLed.c");
  run_test(test_todos_los_leds_inician_apagados, "test_todos_los_leds_inician_apagados", 16);
  run_test(test_encender_un_led, "test_encender_un_led", 25);
  run_test(test_encender_y_apagar_un_led, "test_encender_y_apagar_un_led", 33);
  run_test(test_encender_todos_los_leds, "test_encender_todos_los_leds", 41);
  run_test(test_encender_y_apagar_los_leds, "test_encender_y_apagar_los_leds", 48);
  run_test(test_encender_y_apagar_todos_los_leds, "test_encender_y_apagar_todos_los_leds", 64);
  run_test(test_consultar_estado_de_led, "test_consultar_estado_de_led", 72);

  return UnityEnd();
}
