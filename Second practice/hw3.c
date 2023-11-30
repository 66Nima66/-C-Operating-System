
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// std lib function
#include <unistd.h>	// close function
#include <pthread.h>	// creating, running, joining threads
#include <string.h>	// string processing 
#include "web.h"	// our code :)

#define DEBUG 1 



// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2022
// hw3.c file
// NIMA BAHRAMI
// Homework 3
//
// -----------------------------------

// ---------------------------------------------
// Global variable shared by all threads
// This is not thread safe.
web_t* web_array;


// ---------------------------------------------
// Function prototypes to read and write url files
int read_url_file( char* file_path, web_t* web_array );
int write_url_file( char* file_path, web_t* web_array, int num_urls );

// ---------------------------------------------
// Function prototypes that is ran by a thread
void th_run( int* i );

int main( int argc, char *argv[] ) {

	int i = 0;


	// malloc web_t array with 100 elements
	web_array = malloc( sizeof( web_t )*100 );

	// -------------------------------------------------
	// Verify the correct number of arguments are provided
	// when the application is executed.

	if ( argc == 2 ) { 

		int num_urls = read_url_file( argv[1], web_array );

		if ( DEBUG ) printf("Number of urls in text file = %d\n", num_urls);


		pthread_t thread_array[num_urls];
		int index[num_urls];

		for ( i=0; i<num_urls; i++ ) {

			if ( DEBUG ) {
				printf("URL=%s\n", web_array[i].url ); 
				printf("CNT=%d\n", web_array[i].link_cnt );
				printf("WEBPAGE=%s\n", web_array[i].webpage );
			}
	
			index[i] = i;

			
			 if (pthread_create(&thread_array[i], NULL, (void* (*)(void*))th_run, &(index[i])) != 0)
            {
                perror("pthread_create() error");
                exit(1);
            }
			



			
		}

		void *ret;
		 for (int i = 0; i < num_urls; i++)
        {
            if (pthread_join(thread_array[i], &ret) != 0)
            {
                perror("pthread_join() error");
                exit(3);
            }
        }
		

		
		  write_url_file("make_merica_great_again.txt", web_array, num_urls);
		

	} else {

		printf( "URL file is not specified!\n" );
		printf( "Example usage: %s %s\n", argv[0], "url.txt" );

	}

	return OK;
     
} // end main function

// ------------------------------------
// Function ran by an individual thread
// 
//
// Arguments:	i = an index value (to access individual web_t structs in the web_array)
//
// Return:     	nothing (void)
//
void th_run( int* i ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	if ( web_array[ (*i) ].link_cnt == OK ) {

		parse( &web_array[ (*i) ] );

	} else printf( "[%s] failed to retrieve webpage\n", web_array[ (*i) ].url );

	pthread_exit( NULL );

} // end th_run function

// ------------------------------------
// Function that writes the identified links 
// stored in the web_t links field to formated
// text file.
// 
// 
//
// Arguments:	file_path = fully qualified path to output text file
//		web_array = pointer to array of web_t structs
//		num_urls = the total number of websites in the url.txt file
//
// Return:     	OK on success, FAIL on error
//
int write_url_file( char* file_path, web_t* web_array, int num_urls ) {


	    FILE* fp = fopen(file_path, "w+");

    for (int i = 0; i < num_urls; i++)
    {
        for (int j = 0; j < web_array[i].link_cnt; j++)
        {
            fprintf(fp, "%s, %s\n", web_array[i].url, web_array[i].links[j]);
        }
    }

    fclose(fp);
	

	return OK;

} // end write_url_file function



// ------------------------------------
// Function that reads the url in the input
// url text file.
// 
// 
// Arguments:	file_path = fully qualified path to output text file
//		web_array = pointer to array of web_t structs
//
// Return:     	Total number of urls defined in the url text file
//
int read_url_file( char* file_path, web_t* web_array ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	int url_cnt = 0;

	FILE* fhnd;

    	fhnd = fopen( file_path, "r" );

    	char line[50];

    	if ( fhnd != NULL ) {

        	while ( fgets( line, sizeof( line ), fhnd ) != NULL ) {

			line[strlen(line)-1] = '\0';
			web_array[url_cnt].url = malloc(sizeof(char)*100);
			sprintf( web_array[url_cnt].url, "www.%s", line );

			web_array[url_cnt].link_cnt = OK;

			if ( FAIL == get_webpage( &web_array[url_cnt++] ) ) {

				printf("Failed to load webpage for URL( %s )\n", web_array[url_cnt--].url );
				web_array[url_cnt].link_cnt = FAIL;				

			}

        	}

    	} else url_cnt = FAIL;

    	fclose( fhnd );

	return url_cnt;

} // end read_url_file function

