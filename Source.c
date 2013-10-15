/*Milestone 1 
  Students:Elena Giagkou
		   Kaiti Stavrou
*/

//vivliothikes
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <string.h>
#include<math.h>

//kathorismos global metavlitwn
#define ram_size 1024			 //megethos kirias mnimis
#define Cache 128				 //megethos krifis mnimis
#define Word_Size 4				 //megethos lexis
#define Block_Words 2			 //arithmos lexewn ana block
#define Flush 5				
#define Ways 4					 //sizeuxi sinolwn n ways(set-assosiative)
#define Temp 4					 //pollaplasiastis

//architektoniki mnimis
#define Full_assosiative 0		 //pliris sizeuxi
#define Direct_mapping 1		 //amesi apeikonisi
#define N_way_set_associative 0  // sizeuxi sinolwn

//sinartiseis programmatos
void power_of_two(int,int,int,int);			//sinartisi elexou metavlitwn an einai dinamis tou 2
void create_data(double,int );					//dimiourgia arxeiou(energeies-entoles , modify,read,flush,write)
void create_data_file(double,int,int,int );		//eisagwgi dekadikou kai metatropi diadikou sto arxeio
void file_parsing(int,int,int,int);			//o sintaktikos analitis prospelavnei olo to arxeio dedomenwn kai dimiourgei ena neo arxeio pliroforiwn-exodou

// file pointers
FILE *fp;		//pointer fp gia to arxeio dedomenwn
FILE *fs;		//pointer fs gia to arxeio pliroforiwn

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------

//Main program
int main(void)
{
	int full_assosiative=Full_assosiative;  
	int direct_mapping=Direct_mapping;
	int n_way_set_associative=N_way_set_associative;
	
	double ram=ram_size;	//dilwnume tis metavlites idies me tis define gia na exume pio efkoli xrisi autwn twn metavlitwn- logarithos dexete mono double
	double cache= Cache ;
	double word_size=Word_Size; 
	double block_words=Block_Words;
	double flush=Flush;
	double ways=Ways;
	double temp=Temp;		//pollaplasiastis

	int tag;				//tag (MSB)
	int index;				//index(middle or LSB)
	int block_offset;		//block offset (LSB)
	double words;			//sinola-blocks lexewn 
	double sets;			//sets
	
	int q=1;					//metavliti ipologismou maximum arithmou
	int File_length;		//megethos_Arxeio - poses entoles tha exei
	int Address_Bits=(log(ram/Word_Size)/(log(2.0)));		//ipologismos bits dieuthinsis logarithmos (megethos ram / megetos lexis ) se byte
	
	//metavlites ipologismou dinamis tou 2
	int ram_power=(int)ram;		
	int cache_power=(int)cache;
	int word_size_power=(int)word_size;
	int ways_power=(int)ways;	
	int flush_counter;	   // statheros arithmos , kathorizei kathe poses entoles tha efanizetai tixea to flush sto arxeio

	int max_num= 1;			//metavliti max dekadikis timis
	while (q<=Address_Bits)	//loop ipologismou max timis
	{
		max_num=2*max_num;
		q=q++;
	}
	
	//periorismoi
	
	power_of_two(ram_power,cache_power,word_size_power,ways_power);		//sinarti dinamis tou 2 

	if(cache_power<=0 || ram_power<=0 || word_size_power<=0)			//elexos timis cache,ram kai word_size an einai <0
	{
		printf("Error: Cache or Ram or Word_size cannot be zero. Change the number");	//minima lathous
		system("PAUSE");
		exit(1);			//exodos apo to programma
	}
	
	if (cache>= ram)		//elexos an i cache einai megaliteri tis ram
	{
		printf("Error: Cashe is bigger or egual with ram . Please change the define number (Cache<ram) \n");
		system("PAUSE");
		exit(1);
	}
	if ((word_size> cache) || (word_size>ram))		//elexos an to megethos tis lexis einai megalitero apo to megethos tis cache i tis ram
	{
		printf("Error: Define word size less than cache or ram memory");
		system("PAUSE");
		exit(1);
	}
	if ((cache)/(word_size)<=block_words)			//elexos an i le3is einai mikroteres apo to lexis mesa se ena block
	{
		printf("Cache words cant be less than words in a block");
		system("Pause");
		exit(1);
	}
	//elexoi architektonikis 
	//elexos an einai <0 i >1 o arithmos epilogis architecture
	if ((full_assosiative<0 || full_assosiative>1) || (direct_mapping<0 || direct_mapping>1) || (n_way_set_associative<0 || n_way_set_associative>1)) 
	{
		printf("Error: Define the choosen architecture with 1 and the non choosen with 0 \n\n");
		system("PAUSE");
		exit(1);
	}
	//elexos an einai perissoteres apo 2 architektonikes epilegmenes na ektelesthoun
	if ((full_assosiative==1 && direct_mapping==1) || (direct_mapping==1 && n_way_set_associative==1) || (n_way_set_associative==1 && direct_mapping==1))
	{
		printf("Error: Define only one architecture with 1 and the other two with 0 \n\n");
		system("PAUSE");
		exit(1);
	}
	//elexos an einai kai oi treis architektonikes 0 
	if (full_assosiative==0 && direct_mapping==0 && n_way_set_associative==0)
	{
		printf("Error: Define only two architectures with 0 not the three of them. Choose one and define it with 1 \n\n");
		system("PAUSE");
		exit(1);
	}

	//ipologismos megethu arxeiou(poses entoles) kai flush counter(meta apo poses entoles tha emfanizete tixai o flush
	File_length=((ram_size/Word_Size)*Temp);
	if ((flush>File_length))						//elexos an to flush einai megalitero apo to megeths tou arxeiou
	{
		printf("Error: flush can't be more than file length");
		system("Pause");
		exit(1);
	}
	flush_counter=(File_length/Flush);
	printf("To arxeio dedomenwn pou prokeite na dimiourgithei exei %d entoles \n",File_length);
	
	//Kalesma sinartisis dimiourgias arxeiou dedomenwn
	printf("To arxeio dimiourgeite \n");
	printf("....................................................... \n");
	printf("\n");
	

	create_data_file(Address_Bits,File_length,flush_counter,max_num); //parametroi  megethos dieuthinsis, megethos arxiou entolwn, flush_counter
	printf("to arxeio exei dimiourgithei me to to onoma data_infile \n");

	//elexos epilogis architektonikis
	if (full_assosiative==1)			//pliris sizeuxi
	{
			block_offset=(log(block_words)/(log(2.0))); //ipologismos block_offset= o logarithmos twn lexewn ana block me vasi to 2 
			index=0;									//sti pliri sizeuxi den iparxoun index 
			tag=Address_Bits-block_offset;				//ipologismos tag = o arithmos twn bits dieuthinsis - block_offset
		//ektipwsi tag,index,block,offset
			printf("The architecture is: FULL ASSOSIATIVE \n");
			printf("The Address Bits are = %d\n",Address_Bits);
			printf("The block offset is: %d\n",block_offset);
			printf("The index is: %d\n",index);
			printf("The tag is: %d\n",tag);
			
			file_parsing(Address_Bits, tag, index, block_offset); //kalesma sinartisis gia dimiourgia arxeiou pliroforiwn-e3odou 
																	//parametroi: bits diefthinsis tag index kai block offset
			
	}
	if (direct_mapping==1)			   //amesis apeikonisis
	{
			block_offset=(log(block_words)/(log(2.0)));	//ipologismos block_offset= o logartihmos twn lexewn ana block me vasi to 2
			words=((cache)/(word_size))/block_words;	//oi lexis = (cache / megethos tis lexis ) / lexis ana block
			index=((log(words))/(log(2.0)));			//index= logarithmo twn lexewn me vasi to 2
			tag=(Address_Bits-block_offset-index);		// tag = ta bits dieuthinsis - block_offset - index
		//ektipwsi tag,index,block offset
			printf("The architecture is: DIRECT MAPPING \n");
			printf("The Address Bits are= %d\n",Address_Bits);
			printf("The block offset is: %d\n",block_offset);
			printf("The index is: %d\n",index);
			printf("The tag is: %d\n",tag);
			file_parsing(Address_Bits,tag, index, block_offset); //kalesma sinartisis gia dimiourgia arxeiou pliroforiwn-e3odou 
	}
	if (n_way_set_associative==1)
	{
		if (ways_power<=0)
		{
			printf("Error: Ways can't be 0 or less");
			system("PAUSE");
			exit(1);
		}
			block_offset=(log(block_words)/(log(2.0))); //ipologismos block_offset= o logartihmos twn lexewn ana block me vasi to 2
			words=((cache)/(word_size))/block_words; //oi lexis = (cache / megethos tis lexis ) / lexis ana block
			sets=words/ways; //ipologismos sets = arithmo twn lexewn / ways
			index=((log(sets))/log(2.0));		//ipologismos index = logarithmos twn sets me vasi to 2
			tag=Address_Bits-index-block_offset; // tag = ta bits dieuthinsis - block_offset - index
		//ektipwsi tag,index,block_offset
			printf("The architecture is: %d WAY SET ASSOSIATIVE\n",ways);
			printf("The Address Bits are = %d\n",Address_Bits);
			printf("The block offset is: %d\n",block_offset);
			printf("The index is: %d\n",index);
			printf("The tag is: %d\n",tag);
			file_parsing(Address_Bits,tag, index, block_offset);
	}
	printf("For more informations please open the file data_outfile \n");
	printf("--------------------------------------------------------------------------");

	system("PAUSE");
	return(0);
}//telos programmatos

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------

//Sinartisis
void create_data_file(double add_bits,int file_length,int Flush_counter,int Max_num) //sinartisi dimiourgias arxeiou data_file me m(modify),r(read),f(flush),w(write)
{
	int i;	//metritis 
	int flush_check=0;	//metavliti pou perni tin timi tou i otan ekteleitai to flush kai ti prostheti sto Flush_counter 
	int num_letter;		//epilogi arithmou -- grammatos
	errno_t err;		// errno_t chrisimopoiite otan mia opoiadipote sinartisi epistrefei ena tipo i parametro pou exei na kanei me error codes apo ton errno
	err=fopen_s( &fp, "data_infile", "w+" ); //anoigume to arxeio data infile gia grafei
	if (err==0) // an to err einai 0 
	{
		srand((unsigned) time(NULL)); //seeds the random number generator with the current system time.
	
		for (i=0;i<file_length;i++) // for loop oso einai to megethos tou arxeiou
		{
			if (Flush!=0) //an to define flush einai >0 tote isxii to if
			{
				if(i<(Flush_counter+flush_check)) //ektelei auto to meros tou kodika oso vriskomaste se entoli mikroteri tou Flush_counter+flush_ckeck
				{
					num_letter=rand()%3+1;		//epilegi tixai apo tis 3 epiloges 1,2,3 m,r,w
					switch(num_letter)
					{
						case 1: fprintf(fp,"m ");		//modify
								create_data(add_bits,Max_num);	//kalesma sinartisis gia dimiourgia dekadikou kai metatropi tou se diadiko
								break;
						case 2: fprintf(fp,"r ");		//read
								create_data(add_bits,Max_num);
								break;
						case 3: fprintf(fp,"w ");		//write
								create_data(add_bits,Max_num);
								break;
						default:printf("Error! random is not correct"); //minima lathous
					}
				}
				else		//an i entoli pou vriskomaste einai isi me to flush_counter +flush_check  tha isxi to else kai tha kanume random kai to flush
				{
					num_letter=rand()%4+1;		//epilogi metaxi 1,2,3,4 m,r,w,f
					switch(num_letter)			//epilogi grammatos aanaloga me ton arithmo
					{
						case 1: fprintf(fp,"m ");		//modify
								create_data(add_bits,Max_num);	//kalesma sinartisis gia dimiourgia dekadikou kai metatropi tou se diadiko
								break;
						case 2: fprintf(fp,"r ");		//read
								create_data(add_bits,Max_num);
								break;
						case 3: fprintf(fp,"w ");		//write
								create_data(add_bits,Max_num);
								break;
						case 4: fprintf(fp,"f \n");		//flush
								flush_check=i;			//to flush check isoutai me tin entoli pou exume twra outws ostw meta apo flusch_ckeck +flush counter na emfanissti tixea i flush
								break;
						default:printf("Error! random is not correct");//minima lathous
					}
				}
			}
			else //aliws an den iparxun katholou flush tha epilegi apo autes tis 3 epiloges
			{
				num_letter=rand()%3+1; //epilogi tixaiou arithmou 1,2,3 kai antistixisi me to gramma
					switch(num_letter)
					{
						case 1: fprintf(fp,"m ");		//modify
								create_data(add_bits,Max_num);
								break;
						case 2: fprintf(fp,"r ");		//read
								create_data(add_bits,Max_num);
								break;
						case 3: fprintf(fp,"w ");		//write
								create_data(add_bits,Max_num);
								break;
						default:printf("Error! random is not correct");
					}
				}
			}
	}	
err= fclose( fp);	//klinoume to arxeio dedomenwn data_infile
}

void create_data(double bits,int max) //sinartisi me parametro ta bits diefthinsis
{
	//arxikopoiisi timwn
	int counter1=0;
	int counter2=0;
	int counter3=0;
	int binary[256];
	int rand_num;
	int decimal=0;
	for (counter1=0;counter1<bits;counter1++)	//midenizume tis thesis tou pinaka analoga me ton arithmo twn bits
		{
			binary[counter1]=0;		//midenismos mia pros mia thesis
		}
	rand_num=rand()%max;		//epilogi enos tixeou dekadikou arithmou 0-max
	decimal=rand_num;			
	while(rand_num>0)		//ipologismos dekadikou se diadiko arithmo
		{
			binary[counter2]=rand_num%2;
			counter2++;
			rand_num=rand_num/2;
		}
	for (counter3=(int)(bits-1);counter3>=0;counter3--) //ektipwsi pinaka apo tin anapodi bits-0 sto arxeio
		{
			fprintf(fp,"%d",binary[counter3]);
		}
	fprintf(fp," %d",decimal);	//emfanisi dekadikou sto arxeio
	fprintf(fp,"\n");
}

void file_parsing(int address_bit, int Tag, int Index, int Block_offset) // prospelasi arxeiou dedomenwn gia dimiourgia arxeiou e3odou
{                                                                           // parametroi: bit diefthinsis ,tag index kai block_offset
	char line[128]; //pinakas me ta stoixeia pou tha kanei parsing o parser
	errno_t err1;	
	errno_t err2;
	int i;
	int z=0;
	err1=fopen_s(&fp,"data_infile","r");	//anoigma arxeiou gia anagnwsi mono
	if(err1==0)
	{
		err2=fopen_s(&fs,"data_outfile","w");		//anoigma arxeiou  gia grafei mono
		if (err2==0)
		{	
			fprintf(fs,"MODE	         BINARY			TAG		INDEX		BLOCK OFFSET\n");	//titloi arxeiou
			fprintf(fs,"---------------------------------------------------------------------------------------------\n");
				while ((fgets(line,128,fp))!=NULL)		//mexri na ftasti sto telos tis grammis pernei ena ena charaktira kai to topotheta sto pinaka
				{
					if (line[0]=='m')			//prwti thesi == m 
					{
						fprintf(fs,"Modify		");	//ektipwsi sto arxeio Modify
					}
					if (line[0]=='f')			//an i prwti thesi == f 
					{
						fprintf(fs,"Flush		");		//ektipwsi sto arxeio Flush
					}
					if (line[0]=='w')		//an i prwti thesi == w
					{
						fprintf(fs,"Write		");		//ektipwsi sto arxeio Write
					}
					if (line[0]=='r')		// an i prwti thesi = r
					{
						fprintf(fs,"Read		");	//ektipwsi sto arxeio Read
					}
					if (line[0]!='f')		//an den einai flush tote na ipologizei ta exis:
					{
						for (i=2;i<=address_bit+1;i++)			//emfanizi ta bit tis diefthinsis
						{
							fprintf(fs,"%c",line[i]);		//ektipwni ta bits diefthinsis sto arxeio
						}
						fprintf(fs,"		");
						for (i=2;i<=Tag+1;i++)				//epilegei apo ti thesi 2 mexri ti thesi Tag+1 gia tin epilogi tou tag
						{
							fprintf(fs,"%c",line[i]);		//ektipwni ta bits ta sto arxeio
						}
						fprintf(fs,"		");
						for(i=Tag+2; i<=Tag+Index+1; i++)	//epilegei ta bits apo ti thesi Tag+2 mexri tin Tag+index+1 gia index
						{
							fprintf(fs,"%c",line[i]);	//ektipwni ta bits index sto arxeio
						}
						fprintf(fs,"		 ");
						for(i=Tag+Index+2; i<=Tag+Index+Block_offset+1; i++)	//epilegei ta bits tag +index +2 mexri tag +index+block_oofset +1 gia block_offset
						{
							fprintf(fs,"  %c",line[i]); //ektipwsi bits block_offset sto arxeio
						}
					}
					
					fprintf(fs,"\n");
					
				}
		}
	}
err1=fclose(fp);//klisimo arxeiou data_infile
err2=fclose(fs);//klisimo arxeiou data_outfile
}

void power_of_two(int Ram_power,int Cache_power,int Word_size_power,int Ways_power) //elexos an oi times tis ram tis cache tou word size kai twn ways p dothikan apo to christi einai dinamis tou 2
{
	 while((Ram_power % 2) == 0) //kathws to ipolipo tis timis tis ram einai 0 na dierume ti ram me to 2
	{
		Ram_power = Ram_power / 2;
	 }
	 if (Ram_power>1)		//an i timi sto telos tis ram einai >1 => den einai dinami tou 2
	 {
		 printf("Error: ram is not power of 2");
		 system("PAUSE");
		 exit(1);
	 }
	
	 while((Cache_power % 2) == 0)//kathw to ipolipo tis timis tis cache einai 0 na dierume ti cache me to 2
	{
		Cache_power = Cache_power / 2;
	 }
	 if (Cache_power>1)		//an i timi sto telos tis ram einai >1 => den einai dinami tou 2
	 {
		 printf("Error: cache is not power of 2");
		 system("PAUSE");
		 exit(1);
	 }
	 while ((Word_size_power % 2)==0) //kathws to ipolipo tis timis tou word size einai 0 na dierume ti cache me to 2
	 {
		 Word_size_power= Word_size_power/2;
	 }
	 if (Word_size_power>1) //an i timi sto telos tou word size einai >1 => den einai dinami tou 2
	 {
		 printf("Error: word_size is not power of 2");
		 system("PAUSE");
		 exit(1);
	 }
	  while ((Ways_power % 2)==0)//kathws to ipolipo tis timis twn ways einai 0 na dierume ti cache me to 2
	 {
		 Ways_power= Ways_power/2;
	 }
	 if (	Ways_power>1)  //an i timi sto telos twn ways einai >1 => den einai dinami tou 2
	 {
		 printf("Error: ways are not power of 2");
		 system("PAUSE");
		 exit(1);
	 }
}
