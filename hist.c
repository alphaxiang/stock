/*
Copyright reserved for alphaxiang     
       Author :alphaxiang  
       Email  :1907767981@qq.com
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "base.h"
#include "assert.h"

void float_print( const char * str ,double x ,int newline)
{
	printf("%s: ",str);
	
	if(x < -9.995) 
		 printf("%.2f  ",x) ;
        else if(x > 9.995 )
  	         printf(" %.2f  ",x);
        else if(x < 0)
  	         printf(" %.2f  ",x);
        else 
  	         printf("  %.2f  ",x);
  	 
        if(newline == 1) printf("\n");
}

void print_sat(char * name ,TRANSINFO sat)
{
	 printf("%d  " ,sat.record_time);
         float_print("Close",sat.today_close ,0);  
	 float_print("Open_Change",100 * (sat.today_open - sat.last_close)/sat.last_close ,0);	 
	 float_print("MAX_Change",100 * (sat.today_max - sat.last_close)/sat.last_close ,0);
	 float_print("MIN_Change",100 * (sat.today_min - sat.last_close)/sat.last_close ,0);
	 float_print("Today_Change",sat.today_change ,0); 	
	 
	 if(sat.today_close > sat.today_open)
	 	  printf("R    ");
	 else 
	 	  printf("G    ");  
	 	  
	 printf("Transaction_mount %.2f\n" ,sat.transaction_mount); 
}

int print_hist(char * name ,char *arg)
{
	TRANSINFO * tmp ;
	int i ,j ,k ,n ,length;
        double total_change = 0.0 ;
	FILE * fp ;
	
	tmp = (TRANSINFO *)malloc(MAX_RECORD * sizeof(TRANSINFO));
	fp = fopen(name ,"r+");
	
	assert(tmp != NULL && fp != NULL);
	
	fseek(fp, 0, SEEK_END); 
        length = ftell(fp);
  
        n = length / sizeof(TRANSINFO) ;
        n = n < MAX_RECORD ? n : MAX_RECORD ; 
  
        printf("%s   Total %d Transaction record found\n" ,name,n);
  
        if(strcmp(arg ,"-a") == 0 ) {
  	
           fseek(fp, 0, SEEK_SET); 
	   fread(tmp ,sizeof(TRANSINFO) ,n ,fp) ;
	   
	   for(i = 0 ;i < n ;i++) {
              print_sat(name ,tmp[i]) ; 
              total_change += tmp[i].today_change ;
           }
           
           printf("%s   :%d Day Total Change %.2f\n",name ,n ,total_change);
        }
  
        return n ;
}

int main(int argc ,char **argv)
{
	if(argc != 3) 
		 return printf("Bad arguments\n");
		 
	return print_hist(argv[1] ,argv[2]);

}
