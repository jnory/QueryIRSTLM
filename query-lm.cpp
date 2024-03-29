/******************************************************************************
IrstLM: IRST Language Model Toolkit
Copyright (C) 2010 Christian Hardmeier, FBK-irst Trento, Italy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA

******************************************************************************/
/*
 06/12/2013
 This program was priginally written by IRSTLM(score-lm).
 Jun-ya Norimatsu modified for query each word.
*/

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <lmtable.h>
#include <n_gram.h>


void usage()
{
  std::cerr << std::endl << "score-lm - scores words with a language model" << std::endl;
  std::cerr << std::endl << "USAGE:"  << std::endl
	    << "       score-lm -lm <model>  [options]" << std::endl;
  std::cerr << std::endl << "OPTIONS:" << std::endl;
  std::cerr << "       -lm      language model to use (must be specified)" << std::endl;
  std::cerr << "       -dub     dictionary upper bound (default: 10000000" << std::endl;
  std::cerr << "       -level   max level to load from the language models (default: 1000," << std::endl;
  std::cerr << "           meaning the actual LM order)" << std::endl;
  std::cerr << "       -mm 1    memory-mapped access to lm (default: 0)" << std::endl;
  std::cerr << std::endl;
	exit(1);
}

int main(int argc, char **argv)
{
  int mmap = 0;
  int dub = 10000000;
  int requiredMaxlev = 1000;
  char *lm = NULL;

  for(int i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "-mm")) {
      if(++i == argc)
        usage();
      mmap = atoi(argv[i]);
    } else if(!strcmp(argv[i], "-dub")) {
      if(++i == argc)
        usage();
      dub = atoi(argv[i]);
    } else if(!strcmp(argv[i], "-lm")) {
      if(++i == argc)
        usage();
      lm = argv[i];
    } else if(!strcmp(argv[i], "-level")) {
      if(++i == argc)
        usage();
      requiredMaxlev = atoi(argv[i]);
    } else
      usage();
  }

  if(lm == NULL)
    usage();

  std::ifstream lmstr(lm);
  lmtable lmt;
  lmt.setMaxLoadedLevel(requiredMaxlev);
  lmt.load(lmstr, lm, NULL, mmap);
  lmt.setlogOOVpenalty(dub);
  lmt.setlogOOVpenalty(0.0);

  for(;;) {
    std::string line;
    std::getline(std::cin, line);
    if(!std::cin.good())
      return !std::cin.eof();
		line = "<s> " + line + " </s>";

    std::istringstream linestr(line);
    ngram ng(lmt.dict);
		linestr >> ng;

    while((linestr >> ng)){
    	double logprob = lmt.lprob(ng);
    	std::cout << logprob << std::endl;
		}
  }
}
