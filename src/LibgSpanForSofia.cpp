/*
    $Id: main.cpp,v 1.4 2004/05/21 05:50:13 taku-ku Exp $;
 
   Copyright (C) 2004 Taku Kudo, All rights reserved.
     This is free software with ABSOLUTELY NO WARRANTY.
  
   This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.
    
   This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
    
   You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
     02111-1307, USA
*/
#include <LibgSpanForSofia.h>

#include "gspan.h"
#include <unistd.h>
#include <assert.h>
#include <cstdio>

#define OPT " [-m minsup] [-d] [-e] [-w] "

namespace LibgSpanForSofia {
/////////////////////////////////////
	
void usage (void)
{
	std::cout << "gspan implementation by Taku Kudo" << std::endl;
	std::cout << std::endl;
	std::cout << "usage: gspan [-m minsup] [-D] [-e] [-w] [-L maxpat] {InputFile}" << std::endl;
	std::cout << std::endl;
	std::cout << "options" << std::endl;
	std::cout << "  -h, show this usage help" << std::endl;
	std::cout << "  -m minsup, set the minimum support (absolute count)" << std::endl;
	std::cout << "  -D, use directed edges, default: undirected" << std::endl;
	std::cout << "  -e, output substructures in encoded form (?)" << std::endl;
	std::cout << "  -w, where (?)" << std::endl;
	std::cout << "  -n minnodes, the minimum number of nodes in substructes (default: 0)" << std::endl;
	std::cout << "  -N maxnodes, the maximum number of nodes in substructes (default: -1)" << std::endl;
	std::cout << std::endl;

	std::cout << "The graphs are read from stdin, and have to be in this format:" << std::endl;
	std::cout << "t" << std::endl;
	std::cout << "v <vertex-index> <vertex-label>" << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "e <edge-from> <edge-to> <edge-label>" << std::endl;
	std::cout << "..." << std::endl;
	std::cout << "<empty line>" << std::endl;
	std::cout << std::endl;

	std::cout << "Indices start at zero, labels are arbitrary unsigned integers." << std::endl;
	std::cout << std::endl;
}
	
void puti ( FILE *f, int i ) {
  char array[100];
  int k = 0;
  do {
    array[k] = ( i % 10 ) + '0';
    i /= 10;
    k++;
  }
  while ( i != 0 );
  do {
    k--;
    putc ( array[k], f );
  } while ( k );
}

FILE* output;
// A simple callback function that just print the result in a similar way as the original algorith did (it prints the graph to 'ouput' file)
bool LibgSpanAPI PrintToFileCallback( LibgSpanDataRef, const LibgSpanGraph* graph )
{
  assert( graph != 0 );

  putc ( '#', output );
  putc ( ' ', output );
  puti ( output, graph->Support );
  putc ( '\n', output );

  static int counter = 0;
  counter++;
  putc ( 't', output );
  putc ( ' ', output );
  putc ( '#', output );
  putc ( ' ', output );
  puti ( output, (int) counter );
  putc ( ' ', output );
  putc ( '*', output );
  putc ( ' ', output );
  puti ( output, (int) graph->Support );
  putc ( '\n', output );
  for ( int i = 0; i < graph->VertexCount; i++ ) {
    putc ( 'v', output );
    putc ( ' ', output );
    puti ( output, (int) i );
    putc ( ' ', output );
    puti ( output, (int) graph->Vertices[i] );
    putc ( '\n', output );
  }
  for ( int i = 0; i < graph->EdgeCount; i++ ) {
    putc ( 'e', output );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].From );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].To );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].Label );
    putc ( '\n', output );
  }
  putc( 'x', output );
  for( int i = 0; i < graph->Support; ++i ) {
	  putc( ' ', output );
	  puti( output, (int) graph->Objects[i] );
  }
  putc( '\n', output );
  
  return true;
}

/////////////////////////////////////
}

bool LibgSpanAPI RungSpan( LibgSpanDataRef data,
    const char* inputFileName, int support, ReportGraphCallback callback, int minsize, int maxsize, bool directed )
{
	LibgSpanForSofia::gSpan gspan;
	gspan.run (inputFileName, callback, data, support, minsize, maxsize, directed);
	return true;
}

// A main function for testing properties
int main (int argc, char **argv)
{
	unsigned int minsup = 1;
	unsigned int maxnodes = 0xffffffff;
	unsigned int minnodes = 0;
	bool where = false;
	bool enc = false;
	bool directed = false;

	int opt;
	while ((opt = getopt(argc, argv, "edws::m:N:Dhn:")) != -1) {
		switch(opt) {
		case 's':
		case 'm':
			minsup = atoi (optarg);
			break;
		case 'n':
			minnodes = atoi (optarg);
			break;
		case 'N':
			maxnodes = atoi (optarg);
			break;
		case 'd': // same as original gSpan
		case 'e':
			enc = true;
			break;
		case 'w':
			where = true;
			break;
		case 'D':
			directed = true;
			break;
		case 'h':
		default:
			LibgSpanForSofia::usage ();
			return -1;
		}
	}

	const char* inputFileName = argv[optind];
	LibgSpanForSofia::output = fopen ( "./gSpan-patterns.out", "w" );
	ReportGraphCallback callback = LibgSpanForSofia::PrintToFileCallback;

	LibgSpanForSofia::gSpan gspan;
	gspan.run (inputFileName, callback, 0, minsup, minnodes, maxnodes, directed);

	fclose(LibgSpanForSofia::output);
}
