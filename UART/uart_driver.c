#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include "uart_driver.h"

void UARTWriteByte(UART0_Type *UARTx, uint8_t data){
  UARTx->DR = data;
}


uint8_t UARTReadByte(UART0_Type *UARTx){
  uint8_t data = UARTx->DR;
  return data;
}


bool UARTReceiveErrorCheck(UART0_Type *UARTx, uint32_t error){
  bool orrur = (UARTx->RSR & error);
  return occur;
}


void UARTClearReceiveError(UART0_Type *UARTx, uint32_t error){
  UARTx->ECR = UART_ERR_CLEAR;
}


bool UARTFlagCheck(UART0_Type *UARTx, uint32_t flag){
  bool flag_set = (UARTx->FR & flag);
  return flag_set;
}


void UARTIrDALPDivSet(UART0_Type *UARTx, uint8_t divisor){
  UARTx->ILPR = divisor;
}

uint8_t UARTIrDALPDivGet(UART0_Type *UARTx){
  uint8_t divisor = UARTx->ILPR;
  return divisor;
}


void UARTBaudIntDivSet(UART0_Type *UARTx, uint16_t divisor){
  UARTx->IBRD = divisor;
}


uint16_t UARTBaudIntDivGet(UART0_Type *UARTx){
  uint8_t divisor = UARTx->IBRD;
  return divisor;
}


void UARTBaudFracDivSet(UART0_Type *UARTx, uint8_t divisor){
  UARTx->FBRD = divisor;
}


uint8_t UARTBaudFracDivGet(UART0_Type *UARTx){
  uint8_t divisor = UARTx->FBRD;
  return divisor;
}


void UARTLineControlSet(UART0_Type *UARTx, uint32_t lineControl){
  UARTx->LCRH |= lineControl;
}


void UARTWordWidthSet(UART0_Type *UARTx, uint32_t width){
  UARTx->LCRH |= (width << 5);
}


uint32_t UARTWordWidthGet(UART0_Type *UARTx){
  uint8_t width = (UARTx->LCRH & (0x3 << 5) >> 5);
  return width;
}


void UARTClearToSendEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_CTSEN;
}


void UARTClearToSendDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_CTSEN;
}


void UARTReqToSendEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_RTSEN;
}


void UARTReqToSendDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_RTSEN;
}


void UARTReqToSendSet(UART0_Type *UARTx){
  UARTx->CTL |= UART_RTS;
}


void UARTReqToSendClear(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_RTS;
}


void UARTReceiveEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_RXE;
}


void UARTReceiveDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_RXE;
}


void UARTTransmitEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_TXE;
}


void UARTTransmitDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_TXE;
}


void UARTLoopBackEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_LBE;
}


void UARTLoopBackDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_LBE;
}


void UARTHighSpeedEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_HSE;
}


void UARTHighSpeedDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_HSE;
}


void UARTEndOfTxEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_EOT;
}


void UARTEndOfTxDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_EOT;
}


void UARTSmartEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_SMART;
}


void UARTSmartDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_SMART;
}


void UARTSIRLowPwrEnable(UART0_Type *UARTx);{
  UARTx->CTL |= UART_SIRLP;
}


void UARTSIRLowPwrEDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_SIRLP;
}


void UARTSIREnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_SIREN;
}


void UARTSIRDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_SIREN;
}


void UARTEnable(UART0_Type *UARTx){
  UARTx->CTL |= UART_ENABLE;
}


void UARTDisable(UART0_Type *UARTx){
  UARTx->CTL &= ~UART_ENABLE;
}


void UARTTxIntFIFOLvlSet(UART0_Type *UARTx, uint32_t IFL){
  UARTx->IFLS |= (IFL << 0);
}


uint32_t UARTTxIntFIFOLvlGet(UART0_Type *UARTx){
  uint32_t lvl = ((UARTx->IFLS & 0x7) >> 0);
  return lvl;
}


void UARTRxIntFIFOLvlSet(UART0_Type *UARTx, uint32_t IFL){
  UARTx->IFLS |= (IFL << 3);
}


uint32_t UARTRxIntFIFOLvlGet(UART0_Type *UARTx){
  uint32_t lvl = ((UARTx->IFLS & (0x7 << 3)) >> 3);
  return lvl;
}


void UARTInterruptEnable(UART0_Type *UARTx, uint32_t interrupt){
  UARTx->IM |= interrupt;
}


void UARTInterruptDisable(UART0_Type *UARTx, uint32_t interrupt){
  UARTx->IM &= ~interrupt;
}


bool UARTRawInterruptGet(UART0_Type *UARTx, uint32_t interrupt){
  bool occur = (UARTx->RIS & interrupt);
  return occur;
}


bool UARTMaskedInterruptGet(UART0_Type *UARTx, uint32_t interrupt){
  bool occur = (UARTx->MIS & interrupt);
  return occur;
}


void UARTInterruptClear(UART0_Type *UARTx, uint32_t interrupt){
  UARTx->ICR |= interrupt;
}


void UARTDMASet(UART0_Type *UARTx, uint32_t param){
  UARTx->DMACTL |= param;
}


void UART9BitAdrSet(UART0_Type *UARTx, uint8_t addr, bool enable){
  UARTx->9BITADDR = addr;
  if(enable){
    UARTx->9BITADDR |= UART_9BITEN;
  } else {
    UARTx->9BITADDR &= ~UART_9BITEN;
  }
}


uint8_t UART9BitAdrGet(UART0_Type *UARTx){
  uint8_t addr = (UARTx->9BITADDR & 0xFF);
}


void UART9BitAdrMask(UART0_Type *UARTx, uint8_t mask){
  UARTx->9BITAMASK = mask;
}


void UARTPeriPropSet(UART0_Type *UARTx, uint32_t prop){
  UARTx->PP |= prop;
}


void UARTClkConfig(UART0_Type *UARTx, uint8_t clock){
  UARTx->CC |= clock;
}


void UARTInit(UART0_Type *UARTx){
  
}


void UARTTransmit(UART0_Type *UARTx, uint8_t *tsm_buffer);


void UARTReceive(UART0_Type *UARTx, uint8_t *rcv_buffer);
