
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2022
// web.c header file
// NIMA BAHRAMI
//
// Homework 3
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "web.h"

// global variable used by the write_callback function
size_t length = 0;


// ------------------------------------
// Function prototype that is used by the curl library to
// retrieve the webpage (in the response message) and 
// store in the web_t webpage field.
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//

size_t write_callback(void *ptr, size_t size, size_t nmemb, char** buffer ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	*buffer = (char*)realloc( *buffer, ( length + size*nmemb )*sizeof( char* ) ); 

	strcat( *buffer, (char*) ptr );

	length += (size*nmemb);

	return size * nmemb;

} // end write_callback function

// ------------------------------------
// Function prototype that connects to the web address 
// specified in the web_t url field and retrieves
// the html webpage (using the curl c-library)
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//

int get_webpage( web_t* web_struct ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	CURL *curl;
  	CURLcode error_code = FAIL;

	length = 0;

  	curl = curl_easy_init();

  	if ( curl ) {

	    	web_struct->webpage = malloc( sizeof( char ) * length );

	    	curl_easy_setopt(curl, CURLOPT_URL, web_struct->url );
	    	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &web_struct->webpage );
	 
	    	error_code = curl_easy_perform( curl );

		curl_easy_cleanup( curl );

	}
    
	return ( error_code == OK ) ? OK : FAIL;

} // end get_response function



// ------------------------------------
// Function prototype that parses webpage/URL 
// pointed at by client socket
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//
void parse( web_t* web_struct ) {

    char *searchHref = "href=\"";
    char *currentPagePtr = web_struct->webpage, *closingQuotePtr;

    while ((currentPagePtr = strstr(currentPagePtr, searchHref)) != NULL) {
        if ((closingQuotePtr = strchr(currentPagePtr + strlen(searchHref), '"')) != NULL) {
            *closingQuotePtr = '\0';
            if (strstr(currentPagePtr + strlen(searchHref), "trump") != NULL) {
                if (web_struct->links == NULL) {
                    web_struct->links = (char **)malloc(sizeof(char *) * 256);
                    web_struct->link_cnt = 0;
                }
                web_struct->links[web_struct->link_cnt++] = strdup(currentPagePtr);
            }
            *closingQuotePtr = '"';
            currentPagePtr = closingQuotePtr;
        }
    }



	if ( WEB_DEBUG ) {
		printf("URL=%s\n", web_struct->url ); 
		printf("CNT=%d\n", web_struct->link_cnt );
		printf("WEBPAGE=%s\n", web_struct->webpage );
	}

} // end parse function
