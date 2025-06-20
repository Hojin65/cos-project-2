#include "process_manager.h"
#include "opcode.h"
#include "byte_op.h"
#include "setting.h"
#include <cstring>
#include <iostream>
#include <ctime>
using namespace std;

ProcessManager::ProcessManager()
{
  this->num = 0;
}

void ProcessManager::init()
{
}

// 3차원 벡터: (최저 기온, 최고 기온, 평균 전력)
uint8_t *ProcessManager::processData(DataSet *ds, int *dlen)
{
  uint8_t *ret, *p;
  int num;
  HouseData *house;
  TemperatureData *tdata;
  PowerData *pdata;
  char buf[BUFLEN];
  ret = (uint8_t *)malloc(BUFLEN);
  int min_temp, max_temp, avg_power, sum_power;

  tdata = ds->getTemperatureData();
  num = ds->getNumHouseData();

  min_temp = tdata->getMin();
  max_temp = tdata->getMax();

  sum_power = 0;
  for (int i=0; i<num; i++)
  {
    house = ds->getHouseData(i);
    pdata = house->getPowerData();
    sum_power += pdata->getValue();
  }
  avg_power = sum_power / num;

  memset(ret, 0, BUFLEN);
  *dlen = 0;
  p = ret;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(min_temp, p);
  *dlen += 1;
  VAR_TO_MEM_1BYTE_BIG_ENDIAN(max_temp, p);
  *dlen += 1;
  VAR_TO_MEM_2BYTES_BIG_ENDIAN(avg_power, p);
  *dlen += 2;

  return ret;
}
