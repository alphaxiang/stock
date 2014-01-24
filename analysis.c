#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "base.h"
#include "assert.h"

int great = 0;
int less  = 0;
int delt_time = 5 ;
int misc = 1 ;

void print_sat(STATISTICAL sat ,int measure ,int misc)
{
	 if(measure == 1) {
            printf("%s\n" ,sat.stock_code );
            
            if(!misc) {
	       printf("%d Day mean  %.2f\n" ,delt_time ,sat.t_mean);
	       printf("%d Day delt  %.2f\n" ,delt_time ,sat.t_change);	
	       printf("%d Day max   %.2f\n" ,delt_time ,sat.t_max);
	       printf("%d Day min   %.2f\n" ,delt_time ,sat.t_min);
	       printf("Away  mean   %.2f\n\n" ,sat.t_away_mean); 
            }
	 }
	 
	 if(measure == 2 && sat.continue_increase > 0) {

            printf("%s\n" ,sat.stock_code );            
            if(!misc)  printf("Continue Increase Day  %d\n\n" ,sat.continue_increase);
	 }
	 
	 if(measure == 3 && sat.continue_decrease > 0){

            printf("%s\n" ,sat.stock_code );
            if(!misc) printf("Continue Decrease Day  %d\n\n" ,sat.continue_decrease);
	 }
	 
	 if(measure == 4) {

           printf("%s\n" ,sat.stock_code );
           if(!misc)  printf("Trans Mount Change     %.2f\n\n" ,sat.delt_transmount);
	 }
	 
	 if(measure == 5 && sat.close_red > 0) {

           printf("%s\n" ,sat.stock_code );
           if(!misc) printf("continue close with red days %d\n\n" ,sat.close_red);
	 }
	 if(measure == 6 && sat.close_green > 0) {

           printf("%s\n" ,sat.stock_code );
           if(!misc) printf("continue close with green days %d\n\n" ,sat.close_green);
	 }
	 
	 if(measure == 7) {

            printf("%s\n" ,sat.stock_code );
            if(!misc) printf("red day  %d   green day   %d\n\n"  ,sat.t_red ,sat.t_green);   
	 }
}

int Star(STAR * star ,TRANSINFO transinfo)
{
	double entity_max ,entity_min ,max ,min ,d;
	
	max = transinfo.today_max ;
	min = transinfo.today_min ;
	
	if(transinfo.today_open > transinfo.today_close) {
		entity_max = transinfo.today_open ;
		entity_min = transinfo.today_close ;
		star->color = R ;
	}else {
		entity_max = transinfo.today_close ;
		entity_min = transinfo.today_open ;
		star->color = R ;
	}
	
	if(max > entity_max && min < entity_min) {
		d = max - min ;
		star->u = 100 * (max - entity_max) / d ;
		star->l = 100 * (entity_min - min) / d ;
		star->e = 100 - star->u - star->l ;
	} else {
		return 0 ; // no star
	}
}

STATISTICAL analysis(char * name)
{
	TRANSINFO * tmp ;
	STATISTICAL stat_info ;
	int i ,k ,n ,length ,index;
	FILE * fp ;
	
	tmp = (TRANSINFO *)malloc(MAX_RECORD * sizeof(TRANSINFO));
	fp = fopen(name ,"r+");
	assert(tmp != NULL && fp != NULL);
	
	fseek(fp, 0, SEEK_END); 
  length = ftell(fp);
  
  n = length / sizeof(TRANSINFO) ;
  n = n < MAX_RECORD ? n : MAX_RECORD ; 
  
	index = n ;
  fseek(fp, 0, SEEK_SET); 
	fread(tmp ,sizeof(TRANSINFO) ,n ,fp) ;
  strcpy(stat_info.stock_code ,name) ;
  stat_info.today_change = tmp[index - 1].today_change ;
  stat_info.today_max = 100.0 * (tmp[index - 1].today_max - tmp[index - 1].last_close) / tmp[index - 1].last_close;
  stat_info.today_min = 100.0 * (tmp[index - 1].today_min - tmp[index - 1].last_close) / tmp[index - 1].last_close;
  stat_info.today_amp = tmp[index - 1].today_max  - tmp[index - 1].today_min ;
  stat_info.close_red = stat_info.close_green = 0 ;
  
  if(index > 1 ) {
  	stat_info.delt_transmount = tmp[index - 1].transaction_mount / tmp[index - 2].transaction_mount ;
  } else {
  	stat_info.delt_transmount = 1 ;
  }
  
  for(i = index ;i > 0 ;i--) {
  	// close with red 
  	if(tmp[i-1].today_close > tmp[i-1].today_open)
  		 stat_info.close_red++ ;
    else 
    	 break ;
  }
  
  for(i = index ;i > 0 ;i--) {
  	// close with green 
  	if(tmp[i-1].today_close < tmp[i-1].today_open)
  		 stat_info.close_green++ ;
  	else
  		 break ;
  }
  
  k = index > delt_time ? index - delt_time + 1: 1 ;
  stat_info.t_red = stat_info.t_green = 0 ;
  stat_info.t_change = stat_info.t_mean = 0.0 ;
  stat_info.t_min = stat_info.t_max = tmp[index - 1].today_close ;
  
  for(i = k ; i <= index  ;i++) {

  	stat_info.t_mean   += tmp[i - 1].today_close ;
  	stat_info.t_change += tmp[i - 1].today_change;

  	if(tmp[i-1].today_close < tmp[i-1].today_open)
  		stat_info.t_green++ ;
  	else
  		stat_info.t_red++   ;
  		
  	if(tmp[i-1].today_max > stat_info.t_max) stat_info.t_max = tmp[i-1].today_max ;
  		
    if(tmp[i-1].today_min < stat_info.t_min) stat_info.t_min = tmp[i-1].today_min ;
  			
  }
  
  stat_info.red_ration = (double)stat_info.t_red /(stat_info.t_red + stat_info.t_green) ;	
  stat_info.t_mean = stat_info.t_mean / (index - k + 1) ;
  stat_info.t_away_mean = 100.0 * (tmp[index - 1].today_close - stat_info.t_mean) / stat_info.t_mean ;
  
  i = index ; k = 0 ;
  
  while(i > 0 && tmp[i - 1].today_change >= 0) {
  	  k++ ;	i-- ;
  }
  stat_info.continue_increase = k ;
  
  i = index ; k = 0 ;
  
  while(i > 0 && tmp[i - 1].today_change <= 0) {
  	k++ ;	i-- ;
  }
  
  stat_info.continue_decrease = k ;
   
  free(tmp) ; fclose(fp);
  
  return stat_info ;
}

int main(int argc ,char **argv)
{
	int i ,j ,measure = -1 ,swap,sum = 0;
	char * stock_list ;
	char buf [1024] = {0} ;
	STATISTICAL * analysis_info ,tmp ;
	FILE * fp ;
	
	for(i = 0 ;i < argc ;i++) {

		if(strcmp(argv[i] ,"-list") == 0) {
			stock_list = argv[i+1] ;
		} else if(strcmp(argv[i],"-delta") == 0) {
			delt_time = atoi(argv[i+1]);
		}else if(strcmp(argv[i],"-misc") == 0) {
			misc = 0;
		}else if(strcmp(argv[i],"-measure") == 0) {
			measure = atoi(argv[i+1]);
		}else if(strcmp(argv[i],"-great") == 0) {
			great = atof(argv[i+1]);
		}else if(strcmp(argv[i],"-less") == 0) {
			less = atof(argv[i+1]);
		}
	}
	
	if(stock_list == NULL || measure < 0) {
		printf("Useage :\n");
		printf(" -list     stock list file\n");
		printf(" -delta    Days\n");
		printf(" -measure  Measure Method\n");
		printf(" -misc     output control\n");
		printf(" -great    only output delta change great than\n");
		printf(" -less     only output delta change less than\n");
		printf("Measure Method :\n");
		printf("    1 Measure By Delt Time Price Change\n");
		printf("    2 Measure By Continue Increase Day\n");
		printf("    3 Measure By Continue Decrease Day\n");
		printf("    4 Measure By Transact Mount Change\n");
		printf("    5 Measure By Close With Red day\n");
		printf("    6 Measure By Close With Green day\n");
		printf("    7 Measure By Close With Red Ration\n");
		return 0 ;
	}
	
	analysis_info = (STATISTICAL *)malloc(10000 * sizeof(STATISTICAL));	
  fp = fopen(stock_list,"r+");

  assert(fp != NULL && analysis_info != NULL) ;
  
	while(fgets(buf ,1024 ,fp)) { 
     buf[strlen(buf) - 1] = 0 ;
	   analysis_info[sum++] = analysis(buf);
	   memset(buf ,0x00 ,1024);
	}
	
	for(i = 0 ;i < sum ;i++) for(j = i +1 ;j < sum ;j++) {
	    	
	    	swap = 0 ; 	
	    	if(measure == 1 ) {
	    	   if(analysis_info[i].t_change < analysis_info[j].t_change ) swap = 1 ;
        } else if (measure == 2) {
           if(analysis_info[i].continue_increase > analysis_info[j].continue_increase ) swap = 1 ;
        } else if (measure == 3) {
           if(analysis_info[i].continue_decrease > analysis_info[j].continue_decrease ) swap = 1 ;
        } else if (measure == 4) {
           if(analysis_info[i].delt_transmount < analysis_info[j].delt_transmount ) swap = 1 ;
        } else if (measure == 5) {
           if(analysis_info[i].close_red > analysis_info[j].close_red ) swap = 1 ;
        } else if (measure == 6) {
           if(analysis_info[i].close_green > analysis_info[j].close_green ) swap = 1 ;
        } else if (measure == 7) {
           if(analysis_info[i].red_ration < analysis_info[j].red_ration ) swap = 1 ;
        }
        
	    	if(swap) {
	    		tmp = analysis_info[i] ;
	    		analysis_info[i] = analysis_info[j] ;
	    		analysis_info[j] = tmp ;
	    	}
	}
	
	for(i = 0 ;i < sum ;i++) {
		 if(great == 0 && less == 0)
		   print_sat(analysis_info[i] ,measure ,misc) ;
		 else if(great != 0 && analysis_info[i].t_change > great)
		 	 print_sat(analysis_info[i] ,measure ,misc) ;
                 else if(less != 0 && analysis_info[i].t_change < less)
		 	 print_sat(analysis_info[i] ,measure ,misc) ;
	}
	return 0;

}

