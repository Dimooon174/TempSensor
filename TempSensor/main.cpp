#include <cstdint>            //for int types such as uint32_t
#include "gpioaregisters.hpp" //for Gpioa
#include "gpiocregisters.hpp" //for Gpioc
#include "gpiobregisters.hpp" //for Gpiob
#include "rccregisters.hpp"   //for RCC
#include "tim2registers.hpp"   //for SPI2
#include "nvicregisters.hpp"  //for NVIC
#include "adc1registers.hpp"
#include "adccommonregisters.hpp"
#include <iostream>

extern "C"
{
  int __low_level_init(void)
  {
    RCC::CR::HSEON::On::Set();
    while (!RCC::CR::HSERDY::Ready::IsSet())
    { }
    RCC::CFGR::SW::Hse::Set();
    while (!RCC::CFGR::SWS::Hse::IsSet())
    { }
    RCC::CR::HSION::Off::Set();   
    RCC::AHB1ENR::GPIOAEN::Enable::Set();
    RCC::AHB1ENR::GPIOCEN::Enable::Set();
    GPIOC::MODER::MODER5::Output::Set();
    GPIOA::MODER::MODER5::Output::Set();
    GPIOC::MODER::MODER9::Output::Set();
    GPIOC::MODER::MODER8::Output::Set();
    RCC::APB2ENR::ADC1EN::Enable::Set();
    ADC1::SQR3::SQ1::Channel18::Set();                                  
    ADC_Common::CCR::TSVREFE::Enable::Set();                            
    ADC1::CR1::RES::Bits12::Set();
    ADC1::CR2::CONT::SingleConversion::Set();
    ADC1::CR2::EOCS::SingleConversion::Set();
    ADC1::SQR1::L::Conversions1::Set();
    ADC1::SMPR1::SMP18::Cycles84::Set();
    ADC1::CR2::ADON::Enable::Set(); 
  }
}
   float V25 = 0.76F;
   float AVG_Slope = 0.0025F;
   float Temp = 0.0F;
int main()
{
  
  for(;;)
  {
    ADC1::CR2::SWSTART::On::Set();
     while(!ADC1::SR::EOC::ConversionComplete::IsSet())
    {};
  uint32_t data =   ADC1::DR::Get(); //Get data from ADC;
  Temp = ((((data*3.3)/4096 - V25)/AVG_Slope)+25.0);
  std::cout << " Темература °С : " <<  Temp << std::endl;  
  if(Temp>26)
  {
    GPIOC::BSRR::BS5::High::Write();
  }
  else
  {
    GPIOC::BSRR::BR5::Low::Write();
  }
   if(Temp>27)
  {
    GPIOC::BSRR::BS8::High::Write();
  }
  else
  {
    GPIOC::BSRR::BR8::Low::Write();
  }
   if(Temp>28)
  {
    GPIOC::BSRR::BS9::High::Write();
  }
  else
  {
    GPIOC::BSRR::BR9::Low::Write();
  }
   if(Temp>29)
  {
    GPIOA::BSRR::BS5::High::Write();
  }
  else
  {
    GPIOA::BSRR::BR5::Low::Write();
  }
}
}
