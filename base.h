#define R 1
#define G 0
typedef struct _transinfo{
	  int   record_time             ;
	  int   record_index            ;
	  	  
    double today_open             ;
    double today_close            ;
    double today_max              ;
    double today_min              ;
    double today_change           ;
    
    double transaction_mount      ;
    double transaction_value      ;  
    double last_close             ;
    
}TRANSINFO;

typedef struct _stat{
  char stock_code[16]   ;
  
  double today_change   ;
  double today_max      ;
  double today_min      ;
  double today_amp      ;
  
  double t_mean         ;
  double t_change       ;
  double t_away_mean    ;
  double t_max          ;
  double t_min          ;
  double red_ration     ;
  int    t_red          ;
  int    t_green        ;
  
  double delt_transmount   ;
  
  int    continue_decrease ;
  int    continue_increase ;  
  
  int    close_red          ;
  int    close_green        ;       
    
}STATISTICAL;

/*
 red star  
            |       <------- u   : uper line ,price range great than entity   
            |
         -------
        |       |   <------- e   : entity  ,price range between open price and close price
         -------
            |
            |       <------- l   : lower line ,price range less than entity

*/

typedef struct _star{
	  int   color             ;  // read or green ,if close price great than open price ,red green vice versa 
	  int   u                 ; 
	  int   e                 ;
	  int   l                 ;
}STAR;

#define MAX_RECORD 1000