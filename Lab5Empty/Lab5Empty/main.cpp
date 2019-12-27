//#include <cstdint>            //for int types such as uint32_t
#include "gpioaregisters.hpp"  //for Gpioa
#include "gpiocregisters.hpp"  //for Gpioc
#include "gpiobregisters.hpp"  //for Gpiob
#include "gpioaregisters.hpp"
#include "rccregisters.hpp"    //for RCC
#include "tim2registers.hpp"   //for SPI2
#include "nvicregisters.hpp"   //for NVIC
#include "usart2registers.hpp" //for USART2

using namespace std ;


extern "C"
{
int __low_level_init(void)
{
  RCC::CR::HSEON::On::Set();
  while(!RCC::CR::HSERDY::Ready::IsSet())
  {
  };
  
  RCC::CFGR::SW::Hse::Set();
  while(!RCC::CFGR::SWS::Hse::IsSet())
  {
  };
  
  RCC::APB1ENR::TIM2EN::Enable::Set();
  
  TIM2::PSC::Write(7999);
  TIM2::ARR::Write(500);
  TIM2::CR1::URS::OverflowEvent::Set(); 
  TIM2::CR1::CEN::Enable::Set();
  
  RCC::AHB1ENR::GPIOAEN::Enable::Set();
  GPIOA::MODER::MODER5::Output::Set();
  
  RCC::APB1ENR::USART2EN::Enable::Set();
  
  GPIOA::MODER::MODER2::Alternate::Set();
  GPIOA::MODER::MODER3::Alternate::Set();
  GPIOA::OTYPER::OT2::OutputPushPull::Set();
  GPIOA::OTYPER::OT3::OutputPushPull::Set();
  GPIOA::PUPDR::PUPDR2::PullUp::Set();
  GPIOA::PUPDR::PUPDR3::PullUp::Set();
  GPIOA::AFRL::AFRL2::Af7::Set();
  GPIOA::AFRL::AFRL3::Af7::Set();
  USART2::BRR::Write(832);
  USART2::CR1::UE::Enable::Set();
  
  USART2::CR1::TE::Enable::Set();
  
  return 1;
}
}


template <typename Port, std::uint32_t pinNum>
struct Led //Realizacia perecluchenia cherez Toggle
{
  static void Toggle()
  {
    Port::ODR::Toggle(1<<pinNum);
  }
};

int main()
{
  const char* pStr = "Hello world" ;
  const char *ptr = pStr ;
  for( ; ;)
  {
    if (TIM2::SR::UIF::UpdatePending::IsSet())
      {
        TIM2::SR::UIF::NoUpdate::Set();
        
        while (*ptr != 0)
        {
          USART2::DR::Write(*ptr++);
          //while (USART2::SR::TXE::DataRegisterNotEmpty::IsSet()) 
         // {
         // }
        }
        ptr = pStr ;        
      }
  }
    
  return 0 ;
}

