/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Digital_Clock
Version : 1.0.0
Date    : 5/14/2016
Author  : sigma
Company :
Comments:


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <glcd.h>
#include <font5x7.h>
////////////////////////////////////
#include <stdio.h>
#include "includes/clock_0.c"
#include "includes/eagle.c"
#include "includes/taleb.c"
#include <math.h>

//#include "includes/graphics/glcd.h"
/*#include "includes/font/courier7x14.c"
#include "includes/font/courier_bold14.c"
#include "includes/font/arial14.c"
#include "includes/font/arial_bold14.c"
#include "includes/font/script19.c"  */
/////////////////////////////////////
// Declare your global variables here
int second,hour,minute,H;
int counter,but0_counter,but2_counter,pointer,day_pointer,Rs,Rm,Rh;
int sh_day,sh_month,sh_year;
int ch_day,ch_month,ch_year;
bit clock_mode = 0; //0 : 12 hour ----- 1: 24 hour
char am_pm = 'a'; //a:AM ---- p:PM
char temp_s [15];
bit show_cl=1;
bit mode=0; //0: normal   1: setting
bit mode_about=0;
bit on_while=0;

float xs,ys,xs0,ys0;
float Xm,Ym,Xh,Yh;
float ym0,xm0,yh0,xh0;
int x,y;

////////////////////////////
int MiladiIsLeap(int miladiYear)
{
 if(((miladiYear % 100)!= 0 && (miladiYear % 4) == 0) || ((miladiYear % 100)== 0 && (miladiYear % 400) == 0))
  return 1;
 else
  return 0;
}

typedef struct _SHAMSIDATE
{
    int iYear;
    int iMonth;
    int iDay;
}SHAMSIDATE;

typedef struct _MILADIDATE
{
 int         iYear;
 int         iMonth;
 int         iDay;
}MILADIDATE;

SHAMSIDATE sh;
MILADIDATE m;

SHAMSIDATE MiladiToShamsi(int iMiladiMonth,int iMiladiDay,int iMiladiYear)
{

    int  shamsiDay, shamsiMonth, shamsiYear;
    int  dayCount,farvardinDayDiff,deyDayDiff ;
    int  sumDayMiladiMonth[] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int  sumDayMiladiMonthLeap[]= {0,31,60,91,121,152,182,213,244,274,305,335};
    SHAMSIDATE  shamsidate;

 farvardinDayDiff=79;

if (MiladiIsLeap(iMiladiYear))
 {
    dayCount = sumDayMiladiMonthLeap[iMiladiMonth-1] + iMiladiDay;
 }
else
 {
    dayCount = sumDayMiladiMonth[iMiladiMonth-1] + iMiladiDay;
 }
if((MiladiIsLeap(iMiladiYear - 1)))
 {
    deyDayDiff = 11;
 }
else
 {
    deyDayDiff = 10;
 }
if (dayCount > farvardinDayDiff)
{
    dayCount = dayCount - farvardinDayDiff;
    if (dayCount <= 186)
     {
      switch (dayCount%31)
       {
        case 0:
         shamsiMonth = dayCount / 31;
         shamsiDay = 31;
        break;
        default:
         shamsiMonth = (dayCount / 31) + 1;
         shamsiDay = (dayCount%31);
        break;
      }
      shamsiYear = iMiladiYear - 621;
     }
   else
     {
    dayCount = dayCount - 186;
    switch (dayCount%30)
      {
       case 0:
        shamsiMonth = (dayCount / 30) + 6;
        shamsiDay = 30;
       break;
     default:
       shamsiMonth = (dayCount / 30) + 7;
       shamsiDay = (dayCount%30);
       break;
     }
      shamsiYear = iMiladiYear - 621;
    }
  }
else
  {
    dayCount = dayCount + deyDayDiff;

    switch (dayCount%30)
    {
    case 0 :
      shamsiMonth = (dayCount / 30) + 9;
      shamsiDay = 30;
     break;
    default:
      shamsiMonth = (dayCount / 30) + 10;
      shamsiDay = (dayCount%30);
     break;
    }
    shamsiYear = iMiladiYear - 622;

  }
    shamsidate.iYear = shamsiYear;
    shamsidate.iMonth = shamsiMonth;
    shamsidate.iDay = shamsiDay;

  return shamsidate ;
}

MILADIDATE ShamsiToMiladi(int ShamsiMonth,int ShamsiDay,int ShamsiYear)
{

MILADIDATE    miladiDate;
int           marchDayDiff, remainDay;
int           dayCount, miladiYear, i;


// this buffer has day count of Miladi month from April to January for a none year.

int miladiMonth[12]  =  {30,31,30,31,31,30,31,30,31,31,28,31};
miladiYear = ShamsiYear + 621;

//Detemining the Farvardin the First

if(MiladiIsLeap(miladiYear)==1)
{
//this is a Miladi leap year so Shamsi is leap too so the 1st of Farvardin is March 20 (3/20)
  marchDayDiff = 12;
}
else
{
//this is not a Miladi leap year so Shamsi is not leap too so the 1st of Farvardin is March 21 (3/21)
  marchDayDiff = 11;
}

// If next year is leap we will add one day to Feb.
if(MiladiIsLeap(miladiYear+1)==1)
{
miladiMonth[10] = miladiMonth[10] + 1; //Adding one day to Feb
}

//Calculate the day count for input shamsi date from 1st Farvadin

if((ShamsiMonth>=1)&&( ShamsiMonth<=6))
 dayCount = ((ShamsiMonth-1) * 31) + ShamsiDay;
else
 dayCount =(6 * 31) + ((ShamsiMonth - 7) * 30) + ShamsiDay;

//Finding the correspond miladi month and day

if (dayCount <= marchDayDiff) //So we are in 20(for leap year) or 21for none leap year) to 31 march
{
 miladiDate.iDay = dayCount + (31 - marchDayDiff);
 miladiDate.iMonth = 3;
 miladiDate.iYear=miladiYear;
}
else

{
 remainDay = dayCount - marchDayDiff;


  i = 0; //starting from April

while ((remainDay > miladiMonth[i]))
{
 remainDay = remainDay - miladiMonth[i];
 i++;
}
 miladiDate.iDay = remainDay;

if (i > 8) // We are in the next Miladi Year
{
 miladiDate.iMonth = i - 8;
 miladiDate.iYear =  miladiYear + 1;
}
else
{
 miladiDate.iMonth = i + 4;
 miladiDate.iYear =  miladiYear;
 }

}

 return miladiDate;
}


char* miladi_day [7]={"Sunday   ","Monday   ","Tuesday  ","Wedensday","Thursday ","Friday   ","Saturday "};
char* miladi_month [12]={"January  ","February ","March    ","April    ","May      ","June     ","July     ","August   ","September","October  ","November ","December "};

/////////////////////////////
void inc_selection(int s)
{
 if(clock_mode==0)
 {
  if(s>3)
  {
   s--;
  }
  if(s==3)
  {
   if(am_pm=='a')
   {
    am_pm='p';
   }
   else if(am_pm=='p')
   {
    am_pm='a';
   }
   return;
  }
 }
 switch(s)
 {
  case 0:
   if(second>30)
   {
    second=0;
    minute++;
    if(minute>=60)
   {
    hour++;
    minute=0;
    if(clock_mode==0)
    {
     if(am_pm=='a' && hour>=12)
     {
      am_pm='p';
     }
     else if(am_pm=='p' && hour>=13)
     {
      hour=1;
     }
     else if(am_pm=='p' && hour>=12)
     {
      am_pm='a';
      hour=0;
      sh_day++;
      day_pointer++;
      if(day_pointer==7)
        day_pointer=0;
      //sh_day
       switch(sh_month)
       {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        if(sh_day==32)
        {
        sh_day=1;
        sh_month++;
        if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
        }
        break;

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
         if(sh_day==31)
         {
         sh_day=1;
         sh_month++;
         if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
         }
        break;

        case 12:
         if(sh_day==30)
         {
         sh_day=1;
         sh_month=1;
         sh_year++;
         }
       }
     }
    }
    else
    {
     if(hour>=24)
     {
      hour=0;
      sh_day++;
      day_pointer++;
      if(day_pointer==7)
        day_pointer=0;
      switch(sh_month)
   {
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   if(sh_day==31)
   {
    sh_day=1;
    sh_month++;
    if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
   }
   break;

   case 7:
   case 8:
   case 9:
   case 10:
   case 11:
    if(sh_day==30)
    {
    sh_day=1;
    sh_month++;
    if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
    }
   break;

   case 12:
    if(sh_day==29)
    {
     sh_day=1;
     sh_month=1;
     sh_year++;
    }
  }
     }
    }
   }
   }
   else
    second=0;

   sh.iDay=sh_day;
   sh.iMonth=sh_month;
   sh.iYear=sh_year;
   m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
   ch_day=m.iDay;
   ch_month=m.iMonth;
   ch_year=m.iYear;
  break;

  case 1:
   minute++;
   if(minute>=60)
   {
    hour++;
    minute=0;
    if(clock_mode==0)
    {
     if(am_pm=='a' && hour>=12)
     {
      am_pm='p';
     }
     else if(am_pm=='p' && hour>=13)
     {
      hour=1;
     }
     else if(am_pm=='p' && hour>=12)
     {
      am_pm='a';
      hour=0;
      sh_day++;
      day_pointer++;
      if(day_pointer==7)
        day_pointer=0;
      //sh_day
       switch(sh_month)
       {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        if(sh_day==32)
        {
        sh_day=1;
        sh_month++;
        if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
        }
        break;

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
         if(sh_day==31)
         {
         sh_day=1;
         sh_month++;
         if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
         }
        break;

        case 12:
         if(sh_day==30)
         {
         sh_day=1;
         sh_month=1;
         sh_year++;
         }
       }
     }
    }
    else
    {
     if(hour>=24)
     {
      hour=0;
      sh_day++;
      day_pointer++;
      if(day_pointer==7)
        day_pointer=0;
      switch(sh_month)
   {
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   if(sh_day==31)
   {
    sh_day=1;
    sh_month++;
    if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
   }
   break;

   case 7:
   case 8:
   case 9:
   case 10:
   case 11:
    if(sh_day==30)
    {
    sh_day=1;
    sh_month++;
    if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
    }
   break;

   case 12:
    if(sh_day==29)
    {
     sh_day=1;
     sh_month=1;
     sh_year++;
    }
  }
     }
    }
   }

   sh.iDay=sh_day;
   sh.iMonth=sh_month;
   sh.iYear=sh_year;
   m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
   ch_day=m.iDay;
   ch_month=m.iMonth;
   ch_year=m.iYear;
  break;

  case 2:
   hour++;
   if(clock_mode==0)
   {
    if(am_pm=='a' && hour>=12)
    {
    am_pm='p';
    }
    else if(am_pm=='p' && hour>=13)
    {
     hour=1;
    }
    else if(am_pm=='p' && hour>=12)
    {
    sh_day++;
    day_pointer++;
    if(day_pointer==7)
     day_pointer=0;
    am_pm='a';
    hour=0;

    //sh_day
    switch(sh_month)
    {
     case 1:
     case 2:
     case 3:
     case 4:
     case 5:
     case 6:
      if(sh_day==32)
      {
       sh_day=1;
       sh_month++;

      }
      break;
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
      if(sh_day==31)
      {
       sh_day=1;
       sh_month++;
      }
     break;

     case 12:
      if(sh_day==30)
      {
       sh_day=1;
       sh_month=1;
       sh_year++;
      }
    }
    }
    }
    else
    {
     if(hour>=24)
     {
      hour=0;
      sh_day++;
      day_pointer++;
      if(day_pointer==7)
       day_pointer=0;
      switch(sh_month)
    {
     case 1:
     case 2:
     case 3:
     case 4:
     case 5:
     case 6:
      if(sh_day==32)
      {
       sh_day=1;
       sh_month++;
       if(sh_month==13)
        {
         sh_month=1;
         sh_year++;
        }
      }
      break;
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
      if(sh_day==31)
      {
       sh_day=1;
       sh_month++;
      }
     break;

     case 12:
      if(sh_day==30)
      {
       sh_day=1;
       sh_month=1;
       sh_year++;
      }
    }
     }
    }

    sh.iDay=sh_day;
    sh.iMonth=sh_month;
    sh.iYear=sh_year;
    m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
    ch_day=m.iDay;
    ch_month=m.iMonth;
    ch_year=m.iYear;
  break;

  case 3:
   sh_day++;
   day_pointer++;
   if(day_pointer==7)
    day_pointer=0;
   switch(sh_month)
   {
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   if(sh_day==32)
   {
    sh_day=1;
    sh_month++;

   }
   break;

   case 7:
   case 8:
   case 9:
   case 10:
   case 11:
    if(sh_day==31)
    {
    sh_day=1;
    sh_month++;
    }
   break;

   case 12:
    if(sh_day==30)
    {
     sh_day=1;
     sh_month=1;
     sh_year++;
    }
  }

   sh.iDay=sh_day;
   sh.iMonth=sh_month;
   sh.iYear=sh_year;
   m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
   ch_day=m.iDay;
   ch_month=m.iMonth;
   ch_year=m.iYear;
  break;

  case 4:
   sh_month++;
   if(sh_month==13)
   {
    sh_month=1;
    sh_year++;
   }

   sh.iDay=sh_day;
   sh.iMonth=sh_month;
   sh.iYear=sh_year;
   m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
   ch_day=m.iDay;
   ch_month=m.iMonth;
   ch_year=m.iYear;
  break;

  case 5:
   sh_year++;
   sh.iDay=sh_day;
   sh.iMonth=sh_month;
   sh.iYear=sh_year;
   m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
   ch_day=m.iDay;
   ch_month=m.iMonth;
   ch_year=m.iYear;
  break;

  case 6:
   ch_day++;
   day_pointer++;
   if(day_pointer==7)
    day_pointer=0;
   switch(ch_month)
   {
   case 1:
   case 3:
   case 5:
   case 7:
   case 8:
   case 10:
   case 12:
   if(ch_day==32)
   {
    ch_day=1;
    ch_month++;
    if(ch_month==13)
        {
         ch_month=1;
         ch_year++;
        }
   }
   break;

   case 6:
   case 4:
   case 9:
   case 11:
    if(ch_day==31)
    {
    ch_day=1;
    ch_month++;
    }
   break;
   case 2:
    if(ch_day==30)
    {
     ch_day=1;
     ch_month++;
    }
   break;
  }

   m.iDay=ch_day;
   m.iMonth=ch_month;
   m.iYear=ch_year;
   sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);

   sh_day=sh.iDay;
   sh_month=sh.iMonth;
   sh_year=sh.iYear;
  break;

  case 7:
   ch_month++;
   if(ch_month==13)
   {
    ch_month=1;
    ch_year++;
   }

   m.iDay=ch_day;
   m.iMonth=ch_month;
   m.iYear=ch_year;
   sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);

   sh_day=sh.iDay;
   sh_month=sh.iMonth;
   sh_year=sh.iYear;
  break;

  case 8:
   ch_year++;
   m.iDay=ch_day;
   m.iMonth=ch_month;
   m.iYear=ch_year;
   sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);

   sh_day=sh.iDay;
   sh_month=sh.iMonth;
   sh_year=sh.iYear;
  break;

  case 9:
   ch_month++;
   if(ch_month==13)
   {
    ch_month=1;
    ch_year++;
   }

   m.iDay=ch_day;
   m.iMonth=ch_month;
   m.iYear=ch_year;
   sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);

   sh_day=sh.iDay;
   sh_month=sh.iMonth;
   sh_year=sh.iYear;
  break;

  case 10:
   day_pointer++;
   if(day_pointer==7)
    day_pointer=0;
   glcd_outtextxy(10,30,miladi_day[day_pointer]);
  break;
 }

 glcd_outtextxy(10,20,miladi_month[ch_month-1]);
}

void selection_s(int s)
{
if(clock_mode==1)
{
switch(s)
 {
  case 0: glcd_outtextxy(0,111,"---------------------++-----------------");
          glcd_outtextxy(0,30," ");
          break;
  case 1: glcd_outtextxy(0,111,"------------------++--------------------"); break;
  case 2: glcd_outtextxy(0,111,"---------------++-----------------------"); break;
  case 3: glcd_outtextxy(0,111,"--------------------------------------++"); break;
  case 4: glcd_outtextxy(0,111,"-----------------------------------++---"); break;
  case 5: glcd_outtextxy(0,111,"------------------------------++++------"); break;
  case 6: glcd_outtextxy(0,111,"--------++------------------------------"); break;
  case 7: glcd_outtextxy(0,111,"-----++---------------------------------"); break;
  case 8: glcd_outtextxy(0,111,"++++------------------------------------"); break;
  case 9: glcd_outtextxy(0,111,"----------------------------------------");
          glcd_outtextxy(0,20,"+");
          break;
  case 10: glcd_outtextxy(0,30,"+");
           glcd_outtextxy(0,20," ");
           break;
 }
}
else
{
 switch(s)
 {
 case 0: glcd_outtextxy(0,111,"---------------------++-----------------");
          glcd_outtextxy(0,30," ");
          break;
  case 1: glcd_outtextxy(0,111,"------------------++--------------------"); break;
  case 2: glcd_outtextxy(0,111,"---------------++-----------------------"); break;
  case 3: glcd_outtextxy(0,111,"------------------------++--------------"); break;
  case 4: glcd_outtextxy(0,111,"--------------------------------------++"); break;
  case 5: glcd_outtextxy(0,111,"-----------------------------------++---"); break;
  case 6: glcd_outtextxy(0,111,"------------------------------++++------"); break;
  case 7: glcd_outtextxy(0,111,"--------++------------------------------"); break;
  case 8: glcd_outtextxy(0,111,"-----++---------------------------------"); break;
  case 9: glcd_outtextxy(0,111,"++++------------------------------------"); break;
  case 10: glcd_outtextxy(0,111,"----------------------------------------");
          glcd_outtextxy(0,20,"+");
          break;
  case 11: glcd_outtextxy(0,30,"+");
           glcd_outtextxy(0,20," ");
           break;
 }
}

}


////////////////////


/////////////////////

void init_clock()
{
 counter=0;
 hour=10;
 minute=59;
 second=50;
 am_pm='p';
 clock_mode=0;

 ch_day=15;
 ch_month=5;
 ch_year=2016;

 sh_day=26;
 sh_month=2;
 sh_year=1395;

 m=ShamsiToMiladi(sh_month,sh_day,sh_year);
 sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);
}

void show_clock()
{
 if(show_cl==0)
   return;
 if(clock_mode==0)
 {
  H=hour;
  if(am_pm=='a')
  {
   sprintf(temp_s,"* %02i:%02i:%02i AM *",hour,minute,second);
  }
  else
  {
   sprintf(temp_s,"* %02i:%02i:%02i PM *",hour,minute,second);
  }
 }
 else
 {
  if(hour>12)
   H=hour-12;
  else
   H=hour;
  sprintf(temp_s,"* %02i:%02i:%02i    *",hour,minute,second);
 }


 glcd_outtextxy(80,120,temp_s);
 //sprintf(temp_s,"* %02i : %02i : %02i*",but0_counter,but2_counter,pointer);
 //glcd_outtextxy(40,110,temp_s);



 m.iDay=ch_day;
 m.iMonth=ch_month;
 m.iYear=ch_year;

 /*sh.iDay=sh_day;
 sh.iMonth=sh_month;
 sh.iYear=sh_year; */
 //m=ShamsiToMiladi(sh.iMonth,sh.iDay,sh.iYear);
 //sh=MiladiToShamsi(m.iMonth,m.iDay,m.iYear);
 ch_day=m.iDay;
 ch_month=m.iMonth;
 ch_year=m.iYear;

 /*sh_day=sh.iDay;
 sh_month=sh.iMonth;
 sh_year=sh.iYear; */
 sprintf(temp_s,"%04i/%02i/%02i",ch_year,ch_month,ch_day);
 glcd_outtextxy(0,120,temp_s);
 sprintf(temp_s,"%04i/%02i/%02i",sh_year,sh_month,sh_day);
 glcd_outtextxy(180,120,temp_s);

 glcd_outtextxy(10,20,miladi_month[ch_month-1]);
 glcd_outtextxy(10,30,miladi_day[day_pointer]);
 if(mode==1)
  selection_s(pointer);
 else
  glcd_outtextxy(0,111,"----------------------------------------");
  ////////

  xs=Rs*cos((75-second)*2*3.1415/60.0);
  ys=-Rs*sin((75-second)*2*3.1415/60.0);
  glcd_setcolor(0);
  if(xs0!=xs || ys0!=ys)
  {
   glcd_setlinestyle(1,GLCD_LINE_SOLID);
   glcd_line(x,y,xs0+x,ys0+y);
  }
  glcd_setcolor(1);
  glcd_setlinestyle(1,GLCD_LINE_SOLID);
  glcd_line(x,y,xs+x,ys+y);
  ys0=ys;
  xs0=xs;


  Xm=Rm*cos((75-(minute+(second/60.0)))*2*3.1415/60.0);
  Ym=-Rm*sin((75-(minute+(second/60.0)))*2*3.1415/60.0);
  glcd_setcolor(0);
  if(xm0!=Xm || ym0!=Ym)
  {
   glcd_setlinestyle(2,GLCD_LINE_SOLID);
   glcd_line(x,y,xm0+x,ym0+y);
  }
  glcd_setcolor(1);
  glcd_setlinestyle(2,GLCD_LINE_SOLID);
  glcd_line(x,y,Xm+x,Ym+y);
  ym0=Ym;
  xm0=Xm;

  Xh=(Rh*cos((3-(H+(minute/60.0)))*2*3.1415/12));
  Yh=-Rh*sin((3-(H+(minute/60.0)))*2*3.1415/12);
  glcd_setcolor(0);
  if(xh0!=Xh || yh0!=Yh)
  {
   glcd_setlinestyle(3,GLCD_LINE_SOLID);
   glcd_line(x,y,xh0+x,yh0+y);
  }
  glcd_setcolor(1);
  glcd_setlinestyle(3,GLCD_LINE_SOLID);
  glcd_line(x,y,Xh+x,Yh+y);
  xh0=Xh;
  yh0=Yh;
 }


void about()
{
 mode_about=1;
 glcd_clear();
 show_cl=0;
 glcd_putimagef(0,0,eagle,GLCD_PUTCOPY);
 glcd_putimagef(120,0,taleb,GLCD_PUTCOPY);
}


// Timer1 output compare A interrupt service routine
interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{

 second++;
 if(second>=60)
 {
  second=0;
  minute++;
  if(minute>=60)
  {
   minute=0;
   hour++;
   if(clock_mode==0)
   {
    if(am_pm=='a' && hour==12)
    {
     am_pm='p';
    }
    else if(am_pm=='p' && hour==13)
    {
     hour=1;
    }
    else if(am_pm=='p' && hour==12)
    {
     //hour=0;
     am_pm='a';
     ch_day++;
     sh_day++;
     day_pointer++;
     if(day_pointer==7)
      day_pointer=0;
     switch(sh_month)
     {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
       if(sh_day==32)
        {
         sh_day=1;
         sh_month++;
        }
       break;
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
      if(sh_day==31)
      {
       sh_day=1;
       sh_month++;
      }
      break;
     case 12:
      if(sh_day==30)
      {
       sh_day=1;
       sh_month=1;
       sh_year++;
      }
     }


     //
     m=ShamsiToMiladi(sh_month,sh_day,sh_year);
     ch_day=m.iDay;
     ch_month=m.iMonth;
     ch_year=m.iYear;
     //
    }
    else if(am_pm=='a' && hour==13)
    {
     hour=1;
    }
   }
   else
   {
    if(hour==24)
    {
     hour=0;
     ch_day++;
     sh_day++;
     day_pointer++;
     if(day_pointer==7)
      day_pointer=0;
     switch(sh_month)
     {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
       if(sh_day==32)
        {
         sh_day=1;
         sh_month++;
        }
       break;
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
      if(sh_day==31)
      {
       sh_day=1;
       sh_month++;
      }
      break;
     case 12:
      if(sh_day==30)
      {
       sh_day=1;
       sh_month=1;
       sh_year++;
      }
     }


     //
     m=ShamsiToMiladi(sh_month,sh_day,sh_year);
     ch_day=m.iDay;
     ch_month=m.iMonth;
     ch_year=m.iYear;
     //
    }
   }
  }
 }
  if(on_while==1)
   show_clock();



}


interrupt [TIM2_OVF] void timer2(void)
{
 but0_counter++;
 but2_counter++;
}


////////////////////////////////////

void reset_counters()
{
 but2_counter=0;
 but0_counter=0;
}


void button0()
{
/////////////////
reset_counters();
  if(mode==0)
  {
   if(clock_mode==0)
   {
    clock_mode=1;
    if(am_pm=='a' && hour==12)
    {
     hour=0;
    }
    else if(am_pm=='p' && hour<12)
    {
     hour+=12;
    }
   }
   else
   {
    clock_mode=0;
    if(hour>12)
    {
     am_pm='p';
     hour-=12;
    }
    else if(hour<12)
    {
     if(hour==0)
      hour=12;
     am_pm='a';
    }
   }
  }
  else
  {
   pointer++;
   if(clock_mode==0)
   {
    if(pointer>=12)
    pointer=0;
   }
   else
   {
    if(pointer>=11)
    pointer=0;
   }
   selection_s(pointer);
  }
  //
////////////////




 TCNT2=0;
 on_while=1;
 while(PIND.1==1)
 {
  TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
  if(but0_counter==2)
  {
   TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
   reset_counters();
   TCNT2=0;
   if(mode==0 && mode_about==0)
   {
    about();
   }
  }
 }
 on_while=0;

 TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
 TCNT2=0;

}


void button1()
{
 if(mode==0 && mode_about==1)
 {
  mode_about=0;
  show_cl=1;
  glcd_clear();
  glcd_putimagef(69,0,clock_0,GLCD_PUTCOPY);
  glcd_setfillcolor(0);
  glcd_fillcircle(x,y,32);
 }
 if(mode==1)
 {
  //while(PIND.2==1);
  inc_selection(pointer);
 }
}


void button2()
{
 reset_counters();
 if(mode==0 && mode_about==1)
 {
  mode_about=0;
  show_cl=1;
  glcd_clear();
  glcd_putimagef(69,0,clock_0,GLCD_PUTCOPY);
  glcd_setfillcolor(0);
  glcd_fillcircle(x,y,32);
 }

 if(mode==1)
 {
  mode=0;
  glcd_outtextxy(0,0,"            ");
  glcd_outtextxy(0,20," ");
  glcd_outtextxy(0,30," ");
 }

 on_while=1;
 TCNT2=0;
 while(PIND.3==1)
 {
  TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
  if(but2_counter==2)
  {
   TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
   reset_counters();
   TCNT2=0;
   if(mode==0 && mode_about==0)
   {
    mode=1;
    pointer=0;
    //TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    selection_s(pointer);
    //TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    glcd_outtextxy(0,0,"Setting Mode");
   }
  }
 }
 on_while=0;

 TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
 TCNT2=0;
 reset_counters();

}


////////////////////////////////////

void main(void)
{
GLCDINIT_t glcd_init_data;

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);


// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: CTC top=OCR1A
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 1 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: On
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x7A;
OCR1AL=0x11;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
//ASSR=0<<AS2;
ASSR=0x08;
TCCR2=0x05;
//TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0xC2;
// Timer(s)/Counter(s) Interrupt(s) initialization
//TIMSK=0x40;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT0 Mode: Rising Edge
// INT1: Off
// INT1 Mode: Rising Edge
// INT2: Off
// INT2 Mode: Rising Edge
GICR|=(0<<INT1) | (0<<INT0) | (0<<INT2);
//MCUCR=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
//MCUCSR=(1<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (0<<INTF2);

////////////////////////////
glcd_init_data.font=font5x7;
glcd_init_data.readxmem=NULL;
glcd_init_data.writexmem=NULL;
glcd_init(&glcd_init_data);

// Global enable interrupts
#asm("sei")

Rs= 32;
Rm= 28;
Rh= 18;

x=123;
y=55;

init_clock();
glcd_putimagef(69,0,clock_0,GLCD_PUTCOPY);
glcd_setfillcolor(0);
glcd_fillcircle(x,y,32);


glcd_outtextxy(0,111,"----------------------------------------");


while (1)
      {
      show_clock();
       if(PIND.1==1)
       {
        button0();
       }
       else if(PIND.2==1)
       {
        button1();
       }
       else if(PIND.3==1)
       {
        button2();
       }

      }
}

