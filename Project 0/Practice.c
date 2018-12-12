#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "sorter.h"
#include "mergeSort.c"



record* SortedMerge(record *a, record *b, char* column);
void Split(record *source, record **frontRef, record **backRef);
void mergeSort(record **headRef, char* column);
void sorter(FILE* fPointer,char* filename, char* fieldname);

char *strtok_new(char *comma, char *delims) // This method builds off the strtok() method, where it takes in a char* and outputs a char* before the first delimiter.
											// Since strtok does not account for the delimiters being consecutive, this method will take care of that and assign NULL to the field between the two delimiter.
											// EX) say the input string is "ab,c,,de" -> the 1st field will get "ab", the 2nd field will get "c", the 3rd field will get NULL, and the 4th field will get "de"

{
	static char *savecomma = NULL;
	char *p;
	int n;

	if(comma != NULL)				// checks if the input char* is empty or not, if not empty, variable savecomma will get the input char*
	{
		savecomma = comma;
	}
	if(savecomma == NULL || *savecomma == '\0')  //if input char* empty, return value will be NULL since an empty char* is being inputed
	{
		return(NULL);
	}
	n = strcspn(savecomma,delims);   // return the first location where the savecomma and delims are the same
	p = savecomma; 					// once found, assign it to the output variable p
	savecomma += n;					// move the pointer in savecomma by the location found from strcspn

	if(*savecomma != '\0')
	{
		*savecomma++ = '\0';
	}
	return(p);
}
void handleFile(char* filename,char *fieldname){
	FILE *fp;
	fp=fopen(filename,"r");
	sorter(fp, filename, fieldname);
	
	/*char buff[10000];//trying to determine if the csv file has a proper header line with 28 columns
	fgets(buff,10000,fp);
	int i, count=0; 
	for(i=0;i<=800;i++){
		if(buff[i]==','){
		count++;
		}
	}
	if(count!=27){
		printf("Improper header file, file ignored\n");
		return;
	}
	else{
		printf("%s is a proper header file! Congrats!\n",filename);
	sorter(fp, filename, fieldname);	
	}
	return;
*/}
void handy(char * dname,char *column){
	DIR *dp=opendir(dname);  
	struct dirent *entry;  // has two important members (d-type)-tells me if its a directory (d-name) tells me current file name
	if(dp==NULL){
		printf("directory doesnt exist: %s\n", dname);
		return;
	}
	while((entry=readdir(dp))!=NULL){
		if(strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0){
		   continue;
		}

		if((entry->d_type==DT_DIR)){
			printf("The directory name is: %s\n", entry->d_name);
			
			unsigned int const sz1 = strlen(dname);
			unsigned int const sz2 = strlen(entry->d_name);
			
			char *dnewname=(char *)malloc((sz1+sz2*sizeof(char)));
			
			memcpy(dnewname,dname,sz1);
			memcpy(dnewname+sz1,"/",sizeof(char));
			memcpy(dnewname+sz1+1,entry->d_name,sz2);
			dnewname[strlen(dnewname)]='\0';
		
				   
			handy(dnewname,column);
		}
		if((entry->d_type==DT_REG)){
			char *filename = entry->d_name;
			int len = strlen(filename);
			const char *last_four = &filename[len-4]; //need to account for if filename is less than 4 characters
				if(strcmp(last_four,".csv")==0){ 
				printf("The CSV file name is: %s\n",entry->d_name); //here is where i call handle file and merge sort
				
				unsigned int const sz1 = strlen(dname);
				unsigned int const sz2 = strlen(entry->d_name);
			
				char *dnewname=(char *)malloc((sz1+sz2*sizeof(char)));
			
				memcpy(dnewname,dname,sz1);
				memcpy(dnewname+sz1,"/",sizeof(char));
				memcpy(dnewname+sz1+1,entry->d_name,sz2);
				dnewname[strlen(dnewname)]='\0';
			printf("The new file name concatenated with file path is called: %s \n", dnewname);
					handleFile(dnewname,column);
				
				}		
				else{
				printf("The non CSV file name is: %s \n", entry->d_name);
				
				}
		}
		else{
			printf("The file that is being ignored is called: %s\n",entry->d_name);
	
		}
		 
	}
}
void arghandle(int argc, char* argv[],char **column, char **directory, char **outputdirectory){
if(argc<3){
			printf("not enough input arguments\n");
			exit(1);
	}
	int countc=0;
	int countd=0;
	int counto=0;
	
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i],"-c")==0){
			if(i+2>argc){
				printf("no column heading was inputed after -c, arguments are inputted incorrectly, program terminated");
				exit(1);
			}
			*column = argv[i+1]; 
			countc++;
		}
		if(strcmp(argv[i],"-d")==0){
			if(i+2>argc){
				printf("no column heading was inputed after -d, arguments are inputted incorrectly, program terminated");
				exit(1);
			}
			*directory = argv[i+1]; 
			countd++;
			}
		if(strcmp(argv[i],"-o")==0){
			if(i+2>argc){
				printf("no column heading was inputed after -o, arguments are inputted incorrectly, program terminated");
				exit(1);
			}
			*outputdirectory = argv[i+1];  
			counto++;
			}
		}
		if(countc<1||countc>1||countd>1||counto>=1){
			printf("you have incorrectly formated your input argruments, program terminated\n");
			exit(1);
		}
}
void sorter(FILE* fPointer,char* filename, char* fieldname)
{
	/*char* sortednamePart1 = (char*)malloc(sizeof(char)*800);
	char* sortednamePart2 = (char*)malloc(sizeof(char)*800);
	char* sortedname = (char*)malloc(sizeof(char)*800);
	
	sortednamePart1 = filename;
	sortednamePart2 = "-sorted<";

	strcat(sortednamePart2, fieldname);
	strcat(sortednamePart2, ">");*/
	
	/*unsigned int const sz1 = strlen(filename);
	unsigned int const sz2 = strlen(fieldname);
	
	char *sortedname=(char *)malloc((sz1+sz2+10)*sizeof(char));
	memcpy(sortedname,filename,sz1);
	memcpy(sortedname,"-sorted",7);
	memcpy(sortedname,"<",1);
	memcpy(sortedname,fieldname,sz2);
	memcpy(sortedname,">",1);*/
	

	//strcpy(sortedname, strcat(sortednamePart1,sortednamePart2));
	//FILE *sorted;
	//sorted= fopen("sorted csv", "w");

	char buff[10000];
	//fgets(buff,10000,fPointer); //skips first line of csv file
	fgets(buff,10000,fPointer); //takes next line of csv file

	record *pFirstNode;
	record *pCurrent;

	pFirstNode = malloc(sizeof(record)); //allocate memory for the first node of the linked list
	pFirstNode->next = NULL;

	char *token = strtok_new(buff, ","); // from the first line, we assign all the members of the struct their appropriate char*
	pFirstNode->color = strdup(token);

	token = strtok_new(NULL, ","); // since we are still using the same line, we need to input NULL as the char* and do the same to create the first node in the linked list
	pFirstNode->director = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->numCritic = strdup(token);

	token = strtok_new(NULL , ",");
	pFirstNode->duration = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->fbLikes = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->a3L = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->a2n = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->a1L = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->gross = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->genres = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->a1n = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->movietitle = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->numVote = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->castTotal = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->an3 = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->fnInPic = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->plotWord = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->movieLink = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->numUserReview = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->language = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->country = strdup(token);

	token = strtok_new(NULL, ",");
	pFirstNode->rating = strdup(token);

	token=strtok_new(NULL, ",");
	pFirstNode->budget = strdup(token);

	token=strtok_new(NULL, ",");
	pFirstNode->titleYear = strdup(token);

	token=strtok_new(NULL, ",");
	pFirstNode->a2L = strdup(token);	

	token=strtok_new(NULL, ",");
	pFirstNode->imbdScore = strdup(token);

	token=strtok_new(NULL, ",");
	pFirstNode->aspectRatio = strdup(token);

	token=strtok_new(NULL, ",");
	pFirstNode->MoviefbLikes = strdup(token);

	pCurrent = pFirstNode; // the current pointer is assigned to the first node

	while(fgets(buff, 6000, fPointer))  // this while loop loops until is gets to the bottom of the input csv file
	{
		if(pCurrent != NULL) // if there is more than one node in the linked list, we need to get to the last node so we can add the new node
		{
			while(pCurrent->next != NULL)
			{
				pCurrent = pCurrent->next;
			}
		}

		pCurrent->next = malloc(sizeof(record)); // allocate memory for each new struct (new node in linked list)
		pCurrent = pCurrent->next; // this line connects the linked list
		pCurrent->next = NULL; // since this is the last node, the next node will be NULL

		token = strtok_new(buff, ","); // now this assigns the members of struct their appropriate char*
		if(token[0] == ' ')
		{
			token++;
		}
		pCurrent->color = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->director = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->numCritic = strdup(token);

		token = strtok_new(NULL , ",");
		pCurrent->duration = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->fbLikes = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->a3L = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->a2n = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->a1L = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->gross = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->genres = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->a1n = strdup(token);

		token = strtok_new(NULL, ",");
		if (token[0]=='"'){
			token++;
			char* comma = (char*)malloc(sizeof(char));
			char* quote = (char*)malloc(sizeof(char));
			strcpy(comma, ",");
			strcpy(quote, "\"");

			char* temptoken = (char*)malloc(sizeof(char)*300);
			strcpy(temptoken, token);
			char* token1 = (char*)malloc(sizeof(char)*300);
			strcpy(token1, strtok_new(NULL, "\""));

			strcat(temptoken, comma);
			strcat(temptoken, token1);
			strcat(temptoken, quote);
			pCurrent->movietitle = strdup(temptoken);
		}
		else
		{
			pCurrent->movietitle = strdup(token);
		}

		token = strtok_new(NULL, ",");
		pCurrent->numVote = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->castTotal = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->an3 = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->fnInPic = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->plotWord = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->movieLink = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->numUserReview = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->language = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->country = strdup(token);

		token = strtok_new(NULL, ",");
		pCurrent->rating = strdup(token);

		token=strtok_new(NULL, ",");
		pCurrent->budget = strdup(token);

		token=strtok_new(NULL, ",");
		pCurrent->titleYear = strdup(token);

		token=strtok_new(NULL, ",");
		pCurrent->a2L = strdup(token);

		token=strtok_new(NULL, ",");
		pCurrent->imbdScore = strdup(token);

		token=strtok_new(NULL, ",");
		pCurrent->aspectRatio = strdup(token);

		token=strtok_new(NULL, "\0");
		pCurrent->MoviefbLikes = strdup(token);
	}

	pCurrent = pFirstNode; // after the linked list is created, we can move the current pointer to the first node

	//strcpy(column,fieldname);
	if(strcmp(fieldname,"color") != 0 && 
		strcmp(fieldname,"director_name") != 0 &&
		strcmp(fieldname,"num_critic_for_reviews") != 0 &&
		strcmp(fieldname,"duration") != 0 &&
		strcmp(fieldname,"director_facebook_likes") != 0 &&
		strcmp(fieldname,"actor_3_facebook_likes") != 0 &&
		strcmp(fieldname,"actor_2_name") != 0 &&
		strcmp(fieldname,"actor_1_facebook_likes") != 0 &&
		strcmp(fieldname,"gross") != 0 &&
		strcmp(fieldname,"genres") != 0 &&
		strcmp(fieldname,"actor_1_name") != 0 &&
		strcmp(fieldname,"movie_title") != 0 &&
		strcmp(fieldname,"num_voted_users") != 0 &&
		strcmp(fieldname,"cast_total_facebook_likes") != 0 &&
		strcmp(fieldname,"actor_3_name") != 0 &&
		strcmp(fieldname,"facenumber_in_poster") != 0 &&
		strcmp(fieldname,"plot_keywords") != 0 &&
		strcmp(fieldname,"movie_imdb_link") != 0 &&
		strcmp(fieldname,"num_user_for_reviews") != 0 &&
		strcmp(fieldname,"language") != 0 &&
		strcmp(fieldname,"country") != 0 &&
		strcmp(fieldname,"content_rating") != 0 &&
		strcmp(fieldname,"budget") != 0 &&
		strcmp(fieldname,"title_year") != 0 &&
		strcmp(fieldname,"actor_2_facebook_likes") != 0 &&
		strcmp(fieldname,"imdb_score") != 0 &&
		strcmp(fieldname,"aspect_ratio") != 0 &&
		strcmp(fieldname,"movie_facebook_likes") != 0)
	{
		printf("%s\n", "Not a valid input");
		return;
	}

	mergeSort(&pCurrent, fieldname);  // applies the Merge Sort on the linked list based on the the user input(argv[2])

	/*fprintf(sorted, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
			"color",
			"director_name",
			"num_critic_for_reviews",
			"duration",
			"director_facebook_likes",
			"actor_3_facebook_likes",
			"actor_2_name",
			"actor_1_facebook_likes",
			"gross",
			"genres",
			"actor_1_name",
			"movie_title",
			"num_voted_users",
			"cast_total_facebook_likes",
			"actor_3_name",
			"facenumber_in_poster",
			"plot_keywords",
			"movie_imdb_link",
			"num_user_for_reviews",
			"language",
			"country",
			"content_rating",
			"budget",
			"title_year",
			"actor_2_facebook_likes",
			"imdb_score",
			"aspect_ratio",
			"movie_facebook_likes"); // prints out the first line of the output csv with all the column heading
			
	 
	while(pCurrent != NULL)
	{
		fprintf(sorted, "%s,", pCurrent->color);
		fprintf(sorted, "%s,", pCurrent->director);
		fprintf(sorted, "%s,", pCurrent->numCritic);
		fprintf(sorted, "%s,", pCurrent->duration);
		fprintf(sorted, "%s,", pCurrent->fbLikes);
		fprintf(sorted, "%s,", pCurrent->a3L);
		fprintf(sorted, "%s,", pCurrent->a2n);
		fprintf(sorted, "%s,", pCurrent->a1L);
		fprintf(sorted, "%s,", pCurrent->gross);
		fprintf(sorted, "%s,", pCurrent->genres);
		fprintf(sorted, "%s,", pCurrent->a1n);

			char* title = (char*)malloc(sizeof(char)*500);
			strcpy(title, pCurrent->movietitle);
			char* quote = (char*)malloc(sizeof(char)*500);
			strcpy(quote, "\"");

			for(int i = 0; i < strlen(title); i++)
			{
				if(title[i] == ',')
				{
					strcat(quote, title);
					pCurrent->movietitle = strdup(quote);
					break;
				}
			}
		fprintf(sorted, "%s,", pCurrent->movietitle);
		fprintf(sorted, "%s,", pCurrent->numVote);
		fprintf(sorted, "%s,", pCurrent->castTotal);
		fprintf(sorted, "%s,", pCurrent->an3);
		fprintf(sorted, "%s,", pCurrent->fnInPic);
		fprintf(sorted, "%s,", pCurrent->plotWord);
		fprintf(sorted, "%s,", pCurrent->movieLink);
		fprintf(sorted, "%s,", pCurrent->numUserReview);
		fprintf(sorted, "%s,", pCurrent->language);
		fprintf(sorted, "%s,", pCurrent->country);
		fprintf(sorted, "%s,", pCurrent->rating);
		fprintf(sorted, "%s,", pCurrent->budget);
		fprintf(sorted, "%s,", pCurrent->titleYear);
		fprintf(sorted, "%s,", pCurrent->a2L);
		fprintf(sorted, "%s,", pCurrent->imbdScore);
		fprintf(sorted, "%s,", pCurrent->aspectRatio);
		fprintf(sorted, "%s", pCurrent->MoviefbLikes);

		pCurrent = pCurrent->next;
	}
*/	//fclose(sorter);
}

int main(int argc, char *argv[]){
	// I want to initialize the values of directory and outputdirectory to current director initially
	char* column;
	char* directory= ".";
	char* outputdirectory=".";
	
	arghandle(argc,argv,&column,&directory,&outputdirectory);
	handy(directory,column);
	
	
	
}