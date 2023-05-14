#ifndef SYSTEM_H_3215b7a61b384819b6127a53816114fe
#define SYSTEM_H_3215b7a61b384819b6127a53816114fe

#include <p32xxxx.h>
#include <const.h>  
#include <inttypes.h>

typedef enum SystemClocksErrorEnum {
  Error_None                      = 0,
  Error_RequestedFrequencyTooHigh = 1,
  Error_NoCombinationFound        = 2,
} SystemClocksError;

typedef struct SystemClocksSettingsStruct {
  uint8_t PLLIDIV;
  uint8_t PLLODIV;
  uint8_t PLLMULT;
  uint8_t PBDIV;

  #ifdef EXTENDED_TEST_CASE
    // Error state
    SystemClocksError error;
  #endif
} SystemClocksSettings;

void SystemUnlock();
void SystemLock();
void SystemReset();
void SystemSleep();
void SystemIdle();
void SystemClocksReadSettings(SystemClocksSettings *s);
uint32_t SystemClocksGetCpuFrequency(const SystemClocksSettings *s);
uint32_t SystemClocksGetPeripheralFrequency(const SystemClocksSettings *s);
void SystemClocksCalcCpuClockSettings(SystemClocksSettings *s, 
                      uint32_t cpuFrequency);
void SystemClocksCalcPeripheralClockSettings(SystemClocksSettings *s, 
                         uint32_t peripheralFrequency);
void SystemClocksWriteSettings(const SystemClocksSettings *s);
uint32_t GetSystemClock(void);
uint32_t GetPeripheralClock();
void SystemConfig(uint32_t cpuCoreFrequency, uint32_t peripheralFreqDiv);
void SystemSetPeripheralClock(uint32_t peripheralFrequency);
uint32_t MIPS32 GetCP0Count();
void MIPS32 SetCP0Count(uint32_t count);
void MIPS32 INTEnableSystemMultiVectoredInt(void);

void System_DelayUs(uint32_t us);		// Delay for some us. BLOCKING.
void System_Delay100ns(uint32_t nss);	// Delay for some * 100ns. BLOCKING.


#endif	/* __SYSTEM_H_3215b7a6-1b38-4819-b612-7a53816114fe */
