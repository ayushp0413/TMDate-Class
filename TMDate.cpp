#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
using namespace std;
class TMDate
{
    private:
    int day;
    int month;
    int year;
    int dayNumber;
    int dayOfWeek;
    void toDayNumber();
    void fromDayNumber();
    int isLeapYear(int);
    void isValidDate(const char *,int*,int*,int*,int*);
    
    public:
    // constructors
    TMDate();
    TMDate (const char *dateString);//para
    TMDate (const TMDate &other);//copy 
    TMDate& operator=(const TMDate &other);
    virtual ~TMDate();
    // Other Methods
    int getDay();
    int getMonth();
    int getYear();
    void getMonthString(char*);
    void getMonthShortString(char*);
    int getDayOfWeek();
    void getDayOfWeekString(char*);
    void getDayOfWeekShortString(char*);
    int isLeapYear();//yaha parameters nahi hai,this is for external use 
    // operators 
    TMDate & operator=(const char *dateString);
    void operator=(TMDate &other);
    void operator+=(int days);
    void operator-=(int days);
    int operator==(const TMDate&other);
    int operator<(const TMDate&other);
    int operator>(const TMDate &other);
    int operator>=(const TMDate &other);
    int operator<=(const TMDate &other);
    void operator++(int);
    void operator--(int);
    TMDate operator+(int days);
    TMDate operator-(int days);
    
    /*int getDayNumber()//remove after testing
    {
        return this->dayNumber;
    }*/
};
TMDate:: ~TMDate()
{
    //no DMA ,no requirment
}
TMDate::TMDate()
{
    time_t x;
    time(&x);
    struct tm *now;
    now=localtime(&x);
    day=now->tm_mday;
    month=now->tm_mon+1;
    year=now->tm_year+1900;
    this->toDayNumber();
    this->dayOfWeek=3;
    
}
TMDate::TMDate (const char *dateString)
{
    int d,m,y,isValid;
    isValidDate(dateString,&isValid,&d,&m,&y);
    if(isValid)
    {
        this->day=d;
        this->month=m;
        this->year=y;
        this->toDayNumber();
        this->dayOfWeek=3;
    }
    else
    {
        this->day=0;
        this->month=0;
        this->year=0;
    }
}
TMDate::TMDate(const TMDate &other)
{
    this->dayNumber=other.dayNumber;
    this->day=other.day;
    this->month=other.month;
    this->year=other.year;
}
TMDate& TMDate:: operator=(const TMDate &other)
{
    this->dayNumber=other.dayNumber;
    this->day=other.day;
    this->month=other.month;
    this->year=other.year;
    return *this;
}
TMDate& TMDate:: operator=(const char *dateString)
{
    int d,m,y,isValid;
    isValidDate(dateString,&isValid,&d,&m,&y);
    if(isValid)
    {
        this->day=d;
        this->month=m;
        this->year=y;
        this->toDayNumber();
    }
    else
    {
        this->day=0;
        this->month=0;
        this->year=0;
    }
    return *this;
}
void TMDate::isValidDate(const char* dateString,int* isValid,int* d,int* m,int* y)
{
    int dd,mm,yy;
    char sep;
    int sepIndex1,sepIndex2;
    char mth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    
    *isValid=0;
    if(dateString==NULL)return;
    int x=strlen(dateString);
    if(x<8 || x>10)return;
    if(!(dateString[1]=='/' ||dateString[1]=='-' || dateString[2]=='/' || dateString[2]=='-')) return;
    //confirm 1 or 2 pr separator hai
    if(dateString[1]=='/' || dateString[1]=='-')sepIndex1=1;
    else sepIndex1=2;
    //whatever in sepIndex put in sep variable
    sep=dateString[sepIndex1];
    if(dateString[0]<48 || dateString[0]>57)return;
    if(sepIndex1==2 &&(dateString[1]<48 || dateString[1]>57))return;
    //confirm that date is numeric
    if(sepIndex1==1)
    {
        dd=dateString[0]-48;
        if(dd==0)return;
    }
    else
    {
        dd=((dateString[0]-48)*10)+(dateString[1]-48);
        if(dd==0)return;;
    }
    //----------------day done----------------------------------------------------------
    int i=sepIndex1+1;//ek ghar aage se kahani start krna h
    if(!(dateString[i+1]==sep ||dateString[i+2]==sep))return;
    if(dateString[i+1]==sep)sepIndex2=i+1;
    else sepIndex2=i+2;
    if(dateString[i]<48 || dateString[i]>57)return;
    if(sepIndex2==i+2 && (dateString[i]<48 || dateString[i]>57))return;
    if(sepIndex2==i+1)mm=dateString[i]-48;
    else
    {
        mm=((dateString[i]-48)*10)+(dateString[i+1]-48);;
    }
    if(mm<1 || mm>12)return;
    //------------------Month Done------------------------------------------------------------------
    i=sepIndex2+1;
    yy=0;
    int r=0;
    while(r<=3)
    {
        if(dateString[i+r]<48 || dateString[i+r]>57)return;
        yy=(yy*10)+(dateString[i+r]-48);
        r++;
    }
    if(yy<1901)return;
    if(isLeapYear(yy))mth[1]=29;
    if(dd<1 || dd>mth[mm-1])return;
    *d=dd;
    *m=mm;
    *y=yy;
    *isValid=1;
    
}
int TMDate::isLeapYear(int y)
{
    if(y%400==0)return 1;
    else if(y%100==0)return 0;
    else if(y%4==0)return 1;
    else return 0;
}
void TMDate:: fromDayNumber()
{
    if(this->dayNumber==0)
    {
        this->day=0;
        this->month=0;
        this->year=0;
        return;
    }
    int x=this->dayNumber;
    int d,m,y;
    int daysInYear,daysInMonth;
    //to calculate year from dayNumber
    y=1901;
    while(1)
    {
        if(isLeapYear(y))daysInYear=366;
        else daysInYear=365;
        if(x<=daysInYear)break;
        x=x-daysInYear;
        y=y+1;
    }
    // to calculate month from dayNumber
    int mth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeapYear(y))mth[1]=29;
    m=0;
    while(1)
    {
        daysInMonth=mth[m];
        if(x<=daysInMonth)break;
        x=x-daysInMonth;
        m++;
    }
    m++;
    d=x;
    // cout<<d<<m<<y<<endl;
    this->day=d;
    this->month=m;
    this->year=y;
}
void TMDate::toDayNumber()
{
    int x=0;
    int y=1901;
    while(y<this->year)
    {
        if(isLeapYear(y))x=x+366;
        else x=x+365;
        y++;
    }
    
    int mth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeapYear(this->year))mth[1]=29;
    int ep=month-2;
    int m=0;
    while(m<=ep)
    {
        x=x+mth[m];
        m++;
    }
    //jan is 1 not 0 in our case
    x=x+this->day;
    this->dayNumber=x;
}
int TMDate:: getDayOfWeek()
{
    if(this->dayNumber==0)return 0;
     int leftDays=this->dayNumber%7;
     int dow=2+leftDays;
     if(dow>7)dow=dow%7;
     return dow;
}
void TMDate:: getMonthString(char *monthString)
{
    if(this->dayNumber==0)
    {
        *monthString='\0';
        return ;
    }
    if(this->month==1)strcpy(monthString,"January");
    if(this->month==2)strcpy(monthString,"February");
    if(this->month==3)strcpy(monthString,"March");
    if(this->month==4)strcpy(monthString,"April");
    if(this->month==5)strcpy(monthString,"May");
    if(this->month==6)strcpy(monthString,"June");
    if(this->month==7)strcpy(monthString,"July");
    if(this->month==8)strcpy(monthString,"August");
    if(this->month==9)strcpy(monthString,"September");
    if(this->month==10)strcpy(monthString,"October");
    if(this->month==11)strcpy(monthString,"November");
    if(this->month==12)strcpy(monthString,"December");
}
void TMDate:: getMonthShortString(char *monthShortString)
{
        if(this->dayNumber==0)
    {
        *monthShortString='\0';
        return ;
    }
    const char* mon[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    int m=0;
    while(m<13)
    {
        if(month==m)strcpy(monthShortString,mon[m-1]);
        m++;
    }
    // if(this->month==1)strcpy(monthShortString,"Jan");
    // if(this->month==2)strcpy(monthShortString,"Feb");
    // if(this->month==3)strcpy(monthShortString,"Mar");
    // if(this->month==4)strcpy(monthShortString,"Apr");
    // if(this->month==5)strcpy(monthShortString,"May");
    // if(this->month==6)strcpy(monthShortString,"Jun");
    // if(this->month==7)strcpy(monthShortString,"Jul");
    // if(this->month==8)strcpy(monthShortString,"Aug");
    // if(this->month==9)strcpy(monthShortString,"Sep");
    // if(this->month==10)strcpy(monthShortString,"Oct");
    // if(this->month==11)strcpy(monthShortString,"Nov");
    // if(this->month==12)strcpy(monthShortString,"Dec");
}
void TMDate:: getDayOfWeekString(char *dayOfWeekString)
{
    if(this->dayNumber==0)
    {
        *dayOfWeekString='\0';
        return;
    }
    int dow=getDayOfWeek();
    if(dow==1)strcpy(dayOfWeekString,"Sunday");
    else if(dow==2)strcpy(dayOfWeekString,"Monday");
    else if(dow==3)strcpy(dayOfWeekString,"Tuesday");
    else if(dow==4)strcpy(dayOfWeekString,"Wednesday");
    else if(dow==5)strcpy(dayOfWeekString,"Thursday");
    else if(dow==6)strcpy(dayOfWeekString,"Friday");
    else if(dow==7)strcpy(dayOfWeekString,"Saturday");
}
void TMDate:: getDayOfWeekShortString(char *dayOfWeekShortString)
{
    if(this->dayNumber==0)
    {
        *dayOfWeekShortString='\0';
        return;
    }
    int dow=getDayOfWeek();
    if(dow==1)strcpy(dayOfWeekShortString,"Sun");
    else if(dow==2)strcpy(dayOfWeekShortString,"Mon");
    else if(dow==3)strcpy(dayOfWeekShortString,"Tue");
    else if(dow==4)strcpy(dayOfWeekShortString,"Wed");
    else if(dow==5)strcpy(dayOfWeekShortString,"Thu");
    else if(dow==6)strcpy(dayOfWeekShortString,"Fri");
    else if(dow==7)strcpy(dayOfWeekShortString,"Sat");
    
    
}
int TMDate:: isLeapYear()
{
    if(this->dayNumber==0)return 0;
    if(year%400==0)return 1;
    if(year%100==0)return 0;
    if(year%4==0)return 1;
    return 0;
}

// operators Overloading

void TMDate::operator+=(int days)
{
    if(this->dayNumber==0)return;
    this->dayNumber=dayNumber+days;
    this->fromDayNumber();
}
void TMDate::operator-=(int days)
{
    if(this->dayNumber==0)return;
    if(this->dayNumber<days)//if user go beyond 1901
    {
        this->day=0;
        this->month=0;
        this->year=0;
        return;
    }
    this->dayNumber-=days;
    this->fromDayNumber();
}
int TMDate:: operator==(const TMDate&other)
{
    return (this->dayNumber==other.dayNumber);
}
int TMDate:: operator<(const TMDate &other)
{
    return(this->dayNumber<other.dayNumber);
}
int TMDate:: operator>(const TMDate &other)
{
    return(this->dayNumber<other.dayNumber);
}
void TMDate:: operator=(TMDate &other)
{
    this->dayNumber=other.dayNumber;
    this->fromDayNumber();
}
int TMDate:: operator>=(const TMDate &other)
{
    return(this->dayNumber>=other.dayNumber);
}
int TMDate:: operator<=(const TMDate &other)
{
    return(this->dayNumber<=other.dayNumber);
}
void TMDate:: operator++(int)
{
    this->dayNumber=dayNumber+1;
    this->fromDayNumber();
}
void TMDate:: operator--(int)
{
    this->dayNumber=dayNumber-1;
    this->fromDayNumber();
}
TMDate TMDate::operator+(int days)
{
    if(this->dayNumber==0) return TMDate("00/00/0000");
    TMDate date;
    date.dayNumber=this->dayNumber+days;
    date.fromDayNumber();
    return date;
}
TMDate TMDate:: operator-(int days)
{
    if(this->dayNumber==0) return TMDate("00/00/0000");
    TMDate date;
    date.dayNumber=this->dayNumber-days;
    date.fromDayNumber();
    return date;
    
}
int TMDate::getDay()
{
    return this->day;
}
int TMDate::getMonth()
{
    return this->month;
}
int TMDate::getYear()
{
    return this->year;
}
ostream& operator<<(ostream &oo,TMDate &tmDate)
{
    oo<<tmDate.getDay()<<"/"<<tmDate.getMonth()<<"/"<<tmDate.getYear();
    return oo;
}
int main()
{
    TMDate date1;
    date1="04-03-2003";
    cout<<date1<<endl;
    char mon[12];
    date1.getMonthShortString(mon);
    cout<<mon<<endl;
    date1.getMonthString(mon);
    cout<<mon<<endl;
    char days[12];
    date1.getDayOfWeekShortString(days);
    cout<<days<<endl;
    date1.getDayOfWeekString(days);
    cout<<days<<endl;
    TMDate date2;
    date2=date1+10;
    cout<<date2<<endl;
    TMDate date3;
    date3=date1-19;
    cout<<date3<<endl;
    // cout<<d2.getDayOfWeek()<<endl;
    // cout<<mon<<endl;
    // cout<<d2<<endl;
    // d2+=100;
    // cout<<d2<<endl;
    // cout<<date<<endl;
    // date1=date;
    // date1+=10;
    // cout<<date1<<endl;
    // date1++;
    // cout<<date1<<endl;
    // date1--;
    // cout<<date1<<endl;
    // if(date==date1)
    // {
        // cout<<"Yes! dates are equal"<<endl;
    // }
    // else
    // {
        // cout<<"Dates are not equal"<<endl;
    // }
    //  cout<<date.getDayNumber()<<endl;
    // date.fromDayNumber();
    // date+=10;
    //cout<<date<<endl;
    // date-=10;
    // cout<<date<<endl;
    return 0;
}