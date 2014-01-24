#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "base.h"

const char * script [] =
{
"#!/usr/bin/env python\n"

"import sys\n"
"import xlrd\n"
"import time\n"

"from xlrd import open_workbook,cellname\n"

"book = open_workbook(sys.argv[1]) \n"
"sheet = book.sheet_by_index(0) \n"
 
"s =  sys.argv[1]\n"
"reload(sys)\n"
"if (len(sys.argv) == 2):\n"
"   str_year = time.strftime('%Y',time.localtime(time.time()))  \n"
"else:\n"
"   str_year=sys.argv[2]\n"
"str_month_day = sheet.cell(0,1).value\n"
"str_month_day = str_month_day[0:2] + str_month_day[3:6]\n"
"time_stamp =  str_year + str_month_day + '\\n'\n"

"f = open(\"tmp.txt\",\"w+\");\n"
"f.write(time_stamp)\n"

"row_index = 2\n"
"while row_index < sheet.nrows: \n"
"        line =  '%s  ,'  %( sheet.cell(row_index,0).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,2).value)\n"
"        line += '%f  ,'  %(100 * sheet.cell(row_index,3).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,7).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,8).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,9).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,10).value)\n"
"        line += '%f  ,'  %( sheet.cell(row_index,11).value)\n"
"        line += '%f'  %( sheet.cell(row_index,12).value)\n"
"        line += '\\n'\n"
"        f.write(line)\n"
"        row_index = row_index + 1\n"
"f.close()  \n"
};

int main(int argc ,char **argv)
{
        int i ,length ,record_time                  ;
        double today_open ,today_close ,last_close  ;
        double today_max ,today_min ,today_change   ;
        double transaction_mount ,transaction_value ;
        char cmd_line [256] , buf [1024] = {0}      ;
        TRANSINFO sat , last_sat                    ;
        FILE * tmp  , *fp                           ;
  
        if(argc == 2)
	   sprintf(cmd_line,"python ./proc.py  %s" ,argv[1]);
        else if(argc == 3)
	   sprintf(cmd_line,"python ./proc.py  %s %s" ,argv[1],argv[2]);
		
	fp = fopen("proc.py","w+");
	
	for(i = 0 ;i < sizeof(script)/sizeof(script[0]) ;i++) 
	   fwrite(script[i] ,strlen(script[i]) ,1 ,fp);  
	   
	fclose(fp);
	
	system(cmd_line); 
        system("rm proc.py");
  
	fp = fopen("tmp.txt","r+");
	fgets(buf ,1024 ,fp) ;
	sscanf(buf ,"%d" ,&record_time);
	printf("record time %d\n",record_time);
	memset(buf ,0x00 ,1024);
	
	while(fgets(buf ,1024 ,fp)) { 
		
			   sscanf(buf + 11,"%lf ,%lf ,%lf ,%lf ,%lf ,%lf ,%lf ,%lf " ,&today_close  ,&today_change ,
  				  &transaction_mount ,&transaction_value,&today_open ,&last_close,&today_max ,&today_min);
 
 			   if(transaction_mount == 0) continue ;
  
		           sat.record_time   = record_time     ;
		           sat.today_open    = today_open      ;
			   sat.today_close   = today_close     ;
			   sat.last_close    = last_close      ;
			   sat.today_max     = today_max       ;
 			   sat.today_min     = today_min       ;
  			   sat.today_change  = today_change    ;
  			   sat.transaction_mount = transaction_mount ;
  			   sat.transaction_value = transaction_value ;  
 
 		           buf[8] = 0 ;
  			   tmp = fopen(buf ,"aw+"); 
  
  			   if(tmp == NULL) {  	
  				printf("name path%s\n",buf);
    				printf("OPEN RECORD FILE FAILED.....\n");
  			   } else {	
  				fseek(tmp, 0, SEEK_END); 
  				length = ftell(tmp);
  	
  				if(length > 0) {  		
  		  			sat.record_index   = length / sizeof(TRANSINFO)  + 1 ;
  		  			fseek(tmp, length - sizeof(TRANSINFO), SEEK_SET); 
  		  			fread(&last_sat ,sizeof(TRANSINFO) ,1 ,tmp) ;
  		
  		  			if(record_time > last_sat.record_time) {
  		      			      fwrite(&sat ,sizeof(TRANSINFO) ,1 ,tmp) ;
  		      		        }       				  		    
  			         } else {
  					sat.record_index   = 1                  ;
      					fwrite(&sat ,sizeof(TRANSINFO) ,1 ,tmp) ;
    			         }
   					fclose(tmp);
  			   }
  			   
                           memset(buf ,0x00 ,1024); 
	  }
	
	  system("rm tmp.txt");
          return printf("\n") ;
}
